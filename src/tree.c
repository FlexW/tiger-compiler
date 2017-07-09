/**
 * @file tree.c
 * Definitions for intermediate representation (IR) trees.
 */

#include <stdio.h>
#include <assert.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"


tree_stm *
tree_new_seq (tree_stm *left_ptr,
              tree_stm *right_ptr)
{
  tree_stm *p = (tree_stm*)new (sizeof *p);

  p->kind        = TREE_SEQ;
  p->u.seq.left  = left_ptr;
  p->u.seq.right = right_ptr;

  return p;
}

tree_stm *
tree_new_label (temp_label *label_ptr)
{
  tree_stm *p = (tree_stm*)new (sizeof *p);

  p->kind    = TREE_LABEL;
  p->u.label = label_ptr;

  return p;
}

tree_stm *
tree_new_jump (tree_exp        *exp_ptr,
               temp_label_list *labels_ptr)
{
  tree_stm *p = (tree_stm*)new (sizeof *p);

  p->kind        = TREE_JUMP;
  p->u.jmp.exp   = exp_ptr;
  p->u.jmp.jumps = labels_ptr;

 return p;
}

tree_stm *
tree_new_cjump (tree_rel_op  op,
                tree_exp    *left_ptr,
                tree_exp    *right_ptr,
                temp_label  *truee_ptr,
                temp_label  *falsee_ptr)
{
  tree_stm *p = (tree_stm*)new (sizeof *p);

  p->kind           = TREE_CJUMP;
  p->u.cjump.op     = op;
  p->u.cjump.left   = left_ptr;
  p->u.cjump.right  = right_ptr;
  p->u.cjump.truee  = truee_ptr;
  p->u.cjump.falsee = falsee_ptr;

  return p;
}

tree_stm *
tree_new_move (tree_exp *dst_ptr,
               tree_exp *src_ptr)
{
  tree_stm *p = (tree_stm*)new (sizeof *p);

  p->kind       = TREE_MOVE;
  p->u.move.dst = dst_ptr;
  p->u.move.src = src_ptr;

  return p;
}

tree_stm *
tree_new_exp (tree_exp *exp_ptr)
{
  tree_stm *p = (tree_stm*)new (sizeof *p);

  p->kind  = TREE_EXP;
  p->u.exp = exp_ptr;

  return p;
}

tree_exp *
tree_new_bin_op (tree_bin_op  op,
                 tree_exp    *left_ptr,
                 tree_exp    *right_ptr)
{
  tree_exp *p = (tree_exp*)new (sizeof *p);

  p->kind           = TREE_BINOP;
  p->u.bin_op.op    = op;
  p->u.bin_op.left  = left_ptr;
  p->u.bin_op.right = right_ptr;

  return p;
}

tree_exp *
tree_new_mem (tree_exp *exp_ptr)
{
  tree_exp *p = (tree_exp*)new (sizeof *p);

  p->kind  = TREE_MEM;
  p->u.mem = exp_ptr;

 return p;
}

tree_exp *
tree_new_temp (temp_temp *temp_ptr)
{
  tree_exp *p = (tree_exp*)new (sizeof *p);

  p->kind   = TREE_TEMP;
  p->u.temp = temp_ptr;

  return p;
}

tree_exp *
tree_new_eseq (tree_stm *stm_ptr,
               tree_exp *exp_ptr)
{
  tree_exp *p = (tree_exp*)new (sizeof *p);

  p->kind     =TREE_ESEQ;
  p->u.eseq.stm = stm_ptr;
  p->u.eseq.exp = exp_ptr;

  return p;
}

tree_exp *
tree_new_name (temp_label *name_ptr)
{
  tree_exp *p = (tree_exp*)new (sizeof *p);

  p->kind   = TREE_NAME;
  p->u.name = name_ptr;

  return p;
}

tree_exp *
tree_new_const (int constt)
{
  tree_exp *p = (tree_exp*)new (sizeof *p);

 p->kind     = TREE_CONST;
 p->u.constt = constt;

 return p;
}

tree_exp *
tree_new_call (tree_exp      *fun_ptr,
               tree_exp_list *args_ptr)
{
  tree_exp *p = (tree_exp*) new (sizeof *p);

  p->kind        = TREE_CALL;
  p->u.call.fun  = fun_ptr;
  p->u.call.args = args_ptr;

  return p;
}

tree_rel_op
tree_not_rel (tree_rel_op rel_op)
{
 switch(rel_op)
   {
   case TREE_EQ:
       return TREE_NEQ;

   case TREE_NEQ:
      return TREE_EQ;

   case TREE_LT:
      return TREE_GE;

   case TREE_GE:
      return TREE_LT;

   case TREE_GT:
      return TREE_LE;

   case TREE_LE:
      return TREE_GT;

   case TREE_ULT:
      return TREE_UGE;

   case TREE_UGE:
      return TREE_ULT;

   case TREE_ULE:
      return TREE_UGT ;

   case TREE_UGT:
      return TREE_ULE;
  }
 assert(0);
 return 0;
}

tree_rel_op
tree_commute (tree_rel_op rel_op)
{
  switch(rel_op) {

  case TREE_EQ:
    return TREE_EQ;

  case TREE_NEQ:
    return TREE_NEQ;

  case TREE_LT:
    return TREE_GT;

  case TREE_GE:
    return TREE_LE;

  case TREE_GT:
    return TREE_LT ;

  case TREE_LE:
    return TREE_GE;

  case TREE_ULT:
    return TREE_UGT;

  case TREE_UGE:
    return TREE_ULE;

  case TREE_ULE:
    return TREE_UGE ;

  case TREE_UGT:
    return TREE_ULT;
  }
  assert(0);
  return 0;
}

tree_stm_list *
tree_new_stm_list (tree_stm *head,
                   tree_stm_list *tail)
{
  tree_stm_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

tree_exp_list *
tree_new_exp_list (tree_exp *head,
                   tree_exp_list *tail)
{
  tree_exp_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}
