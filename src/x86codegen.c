/**
 * @file x86codegen.c
 * Code generation for x86 assembly after Maximal Munch.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/absyn.h"
#include "include/temp.h"
#include "include/errormsg.h"
#include "include/tree.h"
#include "include/assem.h"
#include "include/frame.h"
#include "include/codegen.h"
#include "include/table.h"


static assem_instr_list *global_instr_list      = NULL;
static assem_instr_list *global_instr_list_last = NULL;

static bool last_is_label = false;  // reserved for "nop"

static temp_temp *      munch_exp            (tree_exp *e);

static void             munch_stm            (tree_stm *s);

static temp_temp_list * munch_args           (int            i,
                                              tree_exp_list *args);

static void             munch_caller_save    (void);

static void             munch_caller_restore (temp_temp_list *tl);


static void
emit (assem_instr *instr)
{
  last_is_label = (instr->kind == I_LABEL);
  if (global_instr_list_last != NULL)
    {
      global_instr_list_last =
        global_instr_list_last->tail = assem_new_instr_list (instr, NULL);
    }
  else
    {
      global_instr_list_last = global_instr_list = assem_new_instr_list (instr,
                                                                         NULL);
    }
}

assem_instr_list *
codegen (frm_frame     *f,
         tree_stm_list *stm_list)
{
  assem_instr_list *list;
  tree_stm_list    *sl;

  /* miscellaneous initializations as necessary */

  for (sl = stm_list; sl; sl = sl->tail)
    {
      munch_stm (sl->head);
    }
  if (global_instr_list_last && global_instr_list_last->head->kind == I_LABEL)
    {
      emit(assem_new_oper ("nop\n", NULL, NULL, NULL));
    }

  list = global_instr_list;
  global_instr_list = global_instr_list_last = NULL;
  return list;
}

static temp_temp *
generate_mem (tree_exp *e,
              char     *inst,
              char     *inst2)
{
  tree_exp *mem = e->u.mem;
  if (mem->kind == TREE_BINOP)
    {
      if (mem->u.bin_op.op == TREE_PLUS
            && mem->u.bin_op.right->kind == TREE_CONST)
        {
          /* MEM(BINOP(PLUS,e1,CONST(i))) */
          tree_exp *e1 = mem->u.bin_op.left;
          int i = mem->u.bin_op.right->u.constt;
          temp_temp *r = temp_new_temp();
          sprintf (inst, "movl %d(`s0), `d0\n", i);
          emit(assem_new_oper(inst,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (munch_exp (e1), NULL),
                              NULL));
          return r;
        }
      else if (mem->u.bin_op.op == TREE_PLUS
            && mem->u.bin_op.left->kind == TREE_CONST)
        {
          /* MEM(BINOP(PLUS,CONST(i),e1)) */
          tree_exp *e1 = mem->u.bin_op.right;
          int i = mem->u.bin_op.left->u.constt;
          temp_temp *r = temp_new_temp();
          sprintf (inst, "movl %d(`s0), `d0\n", i);
          emit(assem_new_oper (inst,
                               temp_new_temp_list (r, NULL),
                               temp_new_temp_list (munch_exp (e1), NULL),
                               NULL));
          return r;
        }
      else
        {
          /* MEM(e1) */
          tree_exp *e1 = mem;
          temp_temp *r = temp_new_temp();
          sprintf (inst, "movl (`s0), `d0\n");
          emit(assem_new_oper (inst,
                               temp_new_temp_list (r, NULL),
                               temp_new_temp_list (munch_exp(e1), NULL),
                               NULL));
          return r;
        }
      }
  else if (mem->kind == TREE_CONST)
    {
        /* MEM(CONST(i)) */
        int i = mem->u.constt;
        temp_temp *r = temp_new_temp();
        sprintf (inst, "movl %d, `d0\n", i);
        emit(assem_new_oper (inst,
                             temp_new_temp_list (r, NULL),
                             NULL,
                             NULL));
        return r;
      }
  else
    {
        /* MEM(e1) */
      tree_exp *e1 = mem;
      temp_temp *r = temp_new_temp ();
      sprintf (inst, "movl (`s0), `d0\n");
      emit(assem_new_oper(inst,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (munch_exp(e1), NULL),
                          NULL));
      return r;
    }
}

temp_temp *
generate_binop (tree_exp *e,
                char     *inst,
                char     *inst2)
{
  if (e->u.bin_op.op == TREE_PLUS
         && e->u.bin_op.right->kind == TREE_CONST)
    {
      /* BINOP(PLUS,e1,CONST(i)) */
      tree_exp *e1 = e->u.bin_op.left;
      int i = e->u.bin_op.right->u.constt;
      temp_temp *r = temp_new_temp();
      sprintf(inst, "movl `s0, `d0\n");
      emit(assem_new_move (inst,
                           temp_new_temp_list (r, NULL),
                           temp_new_temp_list (munch_exp(e1), NULL)));
      sprintf(inst2, "addl $%d, `d0\n", i);
      emit(assem_new_oper(inst2,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r, NULL),
                          NULL));
      return r;
    }
  else if (e->u.bin_op.op == TREE_PLUS
           && e->u.bin_op.left->kind == TREE_CONST)
    {
      /* BINOP(PLUS,CONST(i),e1) */
      tree_exp *e1 = e->u.bin_op.right;
      int i = e->u.bin_op.left->u.constt;
      temp_temp *r = temp_new_temp();
      sprintf(inst, "movl `s0, `d0\n");
      emit(assem_new_move(inst,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (munch_exp(e1), NULL)));
      sprintf(inst2, "addl $%d, `d0\n", i);
      emit(assem_new_oper(inst2,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r, NULL),
                          NULL));
      return r;
    }
  else if (e->u.bin_op.op == TREE_MINUS
           && e->u.bin_op.right->kind == TREE_CONST)
    {
      /* BINOP(MINUS,e1,CONST(i)) */
      tree_exp *e1 = e->u.bin_op.left;
      int i = e->u.bin_op.right->u.constt;
      temp_temp *r = temp_new_temp();
      sprintf(inst, "movl `s0, `d0\n");
      emit(assem_new_move(inst,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (munch_exp(e1), NULL)));
      sprintf(inst2, "subl $%d, `d0\n", i);
      emit(assem_new_oper(inst2,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r, NULL),
                          NULL));
      return r;
    }
  else if (e->u.bin_op.op == TREE_PLUS)
    {
      /* BINOP(PLUS,e1,e2) */
      tree_exp *e1 = e->u.bin_op.left, *e2 = e->u.bin_op.right;
      temp_temp *r = temp_new_temp();
      temp_temp *r1 = munch_exp(e1);
      temp_temp *r2 = munch_exp(e2);
      sprintf(inst, "movl `s0, `d0\n");
      emit(assem_new_move(inst,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r1, NULL)));
      sprintf(inst2, "addl `s0, `d0\n");
      emit(assem_new_oper(inst2,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r2, temp_new_temp_list (r, NULL)),
                          NULL));
      return r;
    }
  else if (e->u.bin_op.op == TREE_MINUS)
    {
        /* BINOP(MINUS,e1,e2) */
      tree_exp *e1 = e->u.bin_op.left, *e2 = e->u.bin_op.right;
      temp_temp *r = temp_new_temp();
      temp_temp *r1 = munch_exp(e1);
      temp_temp *r2 = munch_exp(e2);
      sprintf(inst, "movl `s0, `d0\n");
      emit(assem_new_move(inst,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r1, NULL)));
      sprintf(inst2, "subl `s0, `d0\n");
      emit(assem_new_oper(inst2,
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (r2, temp_new_temp_list (r, NULL)),
                          NULL));
      return r;
    }
  else if (e->u.bin_op.op == TREE_TIMES)
    {
      if (0 && e->u.bin_op.right->kind == TREE_CONST)
        {
          /* BINOP(MUL,e1,i) */
          tree_exp *e1 = e->u.bin_op.left;
          temp_temp *r = temp_new_temp();
          temp_temp *r1 = munch_exp(e1);
          int i = e->u.bin_op.right->u.constt;
          sprintf(inst, "movl `s0, `d0\n");
          emit(assem_new_move(inst,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (r1, NULL)));
          sprintf(inst2, "imul $%d, `d0\n", i);
          emit(assem_new_oper(inst2,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (r, NULL),
                              NULL));
          return r;
        }
      else if (0 && e->u.bin_op.left->kind == TREE_CONST)
        {
          /* BINOP(MUL,i, e1) */
          tree_exp *e1 = e->u.bin_op.right;
          temp_temp *r = temp_new_temp();
          temp_temp *r1 = munch_exp(e1);
          int i = e->u.bin_op.left->u.constt;
          sprintf(inst, "movl `s0, `d0\n");
          emit(assem_new_move(inst,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (r1, NULL)));
          sprintf(inst2, "imul $%d, `d0\n", i);
          emit(assem_new_oper(inst2,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (r, NULL),
                              NULL));
          return r;
        }
      else
        {
          /* BINOP(MUL,e1,e2) */
          tree_exp *e1 = e->u.bin_op.left, *e2 = e->u.bin_op.right;
          temp_temp *r = temp_new_temp();
          temp_temp *r1 = munch_exp(e1);
          temp_temp *r2 = munch_exp(e2);
          sprintf(inst, "movl `s0, `d0\n");
          emit(assem_new_move(inst,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (r1, NULL)));
          sprintf(inst2, "imul `s0, `d0\n");
          emit(assem_new_oper(inst2,
                              temp_new_temp_list (r, NULL),
                              temp_new_temp_list (r2,
                                                  temp_new_temp_list (r, NULL)),
                              NULL));
          return r;
        }
    }
  else if (e->u.bin_op.op == TREE_DIVIDE)
    {
      /* BINOP(DIV,e1,e2) */
      tree_exp *e1 = e->u.bin_op.left, *e2 = e->u.bin_op.right;
      temp_temp *r = temp_new_temp();
      temp_temp *r1 = munch_exp(e1);
      temp_temp *r2 = munch_exp(e2);
      emit(assem_new_move("movl `s0, `d0\n",
                          temp_new_temp_list (frm_eax(), NULL),
                          temp_new_temp_list (r1, NULL)));
      emit(assem_new_oper("movl $0, `d0\n",
                          temp_new_temp_list (frm_edx(), NULL),
                          NULL,
                          NULL));
      emit(assem_new_oper("divl `s0\n",
                          temp_new_temp_list (frm_eax(),
                                        temp_new_temp_list  (frm_edx(),
                                                             NULL)),
                          temp_new_temp_list (r2,
                                        temp_new_temp_list (frm_edx(),
                                                      temp_new_temp_list (frm_eax(),
                                                                          NULL))),
                          NULL));
      emit(assem_new_move("movl `s0, `d0\n",
                          temp_new_temp_list (r, NULL),
                          temp_new_temp_list (frm_eax(), NULL)));
      return r;
    }
  else
    {
      assert(0);
    }
}

temp_temp *
generate_const (tree_exp *e,
                char     *inst,
                char     *inst2)
{
  /* CONST(i) */
  int i = e->u.constt;
  temp_temp *r = temp_new_temp();
  sprintf(inst, "movl $%d, `d0\n", i);
  emit(assem_new_oper(inst, temp_new_temp_list (r, NULL), NULL, NULL));

  return r;
}

temp_temp *
generate_temp (tree_exp *e,
                char     *inst,
                char     *inst2)
{
  return e->u.temp;
}

temp_temp *
generate_name (tree_exp *e,
               char     *inst,
               char     *inst2)
{
  /* NAME(lab) */
  temp_label *lab = e->u.name;
  temp_temp *r = temp_new_temp();
  sprintf(inst, "movl $%s, `d0\n", temp_label_str (lab));
  emit(assem_new_oper(inst, temp_new_temp_list (r, NULL), NULL, NULL));

  return r;
}

temp_temp *
generate_call (tree_exp *e,
               char     *inst,
               char     *inst2)
{
  /* CALL(NAME(lab),args) */
  munch_caller_save();
  temp_label *lab = e->u.call.fun->u.name;
  tree_exp_list *args = e->u.call.args;
  temp_temp *t = temp_new_temp();
  temp_temp_list *l = munch_args(0, args);
  temp_temp_list *calldefs = frm_caller_saves();
  sprintf(inst, "call %s\n", temp_label_str(lab));
  emit(assem_new_oper(inst,
                      temp_new_temp_list (frm_rv(), calldefs),
                      l,
                      NULL));
  munch_caller_restore(l);
  sprintf(inst2, "movl `s0, `d0\n");
  emit(assem_new_move(inst2,
                      temp_new_temp_list (t, NULL),
                      temp_new_temp_list (frm_rv (), NULL)));
  return t;
}

static temp_temp *
munch_exp(tree_exp *e)
{
  char *inst = new (sizeof(char) * 120);
  char *inst2 = new (sizeof(char) * 120);

  switch (e->kind)
    {
    case TREE_MEM:
      return generate_mem (e, inst, inst2);

    case TREE_BINOP:
      return generate_binop (e, inst, inst2);

    case TREE_CONST:
      return generate_const (e, inst, inst2);

    case TREE_TEMP:
      return generate_temp (e, inst, inst2);

    case TREE_NAME:
      return generate_name (e, inst, inst2);

    case TREE_CALL:
      return generate_call (e, inst, inst2);

    default:
      assert(0);
  }
}

void
generate_move (tree_stm *s,
               char     *inst,
               char     *inst2,
               char     *inst3)
{
   tree_exp * dst = s->u.move.dst, *src = s->u.move.src;
   if (dst->kind == TREE_MEM)
     {
       if (dst->u.mem->kind == TREE_BINOP
           && dst->u.mem->u.bin_op.op == TREE_PLUS
           && dst->u.mem->u.bin_op.right->kind == TREE_CONST)
         {
           if (src->kind == TREE_CONST)
             {
               /* MOVE(MEM(BINOP(PLUS,e1,CONST(i))),CONST(j)) */
               tree_exp *e1 = dst->u.mem->u.bin_op.left, *e2 = src;
               int i = dst->u.mem->u.bin_op.right->u.constt;
               int j = src->u.constt;
               sprintf(inst, "movl $%d, %d(`s0)\n", j, i);
               emit(assem_new_oper (inst,
                                    NULL,
                                    temp_new_temp_list (munch_exp(e1), NULL),
                                    NULL));
             }
           else
             {
               /* MOVE(MEM(BINOP(PLUS,e1,CONST(i))),e2) */
               tree_exp * e1 = dst->u.mem->u.bin_op.left, *e2 = src;
               int i = dst->u.mem->u.bin_op.right->u.constt;
               sprintf(inst, "movl `s1, %d(`s0)\n", i);
               emit(assem_new_oper (inst,
                                    NULL,
                                    temp_new_temp_list (munch_exp(e1),
                                                   temp_new_temp_list (munch_exp(e2),
                                                                       NULL)),
                                    NULL));
             }
         }
       else if (dst->u.mem->kind == TREE_BINOP
                && dst->u.mem->u.bin_op.op == TREE_PLUS
                && dst->u.mem->u.bin_op.left->kind == TREE_CONST)
         {
           if (src->kind == TREE_CONST)
             {
               /* MOVE(MEM(BINOP(PLUS,CONST(i),e1)),CONST(j)) */
               tree_exp * e1 = dst->u.mem->u.bin_op.right, *e2 = src;
               int i = dst->u.mem->u.bin_op.left->u.constt;
               int j = src->u.constt;
               sprintf(inst, "movl $%d, %d(`s0)\n", j, i);
               emit(assem_new_oper (inst,
                                    NULL,
                                    temp_new_temp_list (munch_exp(e1), NULL),
                                    NULL));
             }
           else
             {
               /* MOVE(MEM(BINOP(PLUS,CONST(i),e1)),e2) */
               tree_exp * e1 = dst->u.mem->u.bin_op.right, *e2 = src;
               int i = dst->u.mem->u.bin_op.left->u.constt;
               sprintf(inst, "movl `s1, %d(`s0)\n", i);
               emit(assem_new_oper (inst,
                                    NULL,
                                    temp_new_temp_list (munch_exp (e1),
                                                   temp_new_temp_list (munch_exp (e2),
                                                                  NULL)),
                                    NULL));
             }
         }
       else if (src->kind == TREE_MEM)
         {
          /* MOVE(MEM(e1), MEM(e2)) */
          tree_exp * e1 = dst->u.mem, *e2 = src->u.mem;
          temp_temp * r = temp_new_temp();
          sprintf(inst, "movl (`s0), `d0\n");
          emit(assem_new_oper (inst,
                               temp_new_temp_list (r, NULL),
                               temp_new_temp_list (munch_exp (e2), NULL),
                               NULL));
          sprintf(inst2, "movl `s0, (`s1)\n");
          emit(assem_new_oper (inst2,
                               NULL,
                               temp_new_temp_list (r, temp_new_temp_list (munch_exp (e1),
                                                                          NULL)),
                               NULL));
         }
       else if (dst->u.mem->kind == TREE_CONST)
         {
           /* MOVE(MEM(CONST(i)), e2) */
           tree_exp * e2 = src;
           int i = dst->u.mem->u.constt;
           sprintf(inst, "movl `s0, %d\n", i);
           emit(assem_new_oper (inst,
                                NULL,
                                temp_new_temp_list (munch_exp (e2), NULL),
                                NULL));
        }
       else
         {
           /* MOVE(MEM(e1), e2) */
           tree_exp * e1 = dst->u.mem, *e2 = src;
           sprintf(inst, "movl `s1, (`s0)\n");
           emit(assem_new_oper (inst,
                                NULL,
                                temp_new_temp_list (munch_exp (e1),
                                               temp_new_temp_list (munch_exp (e2),
                                                              NULL)),
                                NULL));
         }
     }
   else if (dst->kind == TREE_TEMP)
     {
       if (src->kind == TREE_CALL)
         {
           if (src->u.call.fun->kind == TREE_NAME)
             {
               /* MOVE(TEMP(t),CALL(NAME(lab),args)) */
               munch_caller_save();
               temp_label *lab = src->u.call.fun->u.name;
               tree_exp_list *args = src->u.call.args;
               temp_temp * t = dst->u.temp;
               temp_temp_list *l = munch_args(0, args);
               temp_temp_list *calldefs = frm_caller_saves();
               sprintf(inst, "call %s\n", temp_label_str(lab));
               emit(assem_new_oper (inst,
                                    temp_new_temp_list (frm_rv(), calldefs),
                                    l,
                                    NULL));
               munch_caller_restore (l);
               sprintf(inst2, "movl `s0, `d0\n");
               emit(assem_new_move(inst2,
                                   temp_new_temp_list (t, NULL),
                                   temp_new_temp_list (frm_rv(), NULL)));
             }
           else
             {
               /* MOVE(TEMP(t),CALL(e,args)) */
                 assert(0);
             }
         }
       else
         {
           /* MOVE(TEMP(i),e2) */
           tree_exp * e2 = src;
           temp_temp * i = dst->u.temp;
           sprintf(inst, "movl `s0, `d0\n");
           emit(assem_new_move(inst,
                               temp_new_temp_list (i, NULL),
                               temp_new_temp_list (munch_exp (e2), NULL)));
         }
     }
   else
     {
       assert(0);
     }
}

void
generate_label (tree_stm *s,
                char     *inst,
                char     *inst2,
                char     *inst3)
{
  /* LABEL(lab) */

  // Avoid two labels in same palce
  if (last_is_label)
    emit(assem_new_oper("nop\n", NULL, NULL, NULL));

  temp_label *lab = s->u.label;

  sprintf(inst, "%s:\n", temp_label_str(lab));
  emit(assem_new_label (inst, lab));
}

void
generate_exp (tree_stm *s,
              char     *inst,
              char     *inst2,
              char     *inst3)
{
  if (s->u.exp->kind == TREE_CALL)
    {
      tree_exp *call = s->u.exp;
      if (call->u.call.fun->kind == TREE_NAME)
        {
          /* EXP(CALL(NAME(lab),args)) */
          munch_caller_save();
          temp_label *lab = call->u.call.fun->u.name;
          tree_exp_list *args = call->u.call.args;
          temp_temp_list *l = munch_args(0, args);
          temp_temp_list *calldefs = frm_caller_saves();
          sprintf(inst, "call %s\n", temp_label_str (lab));
          emit(assem_new_oper(inst, calldefs, l, NULL));
          munch_caller_restore(l);
        }
      else
        {
          assert(0);
        }
    }
  else
    {
      /* EXP(e) */
      munch_exp(s->u.exp);
    }
}

void
generate_jump (tree_stm *s,
               char     *inst,
               char     *inst2,
               char     *inst3)
{
  if (s->u.jmp.exp->kind == TREE_NAME)
    {
      /* JUMP(NAME(lab)) */
      temp_label *lab = s->u.jmp.exp->u.name;
      temp_label_list *jumps = s->u.jmp.jumps;
      sprintf(inst, "jmp `j0\n");
      emit(assem_new_oper(inst, NULL, NULL, assem_new_targets(jumps)));
    }
  else
    {
      /* JUMP(e) */
      tree_exp *e = s->u.jmp.exp;
      temp_label_list *jumps = s->u.jmp.jumps;
      sprintf(inst, "jmp *`s0\n");
      emit(assem_new_oper(inst,
                          NULL,
                          temp_new_temp_list (munch_exp(e), NULL),
                          assem_new_targets (jumps)));
    }
}

void
generate_cjump (tree_stm *s,
                char     *inst,
                char     *inst2,
                char     *inst3)
{
  /* CJUMP(op,e1,e2,jt,jf) */
  tree_rel_op op = s->u.cjump.op;
  tree_exp *e1 = s->u.cjump.left;
  tree_exp *e2 = s->u.cjump.right;
  temp_temp *r1 = munch_exp(e1);
  temp_temp *r2 = munch_exp(e2);
  temp_temp *r3 = temp_new_temp();
  temp_temp *r4 = temp_new_temp();
  temp_label *jt = s->u.cjump.truee;
  temp_label *jf = s->u.cjump.falsee;
  emit(assem_new_move("movl `s0, `d0\n",
                      temp_new_temp_list (r3, NULL),
                      temp_new_temp_list (r1, NULL)));
  emit(assem_new_move("movl `s0, `d0\n",
                      temp_new_temp_list (r4, NULL),
                      temp_new_temp_list (r2, NULL)));
  sprintf(inst, "cmp `s1, `s0\n");
  emit(assem_new_oper(inst,
                      NULL,
                      temp_new_temp_list (r3, temp_new_temp_list (r4, NULL)),
                      NULL));

  char* opcode = "";
  switch (op)
    {
    case TREE_EQ:  opcode = "je";  break;
    case TREE_NEQ: opcode = "jne"; break;
    case TREE_LT:  opcode = "jl";  break;
    case TREE_GT:  opcode = "jg";  break;
    case TREE_LE:  opcode = "jle"; break;
    case TREE_GE:  opcode = "jge"; break;
    case TREE_ULT: opcode = "jb";  break;
    case TREE_ULE: opcode = "jbe"; break;
    case TREE_UGT: opcode = "ja";  break;
    case TREE_UGE: opcode = "jae"; break;
    }
  sprintf(inst2, "%s `j0\n", opcode);
  emit(assem_new_oper(inst2,
                      NULL,
                      NULL,
                      assem_new_targets(temp_new_label_list (jt, NULL))));
  sprintf(inst3, "jmp `j0\n");
  emit(assem_new_oper(inst3,
                      NULL,
                      NULL,
                      assem_new_targets(temp_new_label_list (jf, NULL))));
}

static void
munch_stm (tree_stm *s)
{
  char *inst = new (sizeof(char) * 120);
  char *inst2 = new (sizeof(char) * 120);
  char *inst3 = new (sizeof(char) * 120);

  switch (s->kind)
    {
    case TREE_MOVE:
      return generate_move (s, inst, inst2, inst3);

    case TREE_LABEL:
      return generate_label (s, inst, inst2, inst3);

    case TREE_EXP:
      return generate_exp (s, inst, inst2, inst3);

    case TREE_JUMP:
      return generate_jump (s, inst, inst2, inst3);

    case TREE_CJUMP:
      return generate_cjump (s, inst, inst2, inst3);

    default:
      assert(0);
    }
}

static void
munch_caller_save ()
{
  temp_temp_list *caller_saves = frm_caller_saves();

  for (; caller_saves; caller_saves = caller_saves->tail)
    emit(assem_new_oper("pushl `s0\n",
                        temp_new_temp_list (frm_sp(), NULL),
                        temp_new_temp_list (caller_saves->head, NULL),
                        NULL));
}

static void
munch_caller_restore(temp_temp_list *tl)
{
  int restore_cnt = 0;
  char *inst = new (sizeof (char) * 128);

  for (; tl; tl = tl->tail)
    restore_cnt++;

  sprintf(inst, "addl $%d, `s0\n", restore_cnt * frm_word_size);
  emit(assem_new_oper(inst,
                      temp_new_temp_list (frm_sp(), NULL),
                      temp_new_temp_list (frm_sp(), NULL),
                      NULL));

  temp_temp_list *caller_saves = temp_reverse_list(frm_caller_saves());
  for (; caller_saves; caller_saves = caller_saves->tail)
    emit(assem_new_oper("popl `d0\n",
                        temp_new_temp_list (caller_saves->head, NULL),
                        temp_new_temp_list (frm_sp(), NULL),
                        NULL));
}

static temp_temp_list *
munch_args (int            i,
            tree_exp_list *args)
{
  if (args == NULL)
    return NULL;

  temp_temp_list *old = munch_args (i + 1, args->tail);

  temp_temp *r = munch_exp (args->head);
  emit(assem_new_oper ("pushl `s0\n",
                       temp_new_temp_list (frm_sp(), NULL),
                       temp_new_temp_list (r, NULL), NULL));

  // No need to reserve values before calling in x86
  return temp_new_temp_list (r, old);
}
