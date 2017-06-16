/**
 * @file env.c
 * Constructur functions for enviroment variables, that get saved in hash table.
 */

#include "include/symbol.h"
#include "include/util.h"
#include "include/env.h"


env_enventry*
env_new_var_entry (tra_access *access_ptr,
                   typ_ty     *typ_ptr)
{
  env_enventry *varentry = new (sizeof (*varentry));

  varentry->kind         = ENV_VAR_ENTRY;
  varentry->u.var.ty     = typ_ptr;
  varentry->u.var.access = access_ptr;

  return varentry;
}

env_enventry*
env_new_fun_entry (tra_level   *level_ptr,
                   temp_label  *label_ptr,
                   typ_ty_list *formals_ptr,
                   typ_ty      *result_ptr)
{
  env_enventry *funentry = new (sizeof (*funentry));

  funentry->kind          = ENV_FUN_ENTRY;
  funentry->u.fun.level   = level_ptr;
  funentry->u.fun.label   = label_ptr;
  funentry->u.fun.formals = formals_ptr;
  funentry->u.fun.result  = result_ptr;

  return funentry;
}

sym_table*
env_base_tenv (void)
{
  sym_table *tenv;
  tenv = sym_new_table ();

  sym_bind_symbol (tenv, sym_new_symbol("int"), typ_new_int ());
  sym_bind_symbol (tenv, sym_new_symbol("string"), typ_new_str ());

  return tenv;
}

sym_table*
env_base_venv (void)
{
  return sym_new_table ();
}
