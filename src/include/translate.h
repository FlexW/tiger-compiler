/**
 * @file translate.h
 * Handels function nesting and local variables for semantic analyse.
 */

#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_

#include <stdbool.h>

#include "list.h"
#include "frame.h"
#include "temp.h"
#include "util.h"


typedef struct _tra_exp    tra_exp;
typedef        list        tra_exp_list;
typedef struct _tra_level  tra_level;
typedef struct _tra_access tra_access;
typedef        list        tra_access_list;

typedef enum
  {
    TRA_PLUS,
    TRA_MINUS,
    TRA_TIMES,
    TRA_DIVIDE,
    TRA_LT,
    TRA_LE,
    TRA_GT,
    TRA_GE,
    TRA_EQ,
    TRA_NEQ
  } TRA_OP;


tra_exp *         tra_simple_var      (tra_access*,
                                       tra_level*);

tra_exp *         tra_field_var       (tra_exp*,
                                       int);

tra_exp *         tra_int_exp         (int);

tra_exp *         tra_subscript_var   (tra_exp*,
                                       tra_exp*);

tra_exp *         tra_arithmetic_exp  (tra_exp*,
                                       tra_exp*,
                                       TRA_OP);

tra_exp *         tra_conditional_exp (tra_exp *,
                                       tra_exp *,
                                       TRA_OP);

tra_exp *         tra_if_exp          (tra_exp*,
                                       tra_exp*,
                                       tra_exp*);

tra_exp *         tra_array_exp       (tra_exp* size_ptr,
                                       tra_exp* init_ptr);

tra_exp *         tra_record_exp      (tra_exp_list *list_ptr);

tra_exp *         tra_while_exp       (tra_exp    *test_ptr,
                                       tra_exp    *body_ptr,
                                       temp_label *done_ptr);

tra_exp *         tra_break_exp       (temp_label *done_ptr);

tra_exp *         tra_assign_exp      (tra_exp *container_ptr,
                                       tra_exp *alloc_ptr);

tra_exp *         tra_seq_exp         (tra_exp_list *list_ptr);

tra_exp *         tra_call_exp        (temp_label   *fun_ptr,
                                       tra_exp_list *exp_list_ptr,
                                       tra_level    *fun_dec_level_ptr,
                                       tra_level    *fun_call_level_ptr);

tra_exp *         tra_for_exp         (tra_exp *low_ptr,
                                       tra_exp *hi_ptr,
                                       tra_exp *body_ptr);

tra_exp *         tra_nil_exp         (void);

tra_exp *         tra_str_exp         (char *str);

tra_exp *         tra_let_exp         (tra_exp_list *list,
                                       tra_exp      *body);

tra_exp *         tra_var_dec         (tra_access *access_ptr,
                                       tra_exp    *init_ptr);

tra_exp *         tra_type_dec        (void);

tra_exp *         tra_func_dec        (void);

void              tra_add_func_frag   (tra_exp   *body_ptr,
                                       tra_level *level_ptr);

tra_level *       tra_outermost_level (void);

tra_level *       tra_new_level       (tra_level      *parent_ptr,
                                       temp_label     *name_ptr,
                                       util_bool_list *formals_ptr);

tra_access_list * tra_formals         (tra_level *level_ptr);

tra_access *      tra_alloc_local     (tra_level *level_ptr,
                                       bool       escape);

void              tra_proc_entry_exit (tra_level       *level_ptr,
                                       tra_exp         *body_ptr,
                                       tra_access_list *formals_ptr);

frm_frag_list *   tra_get_result      (void);

frm_frag_list *   tra_get_frag_list   (void);


#endif /* _TRANSLATE_H_ */
