/**
 * @file canon.c
 * Functions to convert the intermediate tree language
 * into basic blocks and traces.
 */

#include <assert.h>
#include <stdio.h>

#include "include/linked_list.h"
#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/canon.h"

typedef linked_list exp_ref_list;
typedef struct _stm_exp stm_exp;

struct _stm_exp
{
  tree_stm *stm;
  tree_exp *exp;
};

/* Local function prototypes */

static tree_stm *            do_stm    (tree_stm *stm);

static stm_exp *             do_exp    (tree_exp *exp);

static canon_stmlist_list *  mk_blocks (int            stms_index,
                                        tree_stm_list *stms,
                                        temp_label    *done);

static tree_stm_list *       get_next  (void);

/* End local function prototypes */

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
reorder (int           index,
         exp_ref_list *ref_list)
{
  if (ref_list == NULL
      || linked_list_length (ref_list) <= index)
    {
     return tree_new_exp (tree_new_const (0)); /* nop */
    }

  tree_exp *exp = *(tree_exp**)linked_list_get (ref_list, index);
  if (exp->kind == TREE_CALL)
    {
      temp_temp *temp = temp_new_temp();

      exp = tree_new_eseq (tree_new_move (tree_new_temp (temp), exp),
                           tree_new_temp(temp));
      return reorder (index, ref_list);
    }
  else
    {
      stm_exp *hd = do_exp (exp);
      index++;
      tree_stm *s = reorder (index, ref_list);

      if (commute (s, hd->exp))
        {
          exp = hd->exp;
          return seq (hd->stm, s);
        }
      else
        {
          temp_temp *t = temp_new_temp();

          exp = tree_new_temp (t);
          return seq (hd->stm,
                      seq (tree_new_move(tree_new_temp (t), hd->exp), s));
        }
    }
}

static exp_ref_list *
get_call_ref_list (tree_exp *exp)
{
 tree_exp_list *args    = exp->u.call.args;
 exp_ref_list *ref_list = linked_list_new ();

 linked_list_add (ref_list, &exp->u.call.fun);

 for (int i = 0; i < linked_list_length (args); i++)
   {
     tree_exp *exp = (tree_exp*)linked_list_get (args, i);
     linked_list_add (ref_list, &exp);
   }
  return ref_list;
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
  switch (exp->kind)
    {
  case TREE_BINOP:
    {
      exp_ref_list *ref_list = linked_list_new ();
      linked_list_add (ref_list, &exp->u.bin_op.left);
      linked_list_add (ref_list, &exp->u.bin_op.right);

      tree_stm* stm = reorder (0, ref_list);

      return new_stm_exp (stm, exp);
    }
  case TREE_MEM:
    {
      exp_ref_list *ref_list = linked_list_new ();
      linked_list_add (ref_list, &exp->u.mem);

      return new_stm_exp (reorder (0, ref_list), exp);
    }

    case TREE_ESEQ:
      {
        stm_exp *stm_exp = do_exp (exp->u.eseq.exp);
        return new_stm_exp (seq (do_stm (exp->u.eseq.stm), stm_exp->stm),
                            stm_exp->exp);
      }

  case TREE_CALL:
    return new_stm_exp (reorder (0, get_call_ref_list (exp)), exp);

  default:
    return new_stm_exp (reorder (0, NULL), exp);
  }
}

/* processes stm so that it contains no eseq nodes */
static tree_stm *
do_stm (tree_stm *stm)
{
  switch (stm->kind)
    {
    case TREE_SEQ:
      return seq (do_stm (stm->u.seq.left), do_stm (stm->u.seq.right));

    case TREE_JUMP:
      {
        exp_ref_list *ref_list = linked_list_new ();
        linked_list_add (ref_list, &stm->u.jmp.exp);

        return seq (reorder (0, ref_list), stm);
      }

    case TREE_CJUMP:
      {
        exp_ref_list *ref_list = linked_list_new ();
        linked_list_add (ref_list, &stm->u.cjump.left);
        linked_list_add (ref_list, &stm->u.cjump.right);

        return seq (reorder (0, ref_list), stm);
      }

  case TREE_MOVE:
    if (stm->u.move.dst->kind == TREE_TEMP
        && stm->u.move.src->kind == TREE_CALL)
      return seq(reorder(0, get_call_ref_list(stm->u.move.src)), stm);

    else if (stm->u.move.dst->kind == TREE_TEMP)
      {
        exp_ref_list *ref_list = linked_list_new ();
        linked_list_add (ref_list, &stm->u.move.src);

        return seq (reorder (0, ref_list), stm);
      }

    else if (stm->u.move.dst->kind == TREE_MEM)
      {
        exp_ref_list *ref_list = linked_list_new ();
        linked_list_add (ref_list, &stm->u.move.dst->u.mem);
        linked_list_add (ref_list, &stm->u.move.src);

        return seq (reorder (0, ref_list), stm);
      }

    else if (stm->u.move.dst->kind == TREE_ESEQ)
      {
        tree_stm *s     = stm->u.move.dst->u.eseq.stm;
        stm->u.move.dst = stm->u.move.dst->u.eseq.exp;

        return do_stm (tree_new_seq (s, stm));
      }

    assert(0); /* dst should be temp or mem only */

    case TREE_EXP:
      if (stm->u.exp->kind == TREE_CALL)
        return seq (reorder(0, get_call_ref_list (stm->u.exp)), stm);

      else
        {
          exp_ref_list *ref_list = linked_list_new ();
          linked_list_add (ref_list, &stm->u.exp);

          return seq (reorder (0, ref_list), stm);
        }

    default:
      return stm;
 }
}

/* linear gets rid of the top-level SEQ's, producing a list */
static tree_stm_list *
linear (int right_index, tree_stm *stm, tree_stm_list *right)
{
  if (stm->kind == TREE_SEQ)
    {
      right_index++;
      return linear (0, stm->u.seq.left, linear (right_index,
                                                 stm->u.seq.right,
                                                 right));
    }

  tree_stm_list *stm_list = linked_list_new ();
  linked_list_add (stm_list, stm);

  if (right == NULL)
    return stm_list;

  linked_list_catn_list (stm_list, right, right_index);

  return stm_list;
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
  return linear (0, do_stm(stm), NULL);
}

/* Go down a list looking for end of basic block */
static canon_stmlist_list *
next (int            stms_index,
      tree_stm_list *prevstms,
      tree_stm_list *stms,
      temp_label    *done)
{
  if (stms == NULL || linked_list_length (stms) == stms_index)
    {
      temp_label_list *label_list = linked_list_new ();
      tree_stm_list   *stm_list   = linked_list_new ();
      linked_list_add (label_list, done);
      linked_list_add (stm_list,
                       tree_new_jump (tree_new_name (done), label_list));

      return next (0, prevstms, stm_list, done);
    }

  tree_stm *stm = linked_list_get (stms, stms_index);
  if (stm->kind == TREE_JUMP || stm->kind == TREE_CJUMP)
    {
      canon_stmlist_list *stmlist_list;

      linked_list_catn_list (prevstms, stms, stms_index);

      stms_index++;
      stmlist_list = mk_blocks (stms_index, stms, done);
      linked_list_remove (stms, stms_index);

      return stmlist_list;
  }

  else if (stm->kind == TREE_LABEL)
    {
      tree_stm_list   *stm_list   = linked_list_new ();
      temp_label_list *label_list = linked_list_new ();
      temp_label      *lab        = stm->u.label;
      linked_list_add (label_list, lab);
      linked_list_add (stm_list,
                       tree_new_jump (tree_new_name (lab), label_list));

      linked_list_catn_list (stm_list, stms, stms_index);

      return next (stms_index, prevstms, stm_list, done);
    }

  else
    {
      linked_list_catn_list (prevstms, stms, stms_index);

      stms_index++;
      return next (stms_index, stms, stms, done);
  }
}

/* Create the beginning of a basic block */
static canon_stmlist_list *
mk_blocks (int            stms_index,
           tree_stm_list *stms,
           temp_label    *done)
{
  if (stms == NULL || linked_list_length (stms) == stms_index)
    {
      return NULL;
    }

  tree_stm *s = linked_list_get (stms, stms_index);
  if (s->kind != TREE_LABEL)
    {
      tree_stm_list *stm_list = linked_list_new ();
      linked_list_add (stm_list, tree_new_label (temp_new_label ()));
      linked_list_catn_list (stm_list, stms, stms_index);

      return mk_blocks (stms_index, stm_list, done);
    }
  /* else there already is a label */
  canon_stmlist_list *stmlist_list = linked_list_new ();
  linked_list_catn_list (stmlist_list, stms, stms_index);
  stms_index++;
  linked_list_add (stmlist_list, next(stms_index, stms, stms, done));

  return stmlist_list;
}

/**
 * BasicBlocks : Tree.stm list -> (Tree.stm list list * Tree.label)
 * From a list of cleaned trees, produce a list of
 * basic blocks satisfying the following properties:
 *	  1. and 2. as above;
 *	  3.  Every block begins with a LABEL;
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
canon_block *
canon_basic_blocks (tree_stm_list *stm_list)
{
  canon_block *b = new (sizeof (*b));

  b->label     = temp_new_label ();
  b->stm_lists = mk_blocks (0, stm_list, b->label);

  return b;
}

static int global_block_stmlist_index = 0;
static sym_table   *block_env;
static canon_block *global_block;

static tree_stm_list *
get_last(tree_stm_list *list)
{
  return linked_list_get (list, linked_list_length (list) - 1);
  tree_stm_list *last = list;

  while (last->tail->tail) last = last->tail;
  return last;
}

static void trace (int            list_index,
                   tree_stm_list *list)
{
  //tree_stm_list *last = getLast(list);
  tree_stm      *lab = linked_list_get (list, list_index);
  tree_stm      *s   = linked_list_get (list, linked_list_length (list) - 1);

  sym_bind_symbol (block_env, lab->u.label, NULL);

  if (s->kind == TREE_JUMP)
    {
      sym_symbol    *sym = linked_list_get(s->u.jmp.jumps, 0);
      tree_stm_list *target = (tree_stm_list*)sym_lookup (block_env, sym);

      if (!s->u.jmp.jumps->tail && target) {
        last->tail = target; /* merge the 2 lists removing JUMP stm */
        trace(target);
    }
    else last->tail->tail = getNext(); /* merge and keep JUMP stm */
  }
  /* we want false label to follow CJUMP */
  else if (s->kind == T_CJUMP) {
    T_stmList true =  (T_stmList) S_look(block_env, s->u.CJUMP.true);
    T_stmList false =  (T_stmList) S_look(block_env, s->u.CJUMP.false);
    if (false) {
      last->tail->tail = false;
      trace(false);
    }
    else if (true) { /* convert so that existing label is a false label */
      last->tail->head = T_Cjump(T_notRel(s->u.CJUMP.op), s->u.CJUMP.left,
				 s->u.CJUMP.right, s->u.CJUMP.false,
				 s->u.CJUMP.true);
      last->tail->tail = true;
      trace(true);
    }
    else {
      Temp_label false = Temp_newlabel();
      last->tail->head = T_Cjump(s->u.CJUMP.op, s->u.CJUMP.left,
				 s->u.CJUMP.right, s->u.CJUMP.true, false);
      last->tail->tail = T_StmList(T_Label(false), getNext());
    }
  }
  else assert(0);
}

/* get the next block from the list of stmLists, using only those that have
 * not been traced yet */
static tree_stm_list *
get_next ()
{
  if (!global_block->stm_lists
      || linked_list_length (global_block->stm_lists)
      <= global_block_stmlist_index)
    {
      tree_stm_list *stmlist = linked_list_new ();
      linked_list_add (stmlist, tree_new_label (global_block->label));
      return stmlist;
    }
  else
    {
      tree_stm_list *s = linked_list_get(global_block->stm_lists,
                                         global_block_stmlist_index);
    if (sym_lookup (block_env, s->head->u.LABEL)) {/* label exists in the table */
      trace(s);
      return s;
    }
    else {
      global_block.stmLists = global_block.stmLists->tail;
      return getNext();
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
 *	    in this reordering as many JUMP(T.NAME(lab)) statements
 *      as possible are eliminated by falling through into T.LABEL(lab).
 *
 * @param b canon_block.
 *
 * @return tree_stm_list.
 */
tree_stm_list *
canon_trace_schedule (canon_block *b)
{
  canon_stmlist_list *stmlist_list = linked_list_new ();

  block_env    = sym_new_table ();
  global_block = b;
  stmlist_list = global_block->stm_lists;

  canon_stmlist_list *stmlist;
  LINKED_LIST_FOR_EACH (stmlist, stmlist_list)
    {
      tree_stm *stm = linked_list_get (stmlist, 0);
      sym_bind_symbol (block_env, stm->u.label, stm);
    }

  global_block_stmlist_index++;
  return get_next();
}
