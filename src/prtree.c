/**
 * @file printtree.c
 * Functions to print out intermediate representation (IR) trees.
 *
 */
#include <assert.h>
#include <stdio.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/prtree.h"

/* local function prototype */
static void pr_tree_exp (FILE *out,
                         tree_exp *exp,
                         int d);

static void indent (FILE *out,
                    int d)
{
  int i;
  for (i = 0; i <= d; i++)
    fprintf(out, " ");
}

static char bin_oper[][12] = {
   "PLUS", "MINUS", "TIMES", "DIVIDE",
   "AND", "OR", "LSHIFT", "RSHIFT", "ARSHIFT", "XOR"};

static char rel_oper[][12] = {
  "EQ", "NE", "LT", "GT", "LE", "GE", "ULT", "ULE", "UGT", "UGE"};

static void pr_stm (FILE *out,
                    tree_stm *stm,
                    int d)
{
  switch (stm->kind)
    {
    case TREE_SEQ:
      indent(out,d);
      fprintf(out, "SEQ(\n");
      pr_stm(out, stm->u.seq.left,d+1);
      fprintf(out, ",\n");
      pr_stm(out, stm->u.seq.right,d+1);
      fprintf(out, ")");
      break;

    case TREE_LABEL:
      indent(out,d);
      fprintf(out, "LABEL %s", sym_name(stm->u.label));
      break;

    case TREE_JUMP:
      indent(out,d);
      fprintf(out, "JUMP(\n");
      pr_tree_exp(out, stm->u.jmp.exp,d+1);
      fprintf(out, ")");
    break;

    case TREE_CJUMP:
      indent(out,d);
      fprintf(out, "CJUMP(%s,\n", rel_oper[stm->u.cjump.op]);
      pr_tree_exp(out, stm->u.cjump.left,d+1);
      fprintf(out, ",\n");
      pr_tree_exp(out, stm->u.cjump.right,d+1);
      fprintf(out, ",\n");
      indent(out,d+1);
      fprintf(out, "%s,", sym_name(stm->u.cjump.truee));
      fprintf(out, "%s", sym_name(stm->u.cjump.falsee));
      fprintf(out, ")");
      break;

    case TREE_MOVE:
      indent(out,d);
      fprintf(out, "MOVE(\n");
      pr_tree_exp(out, stm->u.move.dst,d+1);
      fprintf(out, ",\n");
      pr_tree_exp(out, stm->u.move.src,d+1);
      fprintf(out, ")");
      break;

    case TREE_EXP:
      indent(out,d);
      fprintf(out, "EXP(\n");
      pr_tree_exp(out, stm->u.exp,d+1);
      fprintf(out, ")");
    break;

    default:
      assert (0);
  }
}

static void
pr_tree_exp (FILE *out,
             tree_exp *exp,
             int d)
{
  switch (exp->kind)
    {
    case TREE_BINOP:
      indent(out,d);
      fprintf(out, "BINOP(%s,\n", bin_oper[exp->u.bin_op.op]);
      pr_tree_exp(out, exp->u.bin_op.left,d+1);
      fprintf(out, ",\n");
      pr_tree_exp(out, exp->u.bin_op.right,d+1);
      fprintf(out, ")");
      break;

    case TREE_MEM:
      indent(out,d);
      fprintf(out, "MEM");
      fprintf(out, "(\n");
      pr_tree_exp(out, exp->u.mem,d+1);
      fprintf(out, ")");
      break;

    case TREE_TEMP:
      indent(out,d);
      fprintf(out,
              "TEMP t%s",
              temp_lookup(temp_name(), exp->u.temp));
      break;

    case TREE_ESEQ:
      indent(out,d);
      fprintf(out, "ESEQ(\n");
      pr_stm(out, exp->u.eseq.stm,d+1);
      fprintf(out, ",\n");
      pr_tree_exp(out, exp->u.eseq.exp,d+1);
      fprintf(out, ")");
      break;

    case TREE_NAME:
      indent(out,d);
      fprintf(out, "NAME %s", sym_name(exp->u.name));
      break;

    case TREE_CONST:
      indent(out,d);
      fprintf(out, "CONST %d", exp->u.constt);
      break;

    case TREE_CALL:
      {
        tree_exp_list *args = exp->u.call.args;
        indent(out,d);
        fprintf(out, "CALL(\n");
        pr_tree_exp(out, exp->u.call.fun, d + 1);
        for (;args; args=args->tail)
          {
            fprintf(out, ",\n"); pr_tree_exp(out, args->head,d+2);
          }
        fprintf(out, ")");
        break;
      }
    default:
      assert (0);
    } /* end of switch */
}

void
print_stm_list (FILE *out,
                tree_stm_list *stm_list)
{
  for (; stm_list; stm_list=stm_list->tail)
    {
      pr_stm(out, stm_list->head,0);
      fprintf(out, "\n");
    }
}
