/**
 * @file env.h
 * Functions and structs to insert Enviroments in table.
 * Global functions and variables start with env_ .
 */

#ifndef _ENV_H_
#define _ENV_H_

#include "symbol.h"
#include "translate.h"
#include "types.h"

typedef struct _env_enventry env_enventry;

struct
_env_enventry
{
  enum
    {
      ENV_VAR_ENTRY,
      ENV_FUN_ENTRY
    } kind;

  union
  {
    struct
    {
      tra_access *access;
      typ_ty     *ty;
    } var;

    struct
    {
      tra_level   *level;
      temp_label  *label;
      typ_ty_list *formals;
      typ_ty      *result;
    } fun;
  } u;
};

env_enventry* env_new_var_entry (tra_access *access_ptr,
                                 typ_ty     *typ_ptr);

env_enventry* env_new_fun_entry (tra_level   *level_ptr,
                                 temp_label  *label_ptr,
                                 typ_ty_list *formals_ptr,
                                 typ_ty      *result_ptr);

sym_table* env_base_tenv (void); /* typ_ty enviroment */

sym_table* env_base_venv (void); /* env_enventry enviroment */


#endif
