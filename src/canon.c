/**
 * @file canon.c
 * Functions to convert the intermediate tree language
 * into basic blocks and traces.
 */

#include <assert.h>
#include <stdio.h>

//#include "include/list.h"
#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/canon.h"

typedef struct _exp_ref_list exp_ref_list;
typedef struct _stm_exp      stm_exp;

struct
_stm_exp
{
  tree_stm *stm;
  tree_exp *exp;
};

struct
_exp_ref_list
{
  tree_exp     **head;
  exp_ref_list  *tail;
};

/* Local function prototypes */

static tree_stm *            do_stm    (tree_stm *stm);

static stm_exp *             do_exp    (tree_exp *exp);

static canon_stmlist_list *  mk_blocks (tree_stm_list *stms,
                                        temp_label    *done);

static tree_stm_list *       get_next  (void);

/* End local function prototypes */

canon_stmlist_list *
canon_new_stmlist_list (tree_stm_list      *head,
                        canon_stmlist_list *tail)
{
  canon_stmlist_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

exp_ref_list *
new_exp_ref_list (tree_exp     **head,
                  exp_ref_list  *tail)
{
  exp_ref_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

static bool
is_nop (tree_stm *x)
{
  return x->kind == TREE_EXP && x->u.exp->kind == TREE_CONST;
}

static tree_stm *
seq (tree_stm *x,
     tree_stm *y)
{
  if (is_nop (x))
    return y;
  if (is_nop (y))
    return x;

  return tree_new_seq (x, y);
}

static bool
commute (tree_stm *x,
         tree_exp *y)
{
  if (is_nop (x))
    return true;

  if (y->kind == TREE_NAME || y->kind == TREE_CONST)
    return true;

  return false;
}


static tree_stm *
reorder (exp_ref_list *ref_list)
{
  if (!ref_list)
    {
      return tree_new_exp (tree_new_const (0)); /* nop */
    }
  else if ((*(tree_exp**)ref_list->head)->kind == TREE_CALL)
    {
      temp_temp *t = temp_new_temp();
      *ref_list->head =
        tree_new_eseq (tree_new_move (tree_new_temp(t),
                                      *ref_list->head),
                       tree_new_temp(t));
      return reorder (ref_list);
    }
  else
    {
      stm_exp  *hd = do_exp (*(tree_exp**)ref_list->head);
      tree_stm *s  = reorder (ref_list->tail);
      if (commute (s, hd->exp))
        {
          *(tree_exp**)ref_list->head = hd->exp;
          return seq (hd->stm, s);
        }
      else
        {
          temp_temp *t = temp_new_temp();
          *(tree_exp**)ref_list->head = tree_new_temp (t);
          return seq (hd->stm,
                      seq (tree_new_move (tree_new_temp (t),hd->exp), s));
        }
    }
}

static exp_ref_list *
get_call_ref_list (tree_exp *exp)
{
  exp_ref_list  *rlist, *curr;
  tree_exp_list *args = exp->u.call.args;

  curr = rlist = new_exp_ref_list (&exp->u.call.fun, NULL);
  for (;args; args = args->tail)
    {
      curr = curr->tail = new_exp_ref_list (&args->head, NULL);
    }
  return rlist;
}

static stm_exp *
new_stm_exp (tree_stm *stm,
             tree_exp *exp)
{
  stm_exp *stm_exp = new (sizeof (*stm_exp));

  stm_exp->stm = stm;
  stm_exp->exp = exp;

  return stm_exp;
}

static stm_exp *
do_exp (tree_exp *exp)
{
  switch(exp->kind)
    {
    case TREE_BINOP:
      {
        exp_ref_list *rlist = new_exp_ref_list (&exp->u.bin_op.right, NULL);
        rlist = new_exp_ref_list (&exp->u.bin_op.left, rlist);

        return new_stm_exp (reorder (rlist), exp);
      }
    case TREE_MEM:
      {
        return new_stm_exp (reorder (new_exp_ref_list (&exp->u.mem, NULL)),
                            exp);
      }
    case TREE_ESEQ:
      {
        stm_exp *x = do_exp(exp->u.eseq.exp);
        return new_stm_exp (seq(do_stm(exp->u.eseq.stm), x->stm), x->exp);
      }
    case TREE_CALL:
      {
        return new_stm_exp (reorder(get_call_ref_list (exp)), exp);
      }
    default:
      {
        return new_stm_exp (reorder(NULL), exp);
      }
    }
}

/* processes stm so that it contains no eseq nodes */
static tree_stm *
do_stm (tree_stm *stm)
{
  switch (stm->kind)
    {
    case TREE_SEQ:
      return seq(do_stm(stm->u.seq.left), do_stm(stm->u.seq.right));

    case TREE_JUMP:
      return seq (reorder (new_exp_ref_list (&stm->u.jmp.exp, NULL)), stm);

    case TREE_CJUMP:
      return seq(reorder (new_exp_ref_list (&stm->u.cjump.left,
                                            new_exp_ref_list (&stm->u.cjump.right,
                                                              NULL))),
                 stm);

    case TREE_MOVE:
      if (stm->u.move.dst->kind == TREE_TEMP &&
            stm->u.move.src->kind == TREE_CALL)
        return seq (reorder(get_call_ref_list (stm->u.move.src)), stm);

      else if (stm->u.move.dst->kind == TREE_TEMP)
        return seq(reorder(new_exp_ref_list (&stm->u.move.src, NULL)), stm);

      else if (stm->u.move.dst->kind == TREE_MEM)
        return seq(reorder(new_exp_ref_list (&stm->u.move.dst->u.mem,
                                             new_exp_ref_list (&stm->u.move.src,
                                                               NULL))),
                   stm);

      else if (stm->u.move.dst->kind == TREE_ESEQ)
        {
          stm->u.move.dst = stm->u.move.dst->u.eseq.exp;
          return do_stm (tree_new_seq (stm->u.move.dst->u.eseq.stm, stm));
        }
      assert(0); /* dst should be temp or mem only */

    case TREE_EXP:
      if (stm->u.exp->kind == TREE_CALL)
        return seq (reorder (get_call_ref_list (stm->u.exp)), stm);
      else
        return seq (reorder (new_exp_ref_list (&stm->u.exp, NULL)), stm);

    default:
      return stm;
    }
}

/* linear gets rid of the top-level SEQ's, producing a list */
static tree_stm_list *
linear (tree_stm *stm, tree_stm_list *right)
{
  if (stm->kind == TREE_SEQ)
    return linear (stm->u.seq.left, linear (stm->u.seq.right, right));
  else
    return tree_new_stm_list (stm, right);
}

/**
 * From an arbitrary Tree statement, produce a list of cleaned trees
 * satisfying the following properties:
 *    1.  No SEQ's or ESEQ's
 *    2.  The parent of every CALL is an EXP(..) or a MOVE(TEMP t,..)
 *
 * @param stm Tree statement.
 *
 * @returns Linearized list.
 */
tree_stm_list *
canon_linearize (tree_stm *stm)
{
  return linear (do_stm (stm), NULL);
}

/* Go down a list looking for end of basic block */
static canon_stmlist_list *
next (tree_stm_list *prevstms,
      tree_stm_list *stms,
      temp_label    *done)
{
  if (!stms)
    return next (prevstms,
                 tree_new_stm_list (tree_new_jump (tree_new_name (done),
                                                   temp_new_label_list (done,
                                                                        NULL)),
                                    NULL),
                 done);

  tree_stm *stm = stms->head;
  if (stm->kind == TREE_JUMP || stm->kind == TREE_CJUMP)
    {
      prevstms->tail = stms;
      canon_stmlist_list *stmlist_list = mk_blocks (stms->tail, done);
      stms->tail = NULL;
      return stmlist_list;
  }
  else if (stm->kind == TREE_LABEL)
    {
      temp_label *lab = stm->u.label;
      return next (prevstms, tree_new_stm_list (tree_new_jump (tree_new_name (lab),
                                                               temp_new_label_list (lab,
                                                                                    NULL)),
                                                stms),
                   done);
    }
  else
    {
      prevstms->tail = stms;
      return next (stms, stms->tail, done);
    }
}

/* Create the beginning of a basic block */
static canon_stmlist_list *
mk_blocks (tree_stm_list *stms,
           temp_label    *done)
{
  if (!stms)
    {
      return NULL;
    }
  tree_stm *stm = stms->head;
  if (stm->kind != TREE_LABEL)
    {
      return mk_blocks (tree_new_stm_list (tree_new_label (temp_new_label()),
                                           stms),
                        done);
    }
  /* else there already is a label */
  return canon_new_stmlist_list (stms, next(stms, stms->tail, done));
}

/**
 * BasicBlocks : Tree.stm list -> (Tree.stm list list * Tree.label)
 * From a list of cleaned trees, produce a list of
 * basic blocks satisfying the following properties:
 *    1. and 2. as above;
 *    3.  Every block begins with a LABEL;
 *    4.  A LABEL appears only at the beginning of a block;
 *    5.  Any JUMP or CJUMP is the last stm in a block;
 *    6.  Every block ends with a JUMP or CJUMP;
 *        Also produce the "label" to which control will be passed
 *        upon exit.
 *
 * @param stm_list Tree statement list.
 *
 * @returns canon_block.
 */
canon_block
canon_basic_blocks (tree_stm_list *stm_list)
{
  canon_block b;

  b.label     = temp_new_label ();
  b.stm_lists = mk_blocks (stm_list, b.label);

  return b;
}

static sym_table   *block_env;
static canon_block  global_block;

static tree_stm_list *
get_last(tree_stm_list *list)
{
  tree_stm_list *last = list;
  while (last->tail->tail)
    last = last->tail;

  return last;
}

static void
trace (tree_stm_list *list)
{
  tree_stm_list *last = get_last (list);
  tree_stm      *lab  = list->head;
  tree_stm      *s    = last->tail->head;

  sym_bind_symbol (block_env, lab->u.label, NULL);
  if (s->kind == TREE_JUMP)
    {
      tree_stm_list *target = (tree_stm_list*) sym_lookup (block_env,
                                                         s->u.jmp.jumps->head);
      if (!s->u.jmp.jumps->tail && target)
        {
          last->tail = target; /* merge the 2 lists removing JUMP stm */
          trace (target);
        }
      else
        {
          last->tail->tail = get_next(); /* merge and keep JUMP stm */
        }
    }
  /* we want false label to follow CJUMP */
  else if (s->kind == TREE_CJUMP)
    {
      tree_stm_list *truee  = (tree_stm_list*) sym_lookup (block_env,
                                                         s->u.cjump.truee);
      tree_stm_list *falsee = (tree_stm_list*) sym_lookup (block_env,
                                                           s->u.cjump.falsee);
      if (falsee)
        {
          last->tail->tail = falsee;
          trace(falsee);
        }
      else if (truee)
        { /* convert so that existing label is a false label */
          last->tail->head = tree_new_cjump (tree_not_rel (s->u.cjump.op),
                                             s->u.cjump.left,
                                             s->u.cjump.right,
                                             s->u.cjump.falsee,
                                             s->u.cjump.truee);
          last->tail->tail = truee;
          trace(truee);
        }
      else
        {
          temp_label *falsee = temp_new_label ();
          last->tail->head = tree_new_cjump (s->u.cjump.op,
                                             s->u.cjump.left,
                                             s->u.cjump.right,
                                             s->u.cjump.truee,
                                             falsee);
          last->tail->tail = tree_new_stm_list (tree_new_label (falsee),
                                                get_next ());
        }
    }
  else
    {
      assert(0);
    }
}

/* get the next block from the list of stmLists, using only those that have
 * not been traced yet */
static tree_stm_list *
get_next ()
{
  if (!global_block.stm_lists)
    {
      return tree_new_stm_list (tree_new_label (global_block.label), NULL);
    }
  else
    {
      tree_stm_list *s = global_block.stm_lists->head;
      tree_stm *stm    = s->head;
      if (sym_lookup (block_env, stm->u.label))
        {/* label exists in the table */
          trace(s);
          return s;
        }
      else
        {
          global_block.stm_lists = global_block.stm_lists->tail;
          return get_next ();
        }
    }
}

/**
 * traceSchedule : Tree.stm list list * Tree.label -> Tree.stm list
 * From a list of basic blocks satisfying properties 1-6,
 * along with an "exit" label,
 * produce a list of stms such that:
 *   1. and 2. as above;
 *   7. Every CJUMP(_,t,f) is immediately followed by LABEL f.
 *      The blocks are reordered to satisfy property 7; also
 *      in this reordering as many JUMP(T.NAME(lab)) statements
 *      as possible are eliminated by falling through into T.LABEL(lab).
 *
 * @param b canon_block.
 *
 * @return tree_stm_list.
 */
tree_stm_list *
canon_trace_schedule (canon_block b)
{
  canon_stmlist_list *stmlist_list;
  block_env    = sym_new_table ();
  global_block = b;

  for (stmlist_list = global_block.stm_lists;
       stmlist_list;
       stmlist_list = stmlist_list->tail)
    {
      tree_stm_list *stm_list = stmlist_list->head;
      tree_stm      *stm      = stm_list->head;
      sym_bind_symbol (block_env, stm->u.label, stmlist_list->head);
    }
  return get_next();
}
