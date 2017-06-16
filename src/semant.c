/**
 * @file semant.c
 * Functions to realize the semantic analyse of the compiler.
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/symbol.h"
#include "include/util.h"
#include "include/errormsg.h"
#include "include/debug.h"
#include "include/translate.h"
#include "include/types.h"
#include "include/absyn.h"
#include "include/env.h"
#include "include/escape.h"

typedef struct _bool_list bool_list;
typedef struct _expty     expty;

#define TRANS_ERROR new_expty (tra_int_exp (0), typ_new_int ());

bool_list *loop_status = NULL; /* List to keep track of loop status */

/**
 * Struct to store translated expression with type information.
 */
struct _expty
{
  tra_exp *exp;
  typ_ty  *ty;
};

struct
_bool_list
{
  bool       head;
  bool_list *tail;
};

/* Local function declarations */

static bool_list*      new_bool_list         (bool       head,
                                              bool_list *tail_ptr);

static expty*          new_expty             (tra_exp *exp_ptr,
                                              typ_ty *ty_ptr);

static typ_field_list* mk_formal_field_list  (sym_table        *tenv_ptr,
                                              absyn_field_list *list_ptr);

static typ_ty_list*    mk_formal_ty_list     (sym_table        *tenv_ptr,
                                              absyn_field_list *list_ptr);

static util_bool_list* mk_formal_escape_list (absyn_field_list *params_ptr);

static bool            check_dec_till_mark   (sym_table *table_ptr,
                                              sym_symbol *sym_ptr);

static void            set_loop_status       (bool_list **loop_stat,
                                              bool        flag);

static void            unset_loop_status     (bool_list **loop_stat);

static bool            get_loop_status       (bool_list *loop_stat);

static TRA_OP          get_tra_op            (ABSYN_OP op);

static void            process_func_header   (tra_level  *level_ptr,
                                              sym_table  *venv_ptr,
                                              sym_table  *tenv_ptr,
                                              absyn_dec  *dec_ptr);

static void            process_func_body     (sym_table  *venv_ptr,
                                              sym_table  *tenv_ptr,
                                              absyn_dec  *dec_ptr,
                                              temp_label *break_done);

static void            process_type_header   (sym_table  *tenv_ptr,
                                              absyn_dec  *dec_ptr);

static void            process_type_body     (sym_table  *tenv_ptr,
                                              absyn_dec  *dec_ptr);

static void            check_infinite_types  (sym_table  *tenv_ptr,
                                              absyn_dec  *dec_ptr);

static expty*  trans_var (tra_level *level_ptr,
                          sym_table *venv_ptr,
                          sym_table *tenv_ptr,
                          absyn_var *var_ptr,
                          temp_label *break_done);

static expty*  trans_exp (tra_level *level_ptr,
                          sym_table *venv_ptr,
                          sym_table *tenv_ptr,
                          absyn_exp *exp_ptr,
                          temp_label *break_done);

static tra_exp* trans_dec (tra_level *level_ptr,
                          sym_table *venv_ptr,
                          sym_table *tenv_ptr,
                          absyn_dec *dec_ptr,
                          temp_label *break_done);

static typ_ty* trans_ty  (sym_table *tenv_ptr,
                          absyn_ty  *ty_ptr);


static expty* check_simple_var    (tra_level *level_ptr,
                                   sym_table *venv_ptr,
                                   sym_table *tenv_ptr,
                                   absyn_var *var_ptr,
                                   temp_label *break_done);

static expty* check_subscript_var (tra_level *level_ptr,
                                   sym_table *venv_ptr,
                                   sym_table *tenv_ptr,
                                   absyn_var *var_ptr,
                                   temp_label *break_done);

static expty* check_field_var     (tra_level *level_ptr,
                                   sym_table *venv_ptr,
                                   sym_table *tenv_ptr,
                                   absyn_var *var_ptr,
                                   temp_label *break_done);

static expty* check_var_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_nil_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_int_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_str_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_call_exp   (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_op_exp     (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_record_exp (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_seq_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_assign_exp (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_if_exp     (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_while_exp  (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_for_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_break_exp  (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_let_exp    (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);

static expty* check_array_exp  (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_exp *exp_ptr,
                                temp_label *break_done);


static tra_exp* check_fun_dec  (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_dec *dec_ptr,
                                temp_label *break_done);

static tra_exp* check_var_dec  (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_dec *dec_ptr,
                                temp_label *break_done);

static tra_exp* check_type_dec (tra_level *level_ptr,
                                sym_table *venv_ptr,
                                sym_table *tenv_ptr,
                                absyn_dec *dec_ptr,
                                temp_label *break_done);

static typ_ty* check_array_ty  (sym_table *tenv_ptr,
                                absyn_ty  *ty_ptr);

static typ_ty* check_record_ty (sym_table *tenv_ptr,
                                absyn_ty  *ty_ptr);

static typ_ty* check_name_ty   (sym_table *tenv_ptr,
                                absyn_ty  *ty_ptr);

/* End local function declarations */


static expty *
new_expty (tra_exp *exp_ptr,
           typ_ty *ty_ptr)
{
  expty *e = new (sizeof (*e));

  e->exp = exp_ptr;
  e->ty  = ty_ptr;

  return e;
}

/**
 * Translates and checks the semantic of a given program.
 *
 * @param exp_ptr The expression (program) to check and translate.
 *
 * @return Fragment list.
 */
frm_frag_list *
sem_trans_prog (absyn_exp *exp_ptr)
{
  sym_table *venv;
  sym_table *tenv;

  tenv = env_base_tenv (); /* Get basic type enviroment */
  venv = env_base_venv (); /* Get basic variable enviroment */

  esc_find_escaping_var (exp_ptr); /* look for escaping variables */
  /* Do sematic analyse */
  trans_exp (tra_outermost_level (), venv, tenv, exp_ptr, NULL);

  return tra_get_frag_list ();
}

/**
 * Checks kind of variable. Calls specific function to handle variable type.
 * Calls function that check if variable is declared.
 *
 * @param level_ptr Current function declaration level
 * @param venv_ptr  Variable enviroment.
 * @param tenv_ptr  Type enviroment.
 * @param var_ptr   The variable to check.
 *
 * @return Struct the hold translated expression and type.
 */
static expty *
trans_var (tra_level *level_ptr,
           sym_table *venv_ptr,
           sym_table *tenv_ptr,
           absyn_var *var_ptr,
           temp_label *break_done)
{
  if (var_ptr == NULL)
    return new_expty (NULL, typ_new_int ());

  switch (var_ptr->kind)
    {
    case ABSYN_SIMPLE_VAR:
      return check_simple_var (level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               var_ptr,
                               break_done);

    case ABSYN_FIELD_VAR:
      return check_field_var (level_ptr,
                              venv_ptr,
                              tenv_ptr,
                              var_ptr,
                              break_done);

    case ABSYN_SUBSCRIPT_VAR:
      return check_subscript_var (level_ptr,
                                  venv_ptr,
                                  tenv_ptr,
                                  var_ptr,
                                  break_done);
    }

  errm_impossible ("Got over switch in trans_var()!\n");
  return NULL;
}

static expty *
check_simple_var (tra_level *level_ptr,
                  sym_table *venv_ptr,
                  sym_table *tenv_ptr,
                  absyn_var *var_ptr,
                  temp_label *break_done)
{
  debug_print ("var_ptr->u.simple: %s\n", sym_name (var_ptr->u.simple));
  env_enventry *enventry = sym_lookup (venv_ptr, var_ptr->u.simple);

  /* See if variable is declared */
  if (enventry && enventry->kind == ENV_VAR_ENTRY)
    {
      /* Translate to intermediate code */
      tra_exp *exp = tra_simple_var (enventry->u.var.access, level_ptr);
      return new_expty (exp, typ_actual_ty(enventry->u.var.ty));
    }

  errm_printf (var_ptr->pos,
               "Undefined variable %s",
               sym_name(var_ptr->u.simple));
  return TRANS_ERROR
}

static expty*
check_field_var (tra_level *level_ptr,
                 sym_table *venv_ptr,
                 sym_table *tenv_ptr,
                 absyn_var *var_ptr,
                 temp_label *break_done)
{
  expty *expty = trans_var (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            var_ptr->u.field.var,
                            break_done);

  if (expty->ty->kind != TYP_RECORD)
    {
      errm_printf (var_ptr->pos, "Expected record type");
      return TRANS_ERROR
    }

  /* Go trough each record entry and look for the matching one */
  int offset = 1; /* Offset is needed to calculate exact memory position of
                     record field */

  typ_field *field;
  LINKED_LIST_FOR_EACH (field, expty->ty->u.record)
    {
      if (!strcmp(sym_name (field->name),
                  sym_name(var_ptr->u.field.sym)))
        {
          tra_exp *tra_field = tra_field_var (expty->exp, offset);
          return new_expty (tra_field, typ_actual_ty (field->ty));
        }
      offset++;
    }

  errm_printf (var_ptr->pos, "Field %s not declared",
               sym_name (var_ptr->u.field.sym));
  return TRANS_ERROR
}

static expty*
check_subscript_var (tra_level *level_ptr,
                     sym_table *venv_ptr,
                     sym_table *tenv_ptr,
                     absyn_var *var_ptr,
                     temp_label *break_done)
{
  /*
    Check that var is declared.
    Check that exp is integer and greater equal zero.
  */
  expty *array = trans_var (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            var_ptr->u.subscript.var,
                            break_done);
  expty *index = trans_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            var_ptr->u.subscript.exp,
                            break_done);

  if (array->ty->kind != TYP_ARRAY) /* Check that var is array */
    {
      errm_printf (var_ptr->pos, "Is not a array type");
      return TRANS_ERROR
    }

  if (index->ty->kind != TYP_INT) /* Check that index is integer */
    {
      errm_printf (var_ptr->pos, "Array index must be integer");
      return new_expty (NULL, typ_new_int ());
    }

  tra_exp *exp = tra_subscript_var (array->exp, index->exp);
  return new_expty (exp, typ_actual_ty (array->ty->u.array));
}

/**
 * Checks kind of expression.
 * Calls specific function to handle expression type.
 * Calls function that performs type checking.
 *
 * @param level_ptr Current function declaration level
 * @param venv_ptr  Variable enviroment.
 * @param tenv_ptr  Type enviroment.
 * @param exp_ptr   The expression to check.
 *
 * @return Struct the hold translated expression and type.
 */
static expty*
trans_exp (tra_level *level_ptr,
           sym_table *venv_ptr,
           sym_table *tenv_ptr,
           absyn_exp *exp_ptr,
           temp_label *break_done)
{
  if (exp_ptr == NULL)
    return new_expty (NULL, typ_new_int ());

  switch (exp_ptr->kind)
    {
    case ABSYN_VAR_EXP:
      return check_var_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_NIL_EXP:
      return check_nil_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_INT_EXP:
      return check_int_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_STR_EXP:
      return check_str_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_CALL_EXP:
      return check_call_exp (level_ptr,
                             venv_ptr,
                             tenv_ptr,
                             exp_ptr,
                             break_done);

    case ABSYN_RECORD_EXP:
      return check_record_exp (level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               exp_ptr,
                               break_done);

    case ABSYN_SEQ_EXP:
      return check_seq_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_IF_EXP:
      return check_if_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr,
                           break_done);

    case ABSYN_WHILE_EXP:
      return check_while_exp (level_ptr,
                              venv_ptr,
                              tenv_ptr,
                              exp_ptr,
                              break_done);

    case ABSYN_FOR_EXP:
      return check_for_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_BREAK_EXP:
      return check_break_exp (level_ptr,
                              venv_ptr,
                              tenv_ptr,
                              exp_ptr,
                              break_done);

    case ABSYN_ARRAY_EXP:
      return check_array_exp (level_ptr,
                              venv_ptr,
                              tenv_ptr,
                              exp_ptr,
                              break_done);

    case ABSYN_LET_EXP:
      return check_let_exp (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            exp_ptr,
                            break_done);

    case ABSYN_OP_EXP:
      return check_op_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr,
                           break_done);

    case ABSYN_ASSIGN_EXP:
      return check_assign_exp (level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               exp_ptr,
                               break_done);
    }

  errm_impossible ("Got over switch in trans_exp()!\n");
  return NULL;
}

static expty *
check_var_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  return trans_var (level_ptr, venv_ptr, tenv_ptr, exp_ptr->u.var, break_done);
}

static expty *
check_nil_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  tra_exp *exp = tra_nil_exp ();
  return new_expty (exp, typ_new_nil ());
}

static expty *
check_int_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  tra_exp *exp = tra_int_exp (exp_ptr->u.intt);
  return new_expty (exp, typ_new_int ());
}

static expty *
check_str_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  return new_expty (tra_str_exp (exp_ptr->u.str), typ_new_str ());
}

static expty *
check_call_exp (tra_level *level_ptr,
                sym_table *venv_ptr,
                sym_table *tenv_ptr,
                absyn_exp *exp_ptr,
                temp_label *break_done)
{
  //expty        *call_arg;
  env_enventry *fundec = sym_lookup (venv_ptr, exp_ptr->u.call.func);

  /* See if function is declared */
  if (fundec == NULL || fundec->kind != ENV_FUN_ENTRY)
    {
      errm_printf (exp_ptr->pos,
                   "Function %s not declared",
                   sym_name (exp_ptr->u.call.func));
      return TRANS_ERROR
    }

  absyn_exp_list *fun_call_arg_list = exp_ptr->u.call.args;
  typ_ty_list    *fun_dec_arg_list  = fundec->u.fun.formals;
  tra_exp_list   *tra_list          = linked_list_new ();
  /*
    Go trough each argument
    See if calling argument and declared argument are the same
  */
  absyn_exp *call;
  typ_ty    *dec;
  for (int i = 0;
       (call = linked_list_get (fun_call_arg_list, i)) != NULL;
       i++)
    {
      dec = linked_list_get (fun_dec_arg_list, i);

      if (call == NULL)
        {
          errm_printf (exp_ptr->pos,
                       "To less arguments in function %s",
                       sym_name (exp_ptr->u.call.func));
          return TRANS_ERROR
        }
      if (dec == NULL)
        {
          errm_printf (exp_ptr->pos,
                       "To much arguments in function %s",
                       sym_name (exp_ptr->u.call.func));
          return TRANS_ERROR
        }

      expty* tra_arg = trans_exp (level_ptr,
                                  venv_ptr,
                                  tenv_ptr,
                                  call,
                                  break_done);
      linked_list_add (tra_list, tra_arg->exp);

      if (!typ_cmpty (tra_arg->ty, dec))
        {
          errm_printf (call->pos,
                       "Argument in function %s are not of the same type",
                       sym_name (exp_ptr->u.call.func));
          return TRANS_ERROR
        }
    }

  tra_exp *tra_exp = tra_call_exp (fundec->u.fun.label,
                                   tra_list,
                                   fundec->u.fun.level,
                                   level_ptr);
  return new_expty (tra_exp, typ_actual_ty (fundec->u.fun.result));
}

static expty *
check_record_exp (tra_level *level_ptr,
                  sym_table *venv_ptr,
                  sym_table *tenv_ptr,
                  absyn_exp *exp_ptr,
                  temp_label *break_done)
{
  /* See if type is declared */
  typ_ty *typ = typ_lookup (exp_ptr->pos, exp_ptr->u.record.typ, tenv_ptr);

  if (typ == NULL || typ->kind != TYP_RECORD)
    {
      errm_printf (exp_ptr->pos,
                   " %s is not record type",
                   sym_name (exp_ptr->u.record.typ));
      return TRANS_ERROR
    }

  typ_field_list    *field_list  = typ->u.record;
  absyn_efield_list *efield_list = exp_ptr->u.record.fields;
  tra_exp_list      *tra_list    = linked_list_new ();

  /* See if initializer types are the same like declared types */
  absyn_efield *efield;
  typ_field    *field;

  debug_print ("efield_list length: %i, field_list length: %i\n",
               linked_list_length (efield_list),
               linked_list_length (field_list));
  for (int i = 0;
       (efield = linked_list_get (efield_list, i)) != NULL
         && (field = linked_list_get (field_list, i)) != NULL;
       i++)
    {
      if (field == NULL)
        {
          errm_printf (exp_ptr->pos,
                       "To much parameters in record %s",
                       sym_name (exp_ptr->u.record.typ));
          return TRANS_ERROR
        }
      /* See if initializer name matches */
      debug_print ("field->name: %s, efield->name: %s\n",
                   sym_name (field->name),
                   sym_name (efield->name));

      if (strcmp (sym_name (field->name), sym_name (efield->name)))
        {
          errm_printf (exp_ptr->pos,
                       "Wrong parameter used in record %s",
                       sym_name (exp_ptr->u.record.typ));
          return TRANS_ERROR
        }

      /* See if type matches */
      expty *init = trans_exp (level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               efield->exp,
                               break_done);
      linked_list_add (tra_list, init->exp);

      if (!typ_cmpty(init->ty, field->ty))
        {
          errm_printf (efield->exp->pos,
                       "Wrong type used in record %s",
                       sym_name (exp_ptr->u.record.typ));
          return TRANS_ERROR
        }
    }

  return new_expty (tra_record_exp (tra_list), typ_actual_ty (typ));
}

static expty *
check_seq_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  tra_exp_list   *tra_list     = linked_list_new ();
  absyn_exp_list *exp_list     = exp_ptr->u.seq;

  if (exp_list == NULL) /* empty sequence is valid */
    return new_expty (tra_int_exp (0), typ_new_void ());

  /* Translate every expression, return type of the last */
  absyn_exp *exp;
  expty     *tra_exp;
  LINKED_LIST_FOR_EACH (exp, exp_list)
    {
      if (exp->kind == ABSYN_BREAK_EXP)
        {
          tra_exp = trans_exp (level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               exp,
                               break_done);
        }
      else
        {
          /* Nested breaks are not allowed */
          set_loop_status (&loop_status, false);
          tra_exp = trans_exp (level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               exp,
                               break_done);
          unset_loop_status (&loop_status);
        }

      linked_list_add (tra_list, tra_exp->exp);
    }

  return new_expty (tra_seq_exp (tra_list), tra_exp->ty);
}

static expty *
check_assign_exp (tra_level *level_ptr,
                  sym_table *venv_ptr,
                  sym_table *tenv_ptr,
                  absyn_exp *exp_ptr,
                  temp_label *break_done)
{
  /*
    Check that both values have the same type.
    Return that type.
  */
  expty *container = trans_var(level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               exp_ptr->u.assign.var,
                               break_done);
  expty *alloc     = trans_exp(level_ptr,
                               venv_ptr,
                               tenv_ptr,
                               exp_ptr->u.assign.exp,
                               break_done);

  if (!typ_cmpty (container->ty, alloc->ty))
    {
      errm_printf (exp_ptr->pos, "Types do not match");
      return TRANS_ERROR
    }

  return new_expty (tra_assign_exp (container->exp, alloc->exp), alloc->ty);
}

static expty *
check_if_exp (tra_level *level_ptr,
              sym_table *venv_ptr,
              sym_table *tenv_ptr,
              absyn_exp *exp_ptr,
              temp_label *break_done)
{
  /*
    Translate all three expressions.
    Check that first expression is integer.
    Check that both other expressions have the same type.
    Return that type or if they return nothing, return void.
  */
  expty *elsee = NULL;
  expty *then  = NULL;
  expty *test  = NULL;

  if (exp_ptr->u.iff.test != NULL)
    {
      test  = trans_exp(level_ptr,
                        venv_ptr,
                        tenv_ptr,
                        exp_ptr->u.iff.test,
                        break_done);
    }
  else
    {
      test = new_expty (NULL, typ_new_int ());
      errm_printf (exp_ptr->pos, "Expression after if requierd");
    }

  if (exp_ptr->u.iff.then != NULL)
    {
      then  = trans_exp (level_ptr,
                         venv_ptr,
                         tenv_ptr,
                         exp_ptr->u.iff.then,
                         break_done);
    }
  else
    {
      then = new_expty (NULL, typ_new_int ());
      errm_printf (exp_ptr->pos, "Expression after then requierd");
    }

  if (exp_ptr->u.iff.elsee != NULL) /* else is optional */
      elsee = trans_exp(level_ptr,
                        venv_ptr,
                        tenv_ptr,
                        exp_ptr->u.iff.elsee,
                        break_done);

  if (test != NULL && test->ty->kind != TYP_INT)
    {
      errm_printf (exp_ptr->u.iff.test->pos, "Condition must be integer");
      return TRANS_ERROR
    }

  /* if if .. then .. else .. , then and else should return same value */
  if (elsee != NULL && !typ_cmpty (then->ty, elsee->ty))
    {
      errm_printf (exp_ptr->u.iff.elsee->pos, "Return type must be the same");
      return TRANS_ERROR
    }

  /* if just if .. then ... , then should return no value */
  if (elsee == NULL)
    {
      if (then->ty->kind != TYP_VOID)
        {
          errm_printf (exp_ptr->pos,
                       "if then expression should return void value!");
          return TRANS_ERROR
        }

      return new_expty (tra_if_exp (test->exp, then->exp, NULL), then->ty);
    }

  return new_expty (tra_if_exp (test->exp, then->exp, elsee->exp), then->ty);
}

static expty*
check_while_exp (tra_level  *level_ptr,
                 sym_table  *venv_ptr,
                 sym_table  *tenv_ptr,
                 absyn_exp  *exp_ptr,
                 temp_label *break_done)
{
  temp_label *done = temp_new_label (); /* For break statement */
  /* Translate test expression. Check that it is integer. */
  expty *test = trans_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr->u.whilee.test,
                           break_done);
  if (test->ty->kind != TYP_INT)
    {
      errm_printf (exp_ptr->u.whilee.test->pos,
                   "Test Condition in while loop must be integer");
      return TRANS_ERROR
    }

  set_loop_status (&loop_status, true);
  expty *body = trans_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr->u.whilee.body,
                           done);
  unset_loop_status (&loop_status);

  if (body->ty->kind != TYP_VOID) /* Body should return void */
    {
      errm_printf (exp_ptr->u.whilee.body->pos,
                   "Body of while loop should return void");
      return TRANS_ERROR
    }
  return new_expty (tra_while_exp (test->exp, body->exp, done), body->ty);
}

static expty*
check_for_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  /*
    Begin new scope. Add symbol to variable enviroment.
    Check that lo and hi expression produce integer.
    Check return type of body exp. Return this type or void.
  */
  sym_begin_scope (venv_ptr);
  /* Add counter to enviroment */
  tra_access *access = tra_alloc_local (level_ptr, exp_ptr->u.forr.escape);
  sym_bind_symbol (venv_ptr,
                   exp_ptr->u.forr.var,
                   env_new_var_entry (access, typ_new_int ()));

  set_loop_status (&loop_status, true);
  expty *body = trans_exp(level_ptr,
                          venv_ptr,
                          tenv_ptr,
                          exp_ptr->u.forr.body,
                          break_done);
  unset_loop_status (&loop_status);

  expty *lo   = trans_exp(level_ptr,
                          venv_ptr,
                          tenv_ptr,
                          exp_ptr->u.forr.lo,
                          break_done);
  expty *hi   = trans_exp(level_ptr,
                          venv_ptr,
                          tenv_ptr,
                          exp_ptr->u.forr.hi,
                          break_done);

  /* Check that counter is of type int */
  if (lo->ty->kind != TYP_INT || hi->ty->kind != TYP_INT)
    {
      errm_printf (exp_ptr->u.forr.lo->pos,
                   "Low and High must be of type integer");
    }

  /* Body should return void */
  if (body->ty->kind != TYP_VOID)
    {
      errm_printf (exp_ptr->u.forr.body->pos,
                   "for loop should return void");
      return TRANS_ERROR
    }

  sym_end_scope (venv_ptr);
  tra_exp *exp = tra_for_exp (lo->exp, hi->exp, body->exp);

  return new_expty (exp, body->ty);
}

static expty*
check_break_exp (tra_level *level_ptr,
                 sym_table *venv_ptr,
                 sym_table *tenv_ptr,
                 absyn_exp *exp_ptr,
                 temp_label *break_done)
{
  if (!get_loop_status (loop_status))
    errm_printf (exp_ptr->pos,
                 "break statement must be in for or while loop");

  return new_expty (tra_break_exp (break_done), typ_new_void ());
}

static expty*
check_array_exp (tra_level *level_ptr,
                 sym_table *venv_ptr,
                 sym_table *tenv_ptr,
                 absyn_exp *exp_ptr,
                 temp_label *break_done)
{
  /* Check that typ is declared in type enviroment. */
  typ_ty *ty  = typ_lookup (exp_ptr->pos, exp_ptr->u.array.typ, tenv_ptr);
  expty *size = trans_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr->u.array.size,
                           break_done);
  expty *init = trans_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr->u.array.init,
                           break_done);

  if (size->ty->kind != TYP_INT) /* Size must be integer */
    {
      errm_printf (exp_ptr->u.array.size->pos, "Array size must be integer");
      return TRANS_ERROR
    }

  if (!typ_cmpty (init->ty, ty->u.array)) /* init and array type must match */
    {
      errm_printf (exp_ptr->u.array.init->pos,
                   "Array init should be same type like array");
      return TRANS_ERROR
    }

  return new_expty (tra_array_exp (size->exp, init->exp),
                    typ_actual_ty (ty));
}

static expty*
check_let_exp (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_exp *exp_ptr,
               temp_label *break_done)
{
  tra_exp_list   *tra_list = linked_list_new ();
  absyn_dec_list *dec_list = exp_ptr->u.let.decs;

  sym_begin_scope (venv_ptr); /* Start new declaration scope */
  sym_begin_scope (tenv_ptr);

  absyn_dec *dec;
  LINKED_LIST_FOR_EACH (dec, dec_list)
  //for (int i = linked_list_length (dec_list) - 1; i >= 0; i--)
    {
      dec = linked_list_get (dec_list, i);
      tra_exp* tra_exp = trans_dec (level_ptr,
                                    venv_ptr,
                                    tenv_ptr,
                                    dec,
                                    break_done);
      linked_list_add (tra_list, tra_exp);
    }
  /* Go trough body */
  expty *body = trans_exp (level_ptr,
                           venv_ptr,
                           tenv_ptr,
                           exp_ptr->u.let.body,
                           break_done);

  sym_end_scope(tenv_ptr); /* End declaration scope */
  sym_end_scope(venv_ptr);

  return new_expty (tra_let_exp (tra_list, body->exp), body->ty);
}

static expty*
check_op_exp (tra_level *level_ptr,
              sym_table *venv_ptr,
              sym_table *tenv_ptr,
              absyn_exp *exp_ptr,
              temp_label *break_done)
{
  ABSYN_OP op    = exp_ptr->u.op.op;
  expty   *left  = trans_exp (level_ptr,
                              venv_ptr,
                              tenv_ptr,
                              exp_ptr->u.op.left,
                              break_done);
  expty   *right = trans_exp (level_ptr,
                              venv_ptr,
                              tenv_ptr,
                              exp_ptr->u.op.right,
                              break_done);

  switch (op)
    {
    case ABSYN_PLUS_OP:
    case ABSYN_MINUS_OP:
    case ABSYN_TIMES_OP:
    case ABSYN_DIVIDE_OP:
      {
        if (left->ty->kind != TYP_INT)
          {
            errm_printf (exp_ptr->u.op.left->pos, "Integer requierd");
            return TRANS_ERROR
          }
        if (right->ty->kind != TYP_INT)
          {
            errm_printf (exp_ptr->u.op.right->pos, "Integer requierd");
            return TRANS_ERROR
          }
        tra_exp *exp = tra_arithmetic_exp (left->exp,
                                           right->exp ,
                                           get_tra_op (op));
        return new_expty (exp, typ_new_int ());
      }
    case ABSYN_LT_OP:
    case ABSYN_LE_OP:
    case ABSYN_GT_OP:
    case ABSYN_GE_OP:
      {
        if (left->ty->kind != TYP_INT)
          {
            errm_printf (exp_ptr->u.op.left->pos, "Integer requierd");
            return TRANS_ERROR
          }
        if (right->ty->kind != TYP_INT)
          {
            errm_printf (exp_ptr->u.op.right->pos, "Integer requierd");
            return TRANS_ERROR
          }
        tra_exp *exp = tra_conditional_exp (left->exp,
                                            right->exp,
                                            get_tra_op (op));
        return new_expty (exp, typ_new_int ());
      }
    case ABSYN_EQ_OP:
    case ABSYN_NEQ_OP:
      {
        if (!typ_cmpty (left->ty, right->ty))
          {
            errm_printf (exp_ptr->u.op.left->pos,
                         "Operands must be of the same type");
            return TRANS_ERROR
          }
        tra_exp *exp = tra_conditional_exp (left->exp,
                                            right->exp,
                                            get_tra_op (op));
        return new_expty (exp, typ_new_int ());
      }
    }
  errm_impossible ("after switch in check_op_exp()");
  return NULL;
}


/**
 * Checks kind of declaration. Calls specific function to handle
 * declaration type.
 * Calls function that performs the declaration.
 *
 * @param level_ptr Current function declaration level
 * @param venv_ptr  Variable enviroment.
 * @param tenv_ptr  Type enviroment.
 * @param dec_ptr   The declaration to check.
 */
static tra_exp *
trans_dec (tra_level *level_ptr,
           sym_table *venv_ptr,
           sym_table *tenv_ptr,
           absyn_dec *dec_ptr,
           temp_label *break_done)
{
  if (dec_ptr == NULL)
    return NULL;

  switch (dec_ptr->kind)
    {
    case ABSYN_FUNCTION_DEC:
      return check_fun_dec (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            dec_ptr,
                            break_done);

    case ABSYN_TYPE_DEC:
      return check_type_dec (level_ptr,
                             venv_ptr,
                             tenv_ptr,
                             dec_ptr,
                             break_done);

    case ABSYN_VAR_DEC:
      return check_var_dec (level_ptr,
                            venv_ptr,
                            tenv_ptr,
                            dec_ptr,
                            break_done);
    }
  errm_impossible ("Got over switch in trans_dec()!\n");
  return NULL;
}

static tra_exp *
check_fun_dec (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_dec *dec_ptr,
               temp_label *break_done)
{
  process_func_header (level_ptr, venv_ptr, tenv_ptr, dec_ptr);
  process_func_body (venv_ptr, tenv_ptr, dec_ptr, break_done);
  return tra_func_dec ();
}

static void
process_func_header (tra_level  *level_ptr,
                     sym_table  *venv_ptr,
                     sym_table  *tenv_ptr,
                     absyn_dec  *dec_ptr)
{
  absyn_fundec_list *fundeclist = dec_ptr->u.function;
  absyn_fundec      *fundec;
  LINKED_LIST_FOR_EACH (fundec, fundeclist)
    {
      /* Check for redeclaration */
      absyn_fundec *fundec2;
      for (int j = i + 1;
           (fundec2 = linked_list_get (fundeclist, j)) != NULL;
           j++)
        {
          if (fundec->name == fundec2->name)
              errm_printf (fundec->pos,
                           "Illegal redeclaration of function %s",
                           sym_name (fundec->name));
        }
      /* See if function or procedure */
      typ_ty *typ;
      if (fundec->result == NULL) /* We have a procedure */
        typ = typ_new_void ();
      else /* function */
        typ = typ_lookup (fundec->pos, fundec->result, tenv_ptr);

      /* Get a new level for the functions body */
      temp_label     *label     = temp_new_label ();
      util_bool_list *boollist  = mk_formal_escape_list (fundec->params);
      tra_level      *new_level = tra_new_level (level_ptr, label, boollist);

      /* translate arguments, then declare it */
      typ_ty_list  *tylist   = mk_formal_ty_list (tenv_ptr, fundec->params);
      env_enventry *enventry = env_new_fun_entry (new_level,
                                                  label,
                                                  tylist,
                                                  typ);
      sym_bind_symbol (venv_ptr, fundec->name, enventry);
    }
}

static void
process_func_body (sym_table  *venv_ptr,
                   sym_table  *tenv_ptr,
                   absyn_dec  *dec_ptr,
                   temp_label *break_done)
{
  absyn_fundec_list  *fundeclist = dec_ptr->u.function;
  absyn_fundec       *fundec;
  LINKED_LIST_FOR_EACH (fundec, fundeclist)
    {
      absyn_field_list *fieldlist = fundec->params;
      typ_ty_list      *tylist    = mk_formal_ty_list (tenv_ptr,
                                                       fieldlist);
      /* Look up result of function */
      env_enventry     *func_head = sym_lookup (venv_ptr, fundec->name);
      typ_ty           *typ       = func_head->u.fun.result;

      tra_access_list *access_list = tra_formals (func_head->u.fun.level);

      /* Start new scope, then declare function parameters */
      sym_begin_scope (venv_ptr);
        absyn_field *field;
        tra_access  *access;
        typ_ty      *ty;
        for (int i = 0;
             (field = linked_list_get (fieldlist, i)) != NULL
               /* since first parameter is static link  + 1*/
               && (access = linked_list_get (access_list, i + 1)) != NULL
               && (ty = linked_list_get (tylist, i)) != NULL;
             i++)
          {
            debug_print ("Declare variable: %s\n", sym_name (field->name));
            env_enventry *enventry = env_new_var_entry (access, ty);
            sym_bind_symbol (venv_ptr, field->name, enventry);
          }

        expty *body = trans_exp (func_head->u.fun.level,
                                 venv_ptr,
                                 tenv_ptr,
                                 fundec->body,
                                 break_done);
      /* Process body and see if body and result equal */
      if (!typ_cmpty (body->ty, typ))
        {
          errm_printf (fundec->pos,
                       "Return type of function %s must equal declared",
                       sym_name (fundec->name));
        }

      sym_end_scope (venv_ptr);
      tra_add_func_frag (body->exp, func_head->u.fun.level);
    }
}

static tra_exp *
check_type_dec (tra_level *level_ptr,
                sym_table *venv_ptr,
                sym_table *tenv_ptr,
                absyn_dec *dec_ptr,
                temp_label *break_done)
{
  process_type_header (tenv_ptr, dec_ptr);
  process_type_body (tenv_ptr, dec_ptr);
  check_infinite_types (tenv_ptr, dec_ptr);

  return tra_type_dec ();
}

static void
process_type_header (sym_table  *tenv_ptr,
                     absyn_dec  *dec_ptr)
{
  absyn_name_ty_list *nametylist = dec_ptr->u.type;
  absyn_name_ty      *namety;
  LINKED_LIST_FOR_EACH (namety, nametylist)
    {
      debug_print ("namety: %s\n", sym_name (namety->name));
      /* Check for redeclaration */
      absyn_name_ty *namety2;
      for (int j = i + 1;
           (namety2 = linked_list_get (nametylist, j)) != NULL;
           j++)
        {
          if (namety->name == namety2->name)
            errm_printf (namety2->ty->pos,
                         "Illegal type redeclaration");
        }
      /* Put headers in type enviroment */
      typ_ty *typ = typ_new_name (namety->name, NULL);
      sym_bind_symbol (tenv_ptr, namety->name, typ); /* insert */
    }
}

static void
process_type_body (sym_table  *tenv_ptr,
                   absyn_dec  *dec_ptr)
{
  /* Concatenate all the bodies to the headers */
  absyn_name_ty_list *nametylist = dec_ptr->u.type;
  absyn_name_ty      *namety;
  LINKED_LIST_FOR_EACH (namety, nametylist)
    {
      /* See if type declared */
      typ_ty *typ = trans_ty (tenv_ptr, namety->ty);

      /* Look it up and then add `body` */
      typ_ty *head = sym_lookup (tenv_ptr, namety->name);
      head->u.name.ty = typ;
    }
}

static void
check_infinite_types (sym_table  *tenv_ptr,
                      absyn_dec  *dec_ptr)
{
  /* Check for infinitive recursive type declaration */
  absyn_name_ty_list *nametylist = dec_ptr->u.type;
  absyn_name_ty      *namety;
  LINKED_LIST_FOR_EACH (namety, nametylist)
    {
      typ_ty *dec_typ = sym_lookup (tenv_ptr, namety->name);
      if (typ_actual_ty (dec_typ) == dec_typ)
        errm_printf (dec_ptr->pos,
                     "Infinite recursive type %s",
                     sym_name (namety->name));

    }
}

static tra_exp *
check_var_dec (tra_level *level_ptr,
               sym_table *venv_ptr,
               sym_table *tenv_ptr,
               absyn_dec *dec_ptr,
               temp_label *break_done)
{
  expty *init;
  init = trans_exp (level_ptr,
                    venv_ptr,
                    tenv_ptr,
                    dec_ptr->u.var.init,
                    break_done);

  /* If type is given, check if is equal to init expression */
  if (dec_ptr->u.var.typ != NULL)
    {
      typ_ty *typ;
      typ = typ_lookup (dec_ptr->pos, dec_ptr->u.var.typ, tenv_ptr);

      if (!typ_cmpty (typ, init->ty))
        errm_printf (dec_ptr->u.var.init->pos, "Types do not match");
    }
  else
    {
      /* Make sure init is not NIL if no type specified */
      if (init->ty->kind == TYP_NIL)
        errm_printf (dec_ptr->u.var.init->pos,
                     "Cannot declare variable without type.");
    }

  /* Declare new varable */
  tra_access   *access   = tra_alloc_local (level_ptr, dec_ptr->u.var.escape);
  env_enventry *enventry = env_new_var_entry (access, init->ty); // Maybe NIL ?
  sym_bind_symbol (venv_ptr, dec_ptr->u.var.var, enventry);

  return tra_var_dec (access, init->exp);
}

/**
 * Checks kind of type. Calls specific function to handle type.
 * Function will lookup if the type is declared.
 *
 * @param Type enviroment.
 * @param The type to check.
 *
 * @return Struct with type information.
 */
static typ_ty*
trans_ty (sym_table *tenv_ptr,
          absyn_ty  *ty_ptr)
{
  if (ty_ptr == NULL)
    return typ_new_int ();

  switch (ty_ptr->kind)
    {
    case ABSYN_RECORD_TY:
      return check_record_ty (tenv_ptr, ty_ptr);

    case ABSYN_ARRAY_TY:
      return check_array_ty (tenv_ptr, ty_ptr);

    case ABSYN_NAME_TY:
      return check_name_ty (tenv_ptr, ty_ptr);
    }

  errm_impossible ("Got over switch in trans_ty()!\n");
  return NULL;
}

static typ_ty*
check_record_ty (sym_table *tenv_ptr,
                 absyn_ty  *ty_ptr)
{
  return typ_new_record (mk_formal_field_list (tenv_ptr, ty_ptr->u.record));
}

static typ_ty*
check_array_ty (sym_table *tenv_ptr,
                absyn_ty  *ty_ptr)
{
  return typ_new_array (typ_lookup (ty_ptr->pos, ty_ptr->u.array, tenv_ptr));
}

static typ_ty*
check_name_ty (sym_table *tenv_ptr,
               absyn_ty  *ty_ptr)
{
  typ_ty *typ = sym_lookup (tenv_ptr, ty_ptr->u.name);

  if (typ == NULL)
    {
      errm_printf (ty_ptr->pos,
                   "Type %s not declared",
                   sym_name (ty_ptr->u.name));
      typ = typ_new_int ();
    }
  return typ;
}

/*
  Check that each type in field_list is declared.
  Add it to new typ_field_list .
*/
static typ_field_list *
mk_formal_field_list (sym_table        *tenv_ptr,
                      absyn_field_list *list_ptr)
{
  typ_field_list   *tyfieldlist = linked_list_new ();
  absyn_field      *absyn_field;
  LINKED_LIST_FOR_EACH (absyn_field, list_ptr)
    {
      typ_ty *typ = sym_lookup (tenv_ptr, absyn_field->typ);
      if (typ == NULL)
        {
          errm_printf (absyn_field->pos,
                       "Type %s not declared",
                       sym_name (absyn_field->typ));
          typ = typ_new_int ();
        }

      typ_field *field = typ_new_field (absyn_field->name, typ);
      linked_list_add (tyfieldlist, field);
    }

  return tyfieldlist;
}

/*
  Check that each type in field_list is declared.
  Add it to new typ_ty_list .
*/
static typ_ty_list *
mk_formal_ty_list (sym_table        *tenv_ptr,
                   absyn_field_list *list_ptr)
{
  typ_ty_list      *tylist = linked_list_new ();
  absyn_field      *field;
  LINKED_LIST_FOR_EACH (field, list_ptr)
    {
      typ_ty *typ = sym_lookup (tenv_ptr, field->typ);
      linked_list_add (tylist, typ);
      debug_print ("field->typ: %s\n", sym_name (field->typ));
    }

  return tylist;
}

/* Creates a bool list from the escape fields of formals */
static util_bool_list *
mk_formal_escape_list (absyn_field_list *params_ptr)
{
  util_bool_list *boollist = linked_list_new ();
  absyn_field    *field;
  LINKED_LIST_FOR_EACH (field, params_ptr)
    {
      linked_list_add (boollist, &field->escape);
    }

  return boollist;
}

/*
  Checks if sym_ptr is declared till marked symbol.
  Return true on declared. If not found false.
*/
static bool
check_dec_till_mark (sym_table  *table_ptr,
                     sym_symbol *sym_ptr)
{
  if (sym_lookup_till_mark (table_ptr, sym_ptr) == NULL)
    return false;

  return true;
}

/*
  Functions to manipulate a bool list.
  Used for getting and setting status of for and while loop.
  Needed to make sure break statement is in the right position.
*/
static void
set_loop_status (bool_list **loop_stat,
                 bool        flag)
{
  *loop_stat = new_bool_list (flag, *loop_stat);
}

static void
unset_loop_status (bool_list **loop_stat)
{
  if (loop_stat == NULL)
    return;

  bool_list *new_top = (*loop_stat)->tail;
  free (*loop_stat);
  *loop_stat = new_top;
}

static bool
get_loop_status (bool_list *loop_stat)
{
  if (loop_stat == NULL)
    return false;

  return loop_stat->head;
}

static bool_list *
new_bool_list (bool       head,
               bool_list *tail_ptr)
{
  bool_list *list = new (sizeof (*list));

  list->head = head;
  list->tail = tail_ptr;

  return list;
}
/*
  Simple converting function,
  that gets the translation modules representation of an operator.
  If not found, returns zero.
 */
static TRA_OP
get_tra_op (ABSYN_OP op)
{
  switch (op)
    {
    case ABSYN_PLUS_OP:
      return TRA_PLUS;

    case ABSYN_MINUS_OP:
      return TRA_MINUS;

    case ABSYN_TIMES_OP:
      return TRA_TIMES;

    case ABSYN_DIVIDE_OP:
      return TRA_DIVIDE;

    case ABSYN_EQ_OP:
      return TRA_EQ;

    case ABSYN_NEQ_OP:
      return TRA_NEQ;

    case ABSYN_LT_OP:
      return TRA_LT;

    case ABSYN_GT_OP:
      return TRA_GT;

    case ABSYN_GE_OP:
      return TRA_GE;

    case ABSYN_LE_OP:
      return TRA_LE;
    }

  return 0;
}