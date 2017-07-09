/**
 * @file absyn.h
 * Structs and constructor declarations for abstract syntax tree.
 *
 * Global functions or structs start with 'absyn'.
 */

#ifndef _ABSYN_H_
#define _ABSYN_H_

#include <stdbool.h>

//#include "list.h"
#include "symbol.h"

typedef struct _absyn_var absyn_var;
typedef struct _absyn_exp absyn_exp;
typedef struct _absyn_dec absyn_dec;
typedef struct _absyn_ty  absyn_ty;

typedef struct _absyn_dec_list     absyn_dec_list;
typedef struct _absyn_exp_list     absyn_exp_list;
typedef struct _absyn_field        absyn_field;
typedef struct _absyn_field_list   absyn_field_list;
typedef struct _absyn_fundec       absyn_fundec;
typedef struct _absyn_fundec_list  absyn_fundec_list;
typedef struct _absyn_name_ty      absyn_name_ty;
typedef struct _absyn_name_ty_list absyn_name_ty_list;
typedef struct _absyn_efield       absyn_efield;
typedef struct _absyn_efield_list  absyn_efield_list;


struct
_absyn_dec_list
{
  absyn_dec      *head;
  absyn_dec_list *tail;
};

struct
_absyn_exp_list
{
  absyn_exp      *head;
  absyn_exp_list *tail;
};

struct
_absyn_field_list
{
  absyn_field      *head;
  absyn_field_list *tail;
};

struct
_absyn_fundec_list
{
  absyn_fundec      *head;
  absyn_fundec_list *tail;
};

struct
_absyn_name_ty_list
{
  absyn_name_ty      *head;
  absyn_name_ty_list *tail;
};

struct
_absyn_efield_list
{
  absyn_efield      *head;
  absyn_efield_list *tail;
};

/**
 * Enum to track wich operator was choosen.
 */
typedef enum
  {
    ABSYN_PLUS_OP,
    ABSYN_MINUS_OP,
    ABSYN_TIMES_OP,
    ABSYN_DIVIDE_OP,
    ABSYN_EQ_OP,
    ABSYN_NEQ_OP,
    ABSYN_LT_OP,
    ABSYN_GT_OP,
    ABSYN_GE_OP,
    ABSYN_LE_OP
  } ABSYN_OP;

/**
 * Structure to hold information about variables.
 */
struct
_absyn_var
{
  enum
    {
      ABSYN_SIMPLE_VAR,
      ABSYN_FIELD_VAR,
      ABSYN_SUBSCRIPT_VAR
    } kind;

  int pos;

  union
  {
    sym_symbol *simple;

    struct
    {
      absyn_var  *var;
      sym_symbol *sym;
    } field;

    struct
    {
      absyn_var *var;
      absyn_exp *exp;
    } subscript;
  } u;
};

/**
 * Structure to hold information about expressions.
 */
struct
_absyn_exp
{
  enum
    {
      ABSYN_VAR_EXP,
      ABSYN_NIL_EXP,
      ABSYN_INT_EXP,
      ABSYN_STR_EXP,
      ABSYN_CALL_EXP,
      ABSYN_OP_EXP,
      ABSYN_RECORD_EXP,
      ABSYN_SEQ_EXP,
      ABSYN_ASSIGN_EXP,
      ABSYN_IF_EXP,
      ABSYN_WHILE_EXP,
      ABSYN_FOR_EXP,
      ABSYN_BREAK_EXP,
      ABSYN_LET_EXP,
      ABSYN_ARRAY_EXP
    } kind;

  int pos;

  union
  {
    absyn_var *var;
    /* nil; - needs only pos */
    int        intt;
    char      *str;

    struct
    {
      sym_symbol     *func;
      absyn_exp_list *args;
    } call;

    struct
    {
      ABSYN_OP   op;
      absyn_exp *left;
      absyn_exp *right;
    } op;

    struct
    {
      sym_symbol        *typ;
      absyn_efield_list *fields;
    } record;

    absyn_exp_list *seq;

    struct
    {
      absyn_var *var;
      absyn_exp *exp;
    } assign;

    struct
    {
      absyn_exp *test;
      absyn_exp *then;
      absyn_exp *elsee;
    } iff;

    struct
    {
      absyn_exp *test;
      absyn_exp *body;
    } whilee;

    struct
    {
      sym_symbol *var;
      absyn_exp  *lo;
      absyn_exp  *hi;
      absyn_exp  *body;
      bool              escape;
    } forr;
    /* breakk; - need only pos */
    struct
    {
      absyn_dec_list *decs;
      absyn_exp      *body;
    } let;

    struct
    {
      sym_symbol *typ;
      absyn_exp  *size;
      absyn_exp  *init;
    } array;
  } u;
};

/**
 * Structure to hold information about declarations.
 */
struct
_absyn_dec
{
  enum
    {
      ABSYN_FUNCTION_DEC,
      ABSYN_VAR_DEC,
      ABSYN_TYPE_DEC
    } kind;

  int pos;

  union
  {
    absyn_fundec_list *function;
    /* escape may change after initial declaration */
    struct
    {
      sym_symbol *var;
      sym_symbol *typ;
      absyn_exp  *init;
      bool        escape;
    } var;

    absyn_name_ty_list *type;
  } u;
};

/**
 * Structure to hold information about data types
 */
struct
_absyn_ty
{
  enum
    {
      ABSYN_NAME_TY,
      ABSYN_RECORD_TY,
      ABSYN_ARRAY_TY
    } kind;

  int pos;

  union
  {
    sym_symbol       *name;
    absyn_field_list *record;
    sym_symbol       *array;
  } u;
};

/* Nodes of lists */

struct
_absyn_field
{
  sym_symbol *name;
  sym_symbol *typ;
  int         pos;
  bool        escape;
};

struct
_absyn_fundec
{
  int               pos;
  sym_symbol       *name;
  absyn_field_list *params;
  sym_symbol       *result;
  absyn_exp        *body;
};

struct
_absyn_name_ty
{
  sym_symbol *name;
  absyn_ty   *ty;
};

struct
_absyn_efield
{
  sym_symbol *name;
  absyn_exp  *exp;
};

/* Function prototypes */

absyn_var* absyn_new_simple_var    (int         pos,
                                    sym_symbol *sym_ptr);

absyn_var* absyn_new_field_var     (int         pos,
                                    absyn_var  *var_ptr,
                                    sym_symbol *sym_ptr);

absyn_var* absyn_new_subscript_var (int        pos,
                                    absyn_var *var_ptr,
                                    absyn_exp *exp_ptr);

absyn_exp* absyn_new_var_exp       (int        pos,
                                    absyn_var *var_ptr);

absyn_exp* absyn_new_nil_exp       (int pos);

absyn_exp* absyn_new_int_exp       (int pos,
                                    int i);

absyn_exp* absyn_new_str_exp       (int   pos,
                                    char *s_ptr);

absyn_exp* absyn_new_call_exp      (int             pos,
                                    sym_symbol     *func_ptr,
                                    absyn_exp_list *args_ptr);

absyn_exp* absyn_new_op_exp        (int        pos,
                                    ABSYN_OP   op,
                                    absyn_exp *left_ptr,
                                    absyn_exp *right_ptr);

absyn_exp* absyn_new_record_exp    (int                pos,
                                    sym_symbol        *typ_ptr,
                                    absyn_efield_list *fields_ptr);

absyn_exp* absyn_new_seq_exp       (int             pos,
                                    absyn_exp_list *seq_ptr);

absyn_exp* absyn_new_assign_exp    (int        pos,
                                    absyn_var *var_ptr,
                                    absyn_exp *exp_ptr);

absyn_exp* absyn_new_if_exp        (int        pos,
                                    absyn_exp *test_ptr,
                                    absyn_exp *then_ptr,
                                    absyn_exp *elsee_ptr);

absyn_exp* absyn_new_while_exp     (int        pos,
                                    absyn_exp *test_ptr,
                                    absyn_exp *body_ptr);

absyn_exp* absyn_new_for_exp       (int         pos,
                                    sym_symbol *var_ptr,
                                    absyn_exp  *lo_ptr,
                                    absyn_exp  *hi_ptr,
                                    absyn_exp  *body_ptr);

absyn_exp* absyn_new_break_exp     (int pos);

absyn_exp* absyn_new_let_exp       (int             pos,
                                    absyn_dec_list *decs_ptr,
                                    absyn_exp      *body_ptr);

absyn_exp* absyn_new_array_exp     (int         pos,
                                    sym_symbol *typ_ptr,
                                    absyn_exp  *size_ptr,
                                    absyn_exp  *init_ptr);

absyn_dec* absyn_new_function_dec  (int                pos,
                                    absyn_fundec_list *func_ptr);

absyn_dec* absyn_new_var_dec       (int         pos,
                                    sym_symbol *var_ptr,
                                    sym_symbol *typ_ptr,
                                    absyn_exp  *init_ptr);

absyn_dec* absyn_new_type_dec      (int                 pos,
                                    absyn_name_ty_list *type_ptr);

absyn_ty*  absyn_new_name_ty       (int         pos,
                                    sym_symbol *name_ptr);

absyn_ty*  absyn_new_record_ty     (int               pos,
                                    absyn_field_list *record_ptr);

absyn_ty*  absyn_new_array_ty      (int         pos,
                                    sym_symbol *array_ptr);



absyn_field*        absyn_new_field       (int         pos,
                                           sym_symbol *name_ptr,
                                           sym_symbol *typ_ptr);

absyn_fundec*       absyn_new_fundec      (int               pos,
                                           sym_symbol       *name_ptr,
                                           absyn_field_list *params_ptr,
                                           sym_symbol       *result_ptr,
                                           absyn_exp        *body_ptr);

absyn_name_ty*      absyn_new_namety      (sym_symbol *name_ptr,
                                           absyn_ty   *ty_ptr);

absyn_efield*       absyn_new_efield      (sym_symbol *name_ptr,
                                           absyn_exp  *exp_ptr);



absyn_efield_list * absyn_new_efield_list (absyn_efield      *head,
                                           absyn_efield_list *tail);

absyn_name_ty_list * absyn_new_name_ty_list (absyn_name_ty      *head,
                                             absyn_name_ty_list *tail);

absyn_fundec_list * absyn_new_fundec_list (absyn_fundec      *head,
                                           absyn_fundec_list *tail);

absyn_field_list * absyn_new_field_list (absyn_field      *head,
                                         absyn_field_list *tail);

absyn_exp_list * absyn_new_exp_list (absyn_exp      *head,
                                     absyn_exp_list *tail);

absyn_dec_list * absyn_new_dec_list (absyn_dec      *head,
                                     absyn_dec_list *tail);

#endif
