/**
 * @file types.h
 * Typ definition for entering types into enviroments.
 *
 * Global functions and variables start with typ_ .
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdbool.h>

#include "symbol.h"


typedef struct _typ_ty         typ_ty;
typedef struct _typ_ty_list    typ_ty_list;
typedef struct _typ_field      typ_field;
typedef struct _typ_field_list typ_field_list;

struct
_typ_ty_list
{
  typ_ty *head;
  typ_ty_list *tail;
};

struct
_typ_field_list
{
  typ_field *head;
  typ_field_list *tail;
};

/**
 * Structure to hold information about a type.
 */
struct
_typ_ty
{
  enum
    {
      TYP_RECORD,
      TYP_NIL,
      TYP_INT,
      TYP_STRING,
      TYP_ARRAY,
      TYP_NAME,
      TYP_VOID
    } kind;

  union
  {
    typ_field_list *record;
    typ_ty         *array;
    struct {sym_symbol *sym; typ_ty *ty;} name;
  } u;
};

struct
_typ_field
{
  sym_symbol *name;
  typ_ty     *ty;
};


/* Function prototypes */

typ_ty*         typ_new_nil        (void);

typ_ty*         typ_new_int        (void);

typ_ty*         typ_new_str        (void);

typ_ty*         typ_new_void       (void);

typ_ty*         typ_new_record     (typ_field_list *fields_ptr);

typ_ty*         typ_new_array      (typ_ty *ty_ptr);

typ_ty*         typ_new_name       (sym_symbol *sym_ptr,
                                    typ_ty     *ty_ptr);

typ_field*      typ_new_field      (sym_symbol *name_ptr,
                                    typ_ty     *ty_ptr);

void            typ_print_ty       (typ_ty *ty_ptr);

void            typ_print_ty_list  (typ_ty_list *list_ptr);

bool            typ_cmpty          (typ_ty *typ1_ptr,
                                    typ_ty *typ2_ptr);

typ_ty*         typ_lookup         (int         pos,
                                    sym_symbol *sym_ptr,
                                    sym_table  *tenv_ptr);

typ_ty*         typ_actual_ty      (typ_ty *typ_ptr);

typ_ty_list * typ_new_ty_list (typ_ty *head,
                               typ_ty_list *tail);

typ_field_list * typ_new_field_list (typ_field *head,
                                     typ_field_list *tail);

#endif /* _TYPES_H_ */
