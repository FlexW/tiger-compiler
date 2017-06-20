/**
 * @file escape.c
 * Goes through abstract syntax tree and looks for escaping variables.
 */

#include <assert.h>
#include <stdlib.h>

#include "include/errormsg.h"
#include "include/symbol.h"
#include "include/util.h"
#include "include/escape.h"


typedef struct _esc_entry esc_entry;

struct
_esc_entry
{
  int   depth;
  bool *escape;
};

/* Local function declaration */

static void        traverse_exp     (sym_table *env_ptr,
                                     int        depth,
                                     absyn_exp *exp_ptr);

static void        traverse_dec     (sym_table *env_ptr,
                                     int        depth,
                                     absyn_dec *dec_ptr);

static void        traverse_var     (sym_table *env_ptr,
                                     int        depth,
                                     absyn_var *var_ptr);

static void        traverse_formals (sym_table         *env_ptr,
                                     int                depth,
                                     absyn_fundec_list *fundec_list_ptr);

static esc_entry * new_esc_entry    (int   depth,
                                     bool *escape_ptr);



/**
 * Looks for escaping variables, marks them with true.
 *
 * @param exp_ptr Expression to check.
 */
void
esc_find_escaping_var (absyn_exp *exp_ptr)
{
  sym_table * esc_env = sym_new_table ();
  traverse_exp (esc_env, 0, exp_ptr);
}


static void
traverse_exp (sym_table *env_ptr,
              int        depth,
              absyn_exp *exp_ptr)
{
  if (exp_ptr == NULL)
    return;

  switch (exp_ptr->kind)
    {
    case ABSYN_VAR_EXP:
      return traverse_var (env_ptr, depth, exp_ptr->u.var);

    case ABSYN_CALL_EXP:
      {
        absyn_exp_list *list = exp_ptr->u.call.args;
        for (; list != NULL; list = list->tail)
          traverse_exp (env_ptr, depth, list->head);

        break;
      }

    case ABSYN_RECORD_EXP:
      {
        absyn_efield_list *list = exp_ptr->u.record.fields;
        for (; list != NULL; list = list->tail)
          {
            absyn_efield *efield = list->head;
            traverse_exp (env_ptr, depth, efield->exp);
          }
        break;
      }

    case ABSYN_SEQ_EXP:
      {
        /* Is valid since a sequence can contain nothing. */
        if (exp_ptr->u.seq == NULL)
          return;

        absyn_exp_list *list = exp_ptr->u.seq;
        for (; list != NULL; list = list->tail)
          traverse_exp (env_ptr, depth, list->head);

        break;
      }

    case ABSYN_IF_EXP:
      traverse_exp (env_ptr, depth, exp_ptr->u.iff.test);
      traverse_exp (env_ptr, depth, exp_ptr->u.iff.then);
      if (exp_ptr->u.iff.elsee != NULL)
        traverse_exp (env_ptr, depth, exp_ptr->u.iff.elsee);
      return;

    case ABSYN_WHILE_EXP:
      traverse_exp (env_ptr, depth, exp_ptr->u.whilee.test);
      traverse_exp (env_ptr, depth, exp_ptr->u.whilee.body);
      return;

    case ABSYN_FOR_EXP:
      traverse_exp (env_ptr, depth, exp_ptr->u.forr.lo);
      traverse_exp (env_ptr, depth, exp_ptr->u.forr.hi);

      sym_begin_scope (env_ptr);
      sym_bind_symbol (env_ptr,
                       exp_ptr->u.forr.var,
                       new_esc_entry (depth, &exp_ptr->u.forr.escape));
      traverse_exp (env_ptr, depth, exp_ptr->u.forr.body);
      sym_end_scope (env_ptr);
      return;

    case ABSYN_ARRAY_EXP:
      traverse_exp (env_ptr, depth, exp_ptr->u.array.size);
      traverse_exp (env_ptr, depth, exp_ptr->u.array.init);
      return;

    case ABSYN_LET_EXP:
      {
        sym_begin_scope (env_ptr);

        absyn_dec_list *list = exp_ptr->u.let.decs;
        for (; list != NULL; list = list->tail)
          traverse_dec (env_ptr, depth, list->head);

        sym_end_scope (env_ptr);

        traverse_exp (env_ptr, depth, exp_ptr->u.let.body);
        return;
      }

    case ABSYN_OP_EXP:
      traverse_exp (env_ptr, depth, exp_ptr->u.op.left);
      traverse_exp (env_ptr, depth, exp_ptr->u.op.right);
      return;

    case ABSYN_ASSIGN_EXP:
      traverse_var (env_ptr, depth, exp_ptr->u.assign.var);
      traverse_exp (env_ptr, depth, exp_ptr->u.assign.exp);
      return;

    case ABSYN_NIL_EXP:
    case ABSYN_INT_EXP:
    case ABSYN_STR_EXP:
    case ABSYN_BREAK_EXP:
      return;

    default:
      assert (0);
    }
}

static void traverse_dec (sym_table *env_ptr,
                          int        depth,
                          absyn_dec *dec_ptr)
{
  if (dec_ptr == NULL)
    return;

  switch (dec_ptr->kind)
    {
    case ABSYN_FUNCTION_DEC:
      return traverse_formals (env_ptr, depth, dec_ptr->u.function);

    case ABSYN_TYPE_DEC:
      return;

    case ABSYN_VAR_DEC:
      sym_bind_symbol (env_ptr,
                       dec_ptr->u.var.var,
                       new_esc_entry (depth, &dec_ptr->u.var.escape));
      return traverse_exp (env_ptr, depth, dec_ptr->u.var.init);
    }
  errm_impossible ("Got over switch in traverse_dec()!\n");
}

static void traverse_var (sym_table *env_ptr,
                          int        depth,
                          absyn_var *var_ptr)
{
  if (var_ptr == NULL)
    return;

  switch (var_ptr->kind)
    {
    case ABSYN_SIMPLE_VAR:
      {
        esc_entry *declared_var = sym_lookup (env_ptr, var_ptr->u.simple);
        if (declared_var != NULL && declared_var->depth < depth)
          *declared_var->escape = true;
        return;
      }

    case ABSYN_FIELD_VAR:
      return traverse_var (env_ptr, depth, var_ptr->u.field.var);

    case ABSYN_SUBSCRIPT_VAR:
      traverse_var (env_ptr, depth, var_ptr->u.subscript.var);
      return traverse_exp (env_ptr, depth, var_ptr->u.subscript.exp);
    }

  errm_impossible ("Got over switch in traverse_var()!\n");
}

static esc_entry *
new_esc_entry (int   depth,
               bool *escape_ptr)
{
  esc_entry *entry = new (sizeof (*entry));

  entry->depth  = depth;
  entry->escape = escape_ptr;

  *escape_ptr = false;

  return entry;
}

/*
  Go trough parameter list of function,
   declare them and then process body
*/
static void
traverse_formals (sym_table         *env_ptr,
                  int                depth,
                  absyn_fundec_list *fundec_list_ptr)
{
  absyn_fundec_list *list = fundec_list_ptr;
  for (; list != NULL; list = list->tail)
    {
      depth++;
      sym_begin_scope (env_ptr);

      absyn_fundec     *fundec      = list->head;
      absyn_field_list *params_list = fundec->params;

      for (; params_list != NULL; params_list = params_list->tail)
        {
          absyn_field *param = params_list->head;
          sym_bind_symbol (env_ptr,
                           param->name,
                           new_esc_entry (depth, &param->escape));
        }

      traverse_exp (env_ptr, depth, fundec->body);
      depth--;
      sym_end_scope (env_ptr);

    }
}
