/**
 * @file assem.c
 * Functions to translate to Assem-instructions for the
 * Jouette assembly language using Maximal Munch.
 */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h> /* for atoi */
#include <string.h> /* for strcpy */

#include "include/util.h"
#include "include/symbol.h"
#include "include/absyn.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/assem.h"
#include "include/frame.h"
#include "include/errormsg.h"

assem_targets *
assem_new_targets (temp_label_list *labels)
{
  assem_targets *p = new (sizeof *p);

  p->labels = labels;

  return p;
}

assem_instr *
assem_new_oper (char *a,
                temp_temp_list *d,
                temp_temp_list *s,
                assem_targets  *j)
{
  assem_instr *p = new (sizeof *p);

  p->kind         = I_OPER;
  p->u.oper.assem = a;
  p->u.oper.dst   = d;
  p->u.oper.src   = s;
  p->u.oper.jumps = j;

  return p;
}

assem_instr *
assem_new_label (char       *a,
                 temp_label *label)
{
  assem_instr *p = new (sizeof *p);

  p->kind          = I_LABEL;
  p->u.label.assem = a;
  p->u.label.label = label;

  return p;
}

assem_instr *
assem_new_move (char           *a,
                temp_temp_list *d,
                temp_temp_list *s)
{
  assem_instr *p = new (sizeof *p);

  p->kind         = I_MOVE;
  p->u.move.assem = a;
  p->u.move.dst   = d;
  p->u.move.src   = s;

  return p;
}

assem_proc *
assem_new_proc (char             *p,
                assem_instr_list *b,
                char             *e)
{
  assem_proc *proc = new (sizeof (*proc));

 proc->prolog = p;
 proc->body   = b;
 proc->epilog = e;

 return proc;
}

/* c should be COL_color; temporarily it is not */
void
assem_print_instr_list (FILE             *out,
                        assem_instr_list *ilist,
                        temp_map         *m)
{
  for (; ilist; ilist = ilist->tail)
    {
      assem_print(out, ilist->head, m);
      fprintf (out, "\n");
    }
  fprintf (out, "\n");
}

/* put list b at the end of list a */
assem_instr_list *
assem_splice (assem_instr_list *a,
              assem_instr_list *b)
{
  assem_instr_list *p;

  if (a == NULL)
    return b;

  for (p = a; p->tail != NULL; p = p->tail)
    ;

  p->tail = b;
  return a;
}

/**
 * Entry point for assembly instructions generation.
 *
 * @param frame    Frame.
 * @param stm_list Intermediate code.
 *
 * @returns Assembly instruction list.
 */
/*assem_instr_list *
assem_codegen (frm_frame     *frame,
               tree_stm_list *stm_list)
{
  tree_stm_list *slist;
  global_frame = frame;

  for (slist = stm_list; slist != NULL; slist = slist->tail)
    {
      munch_stm (slist->head);
    }
  assem_instr_list *instr_list = global_instr_list;

  global_instr_list      = NULL;
  global_instr_list_last = NULL;

  return instr_list;
  }*/

static temp_temp *
nth_temp (temp_temp_list *list,
          int             i)
{
  assert(list);

  if (i==0)
    return list->head;
  else
    return nth_temp (list->tail, i - 1);
}

static temp_label *
nth_label (temp_label_list *list,
           int              i)
{
  assert (list);

  if (i == 0)
    return list->head;
  else
    return nth_label (list->tail, i - 1);
}

/* First param is string created by this function by reading 'assem' string
 * and replacing `d `s and `j stuff.
 * Last param is function to use to determine what to do with each temp.
 */
static
void format (char           *result,
             char           *assem,
             temp_temp_list *dst,
             temp_temp_list *src,
             assem_targets  *jumps,
             temp_map       *m)
{
  char *p;
  int   i = 0; /* offset to result string */

  for(p = assem; p && *p != '\0'; p++)
    {
      if (*p == '`')
        {
          switch(*(++p))
            {
            case 's':
              {
                int   n = atoi (++p);
                char *s = temp_lookup (m, nth_temp (src, n));
                strcpy (result + i, s);
                i += strlen(s);
              }
              break;

            case 'd':
              {
                int   n = atoi(++p);
                char *s = temp_lookup (m, nth_temp (dst, n));
                strcpy (result + i, s);
                i += strlen(s);
              }
              break;

            case 'j':
              assert (jumps);
              {
                int   n = atoi (++p);
                char *s = temp_label_str (nth_label (jumps->labels,n));
                strcpy (result + i, s);
                i += strlen (s);
              }
              break;

            case '`':
              result[i] = '`';
              i++;
              break;

            default:
              assert(0);
            }
        }
      else
        {
          result[i] = *p;
          i++;
        }
    }
  result[i] = '\0';
}

void
assem_print (FILE        *out,
             assem_instr *instr,
             temp_map    *map)
{
  char result[200];

  switch (instr->kind)
    {
    case I_OPER:
      format (result,
              instr->u.oper.assem,
              instr->u.oper.dst,
              instr->u.oper.src,
              instr->u.oper.jumps,
              map);
      fprintf(out, "%s", result);
      break;

    case I_LABEL:
      format(result, instr->u.label.assem, NULL, NULL, NULL, map);
      fprintf(out, "%s", result);
      /* i->u.LABEL->label); */
      break;

    case I_MOVE:
      format(result,
             instr->u.move.assem,
             instr->u.move.dst,
             instr->u.move.src,
             NULL,
             map);
      fprintf(out, "%s", result);
      break;
    default:
      assert (0);
  }
}

static char *
bin_op_lookup (tree_bin_op op)
{
  switch (op)
    {
    case TREE_PLUS:
      return "add";

    case TREE_MINUS:
      return "sub";

    case TREE_DIVIDE:
      return "div";

    case TREE_TIMES:
      return "mul";

    default:
      assert (0);
    }
}

static char *
cmp_jmp_lookup (tree_rel_op op)
{
  switch (op)
    {
    case TREE_EQ:
      return "je";
    case TREE_NEQ:
      return "jne";
    case TREE_LT:
      return "jl";
    case TREE_GT:
      return "jg";
    case TREE_LE:
      return "jle";
    case TREE_GE:
      return "jge";
    default:
      assert (0);
    }
}
