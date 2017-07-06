/**
 * @file translate.c
 * Description see translate.h
 */

#include <assert.h>

#include "include/list.h"
#include "include/frame.h"
#include "include/translate.h"
#include "include/tree.h"

typedef struct _condit_exp condit_exp;
typedef        list        patch_list;

/* Private global list to save all function and string fragments */
frm_frag_list *frag_list = NULL, *sfrag_list = NULL;

/**
 * Saves nested level of variable and access frame.
 */
struct
_tra_access
{
  tra_level  *level;
  frm_access *access;
};

struct
_tra_level
{
  tra_level       *parent;
  frm_frame       *frame;
  //tra_access_list *formals;
  //tra_access_list *locals;
};

struct
_condit_exp
{
  patch_list *trues;
  patch_list *falses;
  tree_stm   *stm;
};


struct
_tra_exp
{
  enum
    {
      TRA_EXP,
      TRA_NO_RES,
      TRA_CONDITIONAL
    } kind;

  union
  {
    tree_exp   *exp;
    tree_stm   *no_res;
    condit_exp *conditional;
  } u;
};

struct
_patch_list
{
  temp_label *head;
  patch_list *tail;
};


static tra_exp *    trans_exp             (tree_exp *exp_ptr);

static tra_exp *    trans_no_res_exp      (tree_stm *no_res_ptr);

static tra_exp *    trans_conditional_exp (patch_list *trues_ptr,
                                           patch_list *falses_ptr,
                                           tree_stm   *stm_ptr);

static tree_exp *   conv_exp              (tra_exp *exp_ptr);

static tree_stm *   conv_no_res_exp       (tra_exp *exp_ptr);

static condit_exp * conv_conditional_exp  (tra_exp *exp_ptr);

static patch_list * join_patch            (patch_list *first_ptr,
                                           patch_list *second_ptr);

static void         do_patch              (patch_list *tlist_ptr,
                                           temp_label *label_ptr);

static tree_exp *   get_offset            (tra_level *used_level_ptr,
                                           tra_level *declared_level_ptr,
                                           temp_temp *frame_pointer_ptr);

static void         frag_list_add         (frm_frag *frag_ptr);

static condit_exp * new_condit            (tree_stm   *stm_ptr,
                                           patch_list *trues_ptr,
                                           patch_list *falses_ptr);

static tra_access_list * new_formals      (tra_level *level);


/**
 * Creates a new nesting level for a function.
 * Adds static link to formals_ptr list,
 * then calls frm_new_frame() to create a new frame for that level.
 *
 * @param parent_ptr  The upper level.
 * @param name_ptr    Name of the function.
 * @param formals_ptr Escape values for formals.
 */
tra_level *
tra_new_level (tra_level      *parent_ptr,
               temp_label     *name_ptr,
               util_bool_list *formals_ptr)
{
  tra_level *new_level   = new (sizeof (*new_level));
  //bool      *static_link = new (sizeof (*static_link));
  //*static_link = true;

  //formals_ptr = list_new_list (static_link, formals_ptr); /* Add static link */

  new_level->frame   = frm_new_frame (name_ptr, formals_ptr);
  new_level->parent  = parent_ptr;
  //new_level->formals = new_formals (new_level);
  //new_level->locals  = NULL;

  return new_level;
}

tra_access *
tra_new_access (tra_level  *level,
                frm_access *frm_access)
{
  tra_access *access = new (sizeof (*access));

  access->access = frm_access;
  access->level  = level;

  return access;
}

/**
 * Creates tra_access struture, to track nested level of variable.
 * Calls frm_alloc_local().
 *
 * @param level  The nested level of variable.
 * @param escape true if variable escapes. false if variable does not escape.
 *
 * @return tra_access structure.
 */
tra_access *
tra_alloc_local (tra_level *level,
                 bool       escape)
{
  return tra_new_access (level, frm_alloc_local (level->frame, escape));
}

/**
 * Creates a access list for all formals from given level.
 *
 * @param level The level
 *
 * @return tra_access_list with all the formals translated in access structs.
 */
tra_access_list *
tra_formals (tra_level *level)
{
  frm_access_list *access_list = frm_formals (level->frame);
  tra_access_list *a = NULL, *last_a = NULL;
  for (; access_list; access_list = access_list->tail)
    {
      if (last_a == NULL)
        {
          a = list_new_list (tra_new_access (level, access_list->head), NULL);
          last_a = a;
        }
      else
        {
          last_a->tail = list_new_list (tra_new_access (level,
                                                        access_list->head),
                                        NULL);
          last_a = last_a->tail;
        }
    }
  return a;
  // return level_ptr->formals;
}

/**
 * Returns the outermost level for a tiger program.
 *
 * @return Outermost level.
 */
tra_level *
tra_outermost_level (void)
{
  static tra_level *outermost_level = NULL;

  if (outermost_level == NULL)
      return outermost_level = tra_new_level (NULL,
                                              temp_named_label ("tigermain"),
                                              NULL);

  return outermost_level;
}

void
tra_proc_entry_exit (tra_level       *level,
                     tra_exp         *body,
                     tra_access_list *formals)
{
  tree_stm *stm = tree_new_move (tree_new_temp (frm_rv ()), conv_exp (body));
  frm_frag *frag = frm_proc_frag (stm, level->frame);
  frag_list = list_new_list (frag, frag_list);
  frm_proc_entry_exit1 (level->frame, stm);
}

/**
  * Translates a integer to the intermediate language.
  *
  * @num The integer.
  *
  * @return Intermediate code represenation.
  */
tra_exp *
tra_int_exp (int num)
{
  return trans_exp (tree_new_const (num));
}

/**
 * Translates the access of a simple Variable into the intermediate code
 * representation.
 *
 * @param access The access to the variable.
 * @param level  The level in wich the variable is used.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_simple_var (tra_access *access,
                tra_level  *level)
{
  /* Variable is declared in the same level */
  if (level == access->level)
    {
      return trans_exp (frm_exp (access->access,
                                 tree_new_temp (frm_fp ())));
    }
  else /* Calculate offset with static links. */
    {
      tree_exp *static_link = frm_static_link_exp (tree_new_temp (frm_fp ()));
      /*
        tree_exp *mem = get_offset (level,
                                  access->level,
                                  frm_fp ());

      return trans_exp (frm_exp (access->access, mem));
      */
      while (access->level != level)
        {
          static_link = frm_upper_static_link_exp (static_link);
          level = level->parent;
        }
      return trans_exp (frm_exp_with_static_link (access->access, static_link));
    }
}

/**
 * Calculates the offset of a field variable and turns it
 * into the intermediate code representation.
 *
 * @param record_ptr The memory from wich the offset must be calculated.
 * @param offset      The actual offset.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_field_var (tra_exp *record_ptr,
               int      offset)
{
  tree_exp *field_pos    = tree_new_const (offset);
  tree_exp *word_size    = tree_new_const (frm_word_size);
  tree_exp *field_offset = tree_new_bin_op (TREE_TIMES, field_pos, word_size);
  tree_exp *field        = tree_new_bin_op (TREE_PLUS,
                                            conv_exp (record_ptr),
                                            field_offset);
  tree_exp *mem          = tree_new_mem (field);

  return trans_exp (mem);
}

/**
 * Calculates the offset of a array variable and turns it
 * into the intermediate code representation.
 *
 * @param array_ptr The memory location of the first array element.
 * @param index_ptr The index of the element.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_subscript_var (tra_exp* array_ptr,
                   tra_exp* index_ptr)
{
  tree_exp *index          = conv_exp (index_ptr);
  tree_exp *word_size      = tree_new_const (frm_word_size);
  tree_exp *element_offset = tree_new_bin_op (TREE_TIMES, index, word_size);
  tree_exp *element        = tree_new_bin_op (TREE_PLUS,
                                              conv_exp (array_ptr),
                                              element_offset);
  tree_exp *mem            = tree_new_mem (element);

  return trans_exp (mem);
}

/**
  * Creates the tree represenation of a arithmetic expression.
  * If operator is not +, -, *, or / then + will be taken as operator.
  *
  * @param left_ptr  Left operand.
  * @param right_ptr Right operand.
  * @param op        Operator.
  *
  * @return The intermediate code represenation.
  */
tra_exp *
tra_arithmetic_exp (tra_exp* left_ptr,
                    tra_exp* right_ptr,
                    TRA_OP   operator)
{
  tree_exp *op;

  switch (operator)
    {
    case TRA_PLUS:
      op = tree_new_bin_op (TREE_PLUS,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr));
      break;

    case TRA_MINUS:
      op = tree_new_bin_op (TREE_MINUS,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr));
      break;

    case TRA_TIMES:
      op = tree_new_bin_op (TREE_TIMES,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr));
      break;

    case TRA_DIVIDE:
      op = tree_new_bin_op (TREE_DIVIDE,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr));
      break;

    default:
      assert (0);
    }

  return trans_exp (op);
}

/**
  * Creates the tree represenation of a arithmetic expression.
  * If operator is not <, <=, >, >=, = or <> then =  will be taken as operator.
  *
  * @param left_ptr  Left operand.
  * @param right_ptr Right operand.
  * @param op        Operator.
  *
  * @return The intermediate code represenation.
  */
tra_exp *
tra_conditional_exp (tra_exp *left_ptr,
                     tra_exp *right_ptr,
                     TRA_OP   op)
{
  tree_stm *stm;
  switch (op)
    {
    case TRA_LT:
      stm = tree_new_cjump (TREE_LT,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
      break;

    case TRA_LE:
      stm = tree_new_cjump (TREE_LE,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
      break;

    case TRA_GT:
      stm = tree_new_cjump (TREE_GT,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
        break;

    case TRA_GE:
      stm = tree_new_cjump (TREE_GE,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
      break;

    case TRA_EQ:
      stm = tree_new_cjump (TREE_EQ,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
      break;

    case TRA_NEQ:
      stm = tree_new_cjump (TREE_NEQ,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
      break;

    default:
      assert (0);
      stm = tree_new_cjump (TREE_EQ,
                            conv_exp (left_ptr),
                            conv_exp (right_ptr),
                            NULL,
                            NULL);
      break;
    }

  patch_list *trues  = list_new_list (&stm->u.cjump.truee, NULL);
  patch_list *falses = list_new_list (&stm->u.cjump.falsee, NULL);

  return trans_conditional_exp (trues, falses, stm);
}

/**
 * Special conditional if comparing two strings.
 *
 * @param o Operator.
 * @param left Left expression.
 * @param right Expression.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_str_cond_exp(TRA_OP   o,
                 tra_exp *left,
                 tra_exp *right)
{
  tree_rel_op op = TREE_PLUS;
  tree_stm *s;
  switch (o)
    {
    case TRA_EQ:
      op = TREE_EQ;
      break;
    case TRA_NEQ:
      op = TREE_NEQ;
      break;
    case TRA_LT:
      op = TREE_LT;
      break;
    case TRA_LE:
      op = TREE_LE;
      break;
    case TRA_GT:
      op = TREE_GT;
      break;
    case TRA_GE:
      op = TREE_GE;
      break;
    default:
      assert (0);
  }
  /* String equal */
  if (op == TREE_EQ || op == TREE_NEQ)
    {
    tree_exp *e = frm_external_call ("stringEqual",
                                     list_new_list (conv_exp (left),
                                                    list_new_list (conv_exp (right),
                                                                   NULL)));
    s = tree_new_cjump (op, e, tree_new_const (1), NULL, NULL);
    /* String compare */
    }
  else
    {
      assert(0);  // Not implemented
    }
  patch_list *trues = list_new_list (&s->u.cjump.truee, NULL);
  patch_list *falses =list_new_list (&s->u.cjump.falsee, NULL);
  return trans_conditional_exp (trues, falses, s);
}

/**
 * Translates a if expression into intermediate code.
 *
 * @param test_ptr Test condition.
 * @param then_ptr Then expression.
 * @param else_ptr Else expression. This is optional, can be set to NULL.
 */
tra_exp *
tra_if_exp (tra_exp* test_ptr,
            tra_exp* then_ptr,
            tra_exp* else_ptr)
{
  condit_exp *test       = conv_conditional_exp (test_ptr);
  /* Allocate a register for the result */
  tree_exp   *reg_result = tree_new_temp (temp_new_temp ());
  /* Label to jump to when done with if statement */
  temp_label *done       = temp_new_label ();
  /* Allocate labels for true and else, fill expressions with these labels */
  temp_label *truee      = temp_new_label ();
  temp_label *falsee     = temp_new_label ();

  do_patch (test->trues, truee);
  do_patch (test->falses, falsee);

  if (else_ptr != NULL)
    {
      tree_exp *elsee = conv_exp (else_ptr);
      tree_exp *then  = conv_exp (then_ptr);
      /*
        If ... then .. else expression.
        Set label before true and false expression.
        True and false expression move their result
        in the result register result_reg.
        After true and false expression evaluated,
        they jump to the common label done.
      */
      temp_label_list *label_list = list_new_list (done, NULL);

      tree_stm *move_false_to_res = tree_new_move (reg_result, elsee);
      tree_stm *false_label       = tree_new_seq (tree_new_label (falsee),
                                                  move_false_to_res);
      tree_stm *move_true_to_res  = tree_new_move (reg_result, then);
      tree_stm *true_label        = tree_new_seq (tree_new_label (truee),
                                                  move_true_to_res);
      tree_stm *jmp_to_done       = tree_new_jump (tree_new_name (done),
                                                   label_list);
      tree_stm *true_exp          = tree_new_seq (true_label, jmp_to_done);
      tree_stm *false_exp         = tree_new_seq (false_label,
                                                  tree_new_label (done));
      tree_stm *true_false        = tree_new_seq (true_exp, false_exp);
      tree_stm *iff               = tree_new_seq (test->stm, true_false);

      return trans_exp (tree_new_eseq (iff, reg_result));
    }

  /*
    If .. then ... expression.
    Set label before true expression. Set false label.
   */
  tree_stm *then       = conv_no_res_exp (then_ptr);
  tree_stm *true_label = tree_new_seq (tree_new_label (truee), then);
  tree_stm *true_false = tree_new_seq (true_label, tree_new_label (falsee));

  return trans_no_res_exp (tree_new_seq (test->stm, true_false));
}

/**
 * Creates intermediate code represenation for array creation.
 *
 * @param size_ptr The size of the array.
 * @param init_ptr Initial value of every element.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_array_exp (tra_exp *size,
               tra_exp *init)
{
  tree_exp_list *args = list_new_list (conv_exp (init),
                                       list_new_list (conv_exp (size), NULL));
  /* Call external function that handels the init */
  return trans_exp (frm_external_call ("initArray", args));
}

/**
 * Creates intermediate code represenation for record creation.
 *
 * @param list_ptr The initialisation expressions.
 * @param num      The number of initial expressions.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_record_exp (tra_exp_list *tra_list)
{
  /*
    Call external function malloc, save pointer in register record.
    In loop create the move statements to initialize each record field
    with the right value.

  tree_exp      *record = tree_new_temp (temp_new_temp ());
  int            s      = list_length (tra_list) * frm_word_size;
  tree_exp      *size   = tree_new_const (s);
  tree_exp_list *args   = list_new_list (size, NULL);
  tree_exp      *malloc = frm_external_call ("malloc", args);

  tree_stm *seq_start = NULL;
  tree_stm *seq       = NULL;
  tree_stm *init1     = NULL;
  for (int i = 0; tra_list != NULL; tra_list = tra_list->tail, i++)
    {
      tra_exp  *tra   = tra_list->head;
      tree_exp *addr  = tree_new_bin_op (TREE_PLUS,
                                         addr,
                                         tree_new_const (i * frm_word_size));
      tree_stm *init2 = tree_new_move (addr, conv_exp (tra));

      if (init1 != NULL)
        {
          if (seq == NULL)
            {
              seq = tree_new_seq (init1, init2);
              seq_start = seq;
            }
          else
            {
              seq->u.seq.right = tree_new_seq (seq->u.seq.right, init2);
              seq = seq->u.seq.right;
            }
        }
      else
        {
          seq_start = init2;
        }

      init1 = init2;
    }
  tree_stm *init_seq = tree_new_seq (tree_new_move (record, malloc),
                                     seq_start);
  return trans_exp (tree_new_eseq (init_seq, record));
  */
  /* Allocation */
  int field_count = list_length (tra_list);
  temp_temp *r = temp_new_temp ();
  tree_stm * alloc = tree_new_move (tree_new_temp (r),
                  frm_external_call ("allocRecord",
                                     list_new_list (tree_new_const (field_count
                                                                    * frm_word_size)
                                                    , NULL)));

  /* Init fields */
  tree_stm *init = NULL, *current = NULL;
  int field_index = 0;
  for (; tra_list; tra_list = tra_list->tail, field_index++)
    {
      if (init == NULL)
        {
          init = current = tree_new_seq (tree_new_move (tree_new_mem (tree_new_bin_op (TREE_PLUS,
                              tree_new_temp (r),
                              tree_new_const ((field_count - 1 - field_index) * frm_word_size))),
                                conv_exp (tra_list->head)),
                        tree_new_exp (tree_new_const (0)));         /* statements in seq cannot be null */
        }
      else
        {
          current->u.seq.right = tree_new_seq (tree_new_move (tree_new_mem (tree_new_bin_op (TREE_PLUS,
                                    tree_new_temp (r),
                                    tree_new_const ((field_count - 1 - field_index) * frm_word_size))),
                                      conv_exp (tra_list->head)),
                              tree_new_exp (tree_new_const (0)));   /* statements in seq cannot be null */
          current = current->u.seq.right;
        }
    }
  return trans_exp (tree_new_eseq (tree_new_seq (alloc, init),
                                   tree_new_temp (r)));
}

/**
 * Translates a while loop in intermediate code.
 *
 * @param test_ptr Test expression.
 * @param body_ptr Body of while loop.
 *
 * @return Intermediate code represenation.
 */
tra_exp *
tra_while_exp (tra_exp    *test_ptr,
               tra_exp    *body_ptr,
               temp_label *done_ptr)
{
  temp_label *test_label = temp_new_label ();
  temp_label *body_label = temp_new_label ();

  condit_exp *test = conv_conditional_exp (test_ptr);
  tree_stm   *body = conv_no_res_exp (body_ptr);

  do_patch (test->trues, body_label);
  do_patch (test->falses, done_ptr);

  temp_label_list *label_list = list_new_list (test_label, NULL);

  tree_stm *looper   = tree_new_jump (tree_new_name (test_label), label_list);
  tree_stm *test_exp = tree_new_seq (tree_new_label (test_label), test->stm);
  tree_stm *body_exp = tree_new_seq (tree_new_label (body_label),
                                     tree_new_seq (body, looper));
  tree_stm *loop     = tree_new_seq (test_exp, body_exp);

  return trans_no_res_exp (tree_new_seq (loop, tree_new_label (done_ptr)));
}

/**
 * Translates break expression into intermediate code.
 *
 * @param done_ptr Label to jump when break hit.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_break_exp (temp_label *done_ptr)
{
  temp_label_list *label_list = list_new_list (done_ptr, NULL);

  tree_stm *jump = tree_new_jump (tree_new_name (done_ptr), label_list);
  return trans_no_res_exp (jump);
}

/**
 * Translates a assign expression into intermediate code.
 *
 * @param container_ptr The variable where value gets assigned.
 * @param alloc_ptr     The expression which contain the value.
 *
 * @return Intermediate code represenation.
 */
tra_exp *
tra_assign_exp (tra_exp *container_ptr,
                tra_exp *alloc_ptr)
{
  tree_exp *container = conv_exp (container_ptr);
  tree_exp *alloc     = conv_exp (alloc_ptr);

  return trans_no_res_exp (tree_new_move (container, alloc));
}

/**
 * Translates a sequentiell expression into intermediate code.
 *
 * @param list_ptr Translated expressions in list.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_seq_exp (tra_exp_list *tra_list)
{
  /*
  tree_stm *tree_seq = NULL, *stree_seq = NULL;

  if (list_length (tra_list) == 1)
    return tra_list->head;

  for (; tra_list != NULL; tra_list = tra_list->tail)
    {
      tra_exp  *tra = tra_list->head;
      tree_stm *stm = conv_no_res_exp (tra);

      if (tree_seq == NULL)
        {
          stree_seq = tree_seq = tree_new_seq (stm, NULL);
        }
      else if (tree_seq->u.seq.right == NULL)
        {
          tree_seq->u.seq.right = stm;
        }
      else
        {
          tree_seq->u.seq.right = tree_new_seq (tree_seq->u.seq.right, stm);
          tree_seq = tree_seq->u.seq.right;
        }
    }
  assert (tree_seq->u.seq.right);  Error if NULL
  return trans_no_res_exp (stree_seq);
  */
  tra_exp_list *rel = NULL;
  for (; tra_list; tra_list = tra_list->tail)
    {
      rel = list_new_list (tra_list->head, rel);
    }

  tree_exp *seq = tree_new_const (0);
  for (; rel; rel = rel->tail)
    {
      seq = tree_new_eseq (tree_new_exp (seq), conv_exp (rel->head));
    }
  return trans_exp (seq);
}

/**
 * Translates a function call into intermediate code represenation.
 *
 * @param fun_ptr            The function name.
 * @param exp_list_ptr       Function arguments.
 * @param fun_dec_level_ptr  Level where function is declared.
 * @param fun_call_level_ptr Level where function is called.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_call_exp (bool          is_lib_func,
              tra_level    *funclv,
              tra_level    *lv,
              temp_label   *name,
              tra_exp_list *rawel)
{
  /*
  tree_exp_list *tree_list = NULL, *stree_list = NULL;
  tree_exp      *fun                           = tree_new_name (fun_ptr);

  for (; tra_list != NULL; tra_list = tra_list->tail)
    {
      tra_exp  *tra = tra_list->head;
      tree_exp *exp = conv_exp (tra);

      if (tree_list == NULL)
        stree_list = tree_list = list_new_list (exp, NULL);
      else
        tree_list = tree_list->tail = list_new_list (exp, NULL);
    }
   Add static link
  tree_list = list_new_list (get_offset (fun_call_level_ptr,
                                         fun_dec_level_ptr,
                                         frm_fp ()),
                             tree_list);
  return trans_exp (tree_new_call (fun, stree_list));
  */
  tree_exp_list *el = NULL, *last_el = NULL;
  for (; rawel; rawel = rawel->tail)
    {
      if (last_el == NULL)
        {
          last_el = el = list_new_list (conv_exp (rawel->head), NULL);
        }
      else
        {
          last_el->tail = list_new_list (conv_exp (rawel->head), NULL);
          last_el = last_el->tail;
        }
    }

  /* Static link */
  if (!is_lib_func)
    {
      tra_level *current     = lv;
      tree_exp  *static_link = frm_static_link_exp (tree_new_temp (frm_fp ()));
    /* Finding static link iteratively */
    if (funclv->parent != current)
      {
        while (current)
          {
            static_link = tree_new_mem (static_link);
            if (funclv->parent == current->parent)
              {
                break;
              }
            current = current->parent;
          }
      }
    el = list_new_list (static_link, el);
  }
  return trans_exp (tree_new_call (tree_new_name (name), el));
}

/**
 * Translates a nil expression into intermediate code.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_nil_exp (void)
{
  return trans_exp (tree_new_const (0));
}


/**
 * Translates a for loop expression into intermediate code.
 *
 * @param low_ptr  Start vaule.
 * @param hi_ptr   End value.
 * @param body_ptr Body of loop.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_for_exp (tra_access *i,
             tra_level  *lv,
             tra_exp    *explo,
             tra_exp    *exphi,
             tra_exp    *body,
             temp_label *breaklbl)
{
  temp_label *test      = temp_new_label ();
  temp_label *loopstart = temp_new_label ();
  temp_label *done      = breaklbl;
  temp_temp  *limit     = temp_new_temp();
  tree_exp   *vari      = conv_exp (tra_simple_var (i, lv));

  tree_stm *s = tree_new_seq(tree_new_move (vari, conv_exp (explo)),
              tree_new_seq (tree_new_label (test),
                tree_new_seq (tree_new_move (tree_new_temp (limit), conv_exp (exphi)),
                  tree_new_seq (tree_new_cjump (TREE_LE, vari, tree_new_temp (limit), loopstart, done),
                    tree_new_seq (tree_new_label (loopstart),
                      tree_new_seq (conv_no_res_exp (body),
                        tree_new_seq (tree_new_move (vari, tree_new_bin_op (TREE_PLUS, vari, tree_new_const (1))),
                          tree_new_seq (tree_new_jump (tree_new_name (test), list_new_list (test, NULL)),
                            tree_new_label (done)))))))));
  return trans_no_res_exp (s);
}

/**
 * Translates a string literal expression into intermediate code.
 *
 * @param str String literal.
 *
 * @return Intermediate code.
 */
tra_exp *
tra_str_exp (char *str)
{
  temp_label *str_label = temp_new_label ();

  frag_list_add (frm_new_str_frag (str_label, str));

  return trans_exp (tree_new_name (str_label));
}

tra_exp *
tra_let_exp (tra_exp_list *list,
             tra_exp      *body)
{
  tree_exp *exp = conv_exp (body);
  for (; list; list = list->tail)
    {
      exp = tree_new_eseq (conv_no_res_exp (list->head), exp);
    }
  return trans_exp (exp);
  /*
  return trans_exp (tree_new_eseq (conv_no_res_exp (tra_seq_exp (list)),
                                   conv_exp (body)));
  */
}

/**
 * Translates a variable declaration into intermediate code representation.
 *
 * @access_ptr Access of the variable.
 * @init_ptr   The init value of the variable.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_var_dec (tra_access *access_ptr,
             tra_exp    *init_ptr)
{
  tree_exp *mem = frm_exp (access_ptr->access,
                           tree_new_temp (frm_fp ()));
  return trans_no_res_exp (tree_new_move (mem, conv_exp (init_ptr)));
}

/**
 * Translates a type declaration into intermediate code representation.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_type_dec (void)
{
  return trans_exp (tree_new_const (0));
}

/**
 * Adds a function declaration fragment to global fragment list.
 *
 * @body_ptr  Function body.
 * @frame_ptr Function frame.
 *
 * @return Intermediate code representation.
 */
void
tra_add_func_frag (tra_exp   *body_ptr,
                   tra_level *level_ptr)
{
  tree_exp *temp = tree_new_temp (frm_fp ());
  frm_frag *frag = frm_new_proc_frag (tree_new_move (temp,
                                                     conv_exp (body_ptr)),
                                      level_ptr->frame);

  frag_list_add (frag);
}

/**
 * Translates a function declaration into intermediate code representation.
 *
 * @return Intermediate code representation.
 */
tra_exp *
tra_func_dec (void)
{
  return trans_exp (tree_new_const (0));
}

/*
  Converts a conditional exp, no_result exp or expression,
  into a tree expression.
*/
static tree_exp *
conv_exp (tra_exp * exp_ptr)
{
  switch (exp_ptr->kind)
    {
    case TRA_EXP:
      return exp_ptr->u.exp;

    case TRA_CONDITIONAL:
      {
        temp_temp  *reg    = temp_new_temp ();
        temp_label *truee  = temp_new_label ();
        temp_label *falsee = temp_new_label ();

        do_patch (exp_ptr->u.conditional->trues,  truee);
        do_patch (exp_ptr->u.conditional->falses, falsee);

        tree_stm *res_true  = tree_new_move (tree_new_temp (reg),
                                               tree_new_const (1));
        tree_stm *res_false = tree_new_move (tree_new_temp (reg),
                                             tree_new_const (0));
        tree_stm *l_false   = tree_new_label (falsee);
        tree_stm *l_true    = tree_new_label (truee);


        tree_exp *list   = tree_new_eseq (res_true, NULL);

        /*
          Create intermediate representation for moving a
          1 (true) or 0 (false) into a register.
         */
        list->u.eseq.exp = tree_new_eseq (exp_ptr->u.conditional->stm, NULL);
        list = list->u.eseq.exp;

        list->u.eseq.exp = tree_new_eseq (l_false, NULL);
        list = list->u.eseq.exp;

        list->u.eseq.exp = tree_new_eseq (res_false, NULL);
        list = list->u.eseq.exp;

        list->u.eseq.exp = tree_new_eseq (l_true, tree_new_temp (reg));

        return list;
      }
    case TRA_NO_RES:
      return tree_new_eseq (exp_ptr->u.no_res, tree_new_const (0));
  }
  assert (0); /* can’t get here */
}

/*
  Converts a conditional exp, no_result exp or expression,
  into a tree statement.
*/
static tree_stm *
conv_no_res_exp (tra_exp *exp_ptr)
{
  switch (exp_ptr->kind)
    {
    case TRA_EXP:
      return tree_new_exp (exp_ptr->u.exp);

    case TRA_CONDITIONAL:
      {
        temp_label *label = temp_new_label ();
        do_patch (exp_ptr->u.conditional->trues, label);
        do_patch (exp_ptr->u.conditional->falses, label);

        tree_stm *stm = tree_new_seq (exp_ptr->u.conditional->stm,
                                      tree_new_label (label));
        return stm;
      }

    case TRA_NO_RES:
      return exp_ptr->u.no_res;
    }

  assert (0); /* can’t get here */
  return NULL;
}

/*
  Converts a conditional exp, no_result exp or expression,
  into a conditional.
*/
static condit_exp *
conv_conditional_exp (tra_exp *exp_ptr)
{
  switch (exp_ptr->kind)
    {
    case TRA_EXP:
      {
        tree_stm *stm      = tree_new_cjump (TREE_EQ,
                                             exp_ptr->u.exp,
                                             tree_new_const (0),
                                             NULL,
                                             NULL);
        patch_list *trues  = list_new_list (&(stm->u.cjump.truee), NULL);
        patch_list *falses = list_new_list (&(stm->u.cjump.falsee), NULL);

        return new_condit (stm, trues, falses);
      }

    case TRA_CONDITIONAL:
      return exp_ptr->u.conditional;

    case TRA_NO_RES:
      assert (0); /* This case should never happen */
      return NULL;
  }

  assert (0);
  return NULL;
}

/*
  Fill placeholder in patchlist with the given label.
 */
static void
do_patch (patch_list *t_list_ptr,
          temp_label *label_ptr)
{
  for (; t_list_ptr; t_list_ptr = t_list_ptr->tail)
    *(temp_label**)t_list_ptr->head = label_ptr;
}

/*
  Appends second patch_list to first_patch list.
 */
static patch_list *
join_patch (patch_list *first_ptr,
            patch_list *second_ptr)
{
  if (first_ptr == NULL)
    return second_ptr;

  for (; first_ptr->tail; first_ptr = first_ptr->tail); /* go to end of list */

  first_ptr->tail = second_ptr;
  return first_ptr;
}

/* Gets the memory offset for a variable that is declared in another scope */
static tree_exp *
get_offset (tra_level *used_level_ptr,
            tra_level *declared_level_ptr,
            temp_temp *frame_pointer_ptr)
{
  tra_level  *outermost_level = tra_outermost_level ();
  tra_access *access          = used_level_ptr->formals->head;
  tree_exp   *mem             = frm_exp (access->access,
                                         tree_new_temp (frame_pointer_ptr));
  mem = tree_new_mem (mem);

  while (used_level_ptr != declared_level_ptr
         && used_level_ptr != outermost_level)
    {
      used_level_ptr = used_level_ptr->parent;
      access         = used_level_ptr->formals->head;

      mem = tree_new_mem (frm_exp (access->access, mem));
    }

  return mem;
}

/*
  Creates or adds a element to the private global fragment list.
 */
static void
frag_list_add (frm_frag *frag)
{
  if (frag_list == NULL)
    sfrag_list = frag_list = list_new_list (frag, NULL);
  else
    frag_list = frag_list->tail = list_new_list (frag, NULL);
}

/**
 * Returns the private global fragment list.
 *
 * @return Fragment list.
 */
frm_frag_list *
tra_get_frag_list (void)
{
  return sfrag_list;
}

static condit_exp *
new_condit (tree_stm   *stm_ptr,
            patch_list *trues_ptr,
            patch_list *falses_ptr)
{
  condit_exp *exp = new (sizeof (*exp));

  exp->stm    = stm_ptr;
  exp->trues  = trues_ptr;
  exp->falses = falses_ptr;

  return exp;
}

static tra_exp *
trans_exp (tree_exp *exp_ptr)
{
  tra_exp *exp = new (sizeof (*exp));

  exp->kind = TRA_EXP;

  exp->u.exp = exp_ptr;

  return exp;
}

static tra_exp *
trans_no_res_exp (tree_stm *no_res_ptr)
{
  tra_exp *exp = new (sizeof (*exp));

  exp->kind = TRA_NO_RES;

  exp->u.no_res = no_res_ptr;

  return exp;
}

static tra_exp *
trans_conditional_exp (patch_list *trues_ptr,
                       patch_list *falses_ptr,
                       tree_stm   *stm_ptr)
{
  tra_exp *exp = new (sizeof (*exp));

  exp->kind = TRA_CONDITIONAL;

  exp->u.conditional = new_condit (stm_ptr, trues_ptr, falses_ptr);

  return exp;
}
/*
static patch_list *
new_patch_list (temp_label *head_ptr,
                patch_list *tail_ptr)
{
  patch_list *list = new (sizeof (*list));

  list->head = head_ptr;
  list->tail = tail_ptr;

  return list;
}
*/
static tra_access_list *
new_formals (tra_level *level)
{
  tra_access_list *tra_access_list = NULL, *stra_access_list = NULL;
  frm_access_list *frm_access_list = frm_formals (level->frame);

  for (; frm_access_list != NULL; frm_access_list = frm_access_list->tail)
    {
      frm_access *frm_access = frm_access_list->head;
      tra_access *tra_access = new (sizeof (*tra_access));
      tra_access->access = frm_access;
      tra_access->level  = level;

      if (tra_access_list == NULL)
        stra_access_list = tra_access_list = list_new_list (tra_access,
                                                            NULL);
      else
        tra_access_list = tra_access_list->tail = list_new_list (tra_access,
                                                                 NULL);
    }
  return stra_access_list;
}
