/**
 * @file types.c
 *
 * Constructor functions for typ structures.
 */

#include <stdio.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/errormsg.h"
#include "include/types.h"


static typ_ty nil_ty  = {.kind = TYP_NIL};
static typ_ty int_ty  = {.kind = TYP_INT};
static typ_ty str_ty  = {.kind = TYP_STRING};
static typ_ty void_ty = {.kind = TYP_VOID};

static char strty[][12] = {
   "ty_record", "ty_nil", "ty_int", "ty_string",
   "ty_array", "ty_name", "ty_void"};

typ_ty*
typ_new_nil (void)
{
  return &nil_ty;
}

typ_ty*
typ_new_int (void)
{
  return &int_ty;
}

typ_ty*
typ_new_str (void)
{
  return &str_ty;
}

typ_ty*
typ_new_void (void)
{
  return &void_ty;
}

typ_ty*
typ_new_record (typ_field_list *fields_ptr)
{
  typ_ty *t = new (sizeof (*t));

  t->kind   = TYP_RECORD;
  t->u.record = fields_ptr;

  return t;
}

typ_ty*
typ_new_array (typ_ty *ty_ptr)
{
  typ_ty *t = new (sizeof (*t));

  t->kind   = TYP_ARRAY;
  t->u.array = ty_ptr;

  return t;
}

typ_ty*
typ_new_name (sym_symbol *sym_ptr,
              typ_ty     *ty_ptr)
{
  typ_ty *t = new (sizeof (*t));

  t->kind        = TYP_NAME;
  t->u.name.sym  = sym_ptr;
  t->u.name.ty   = ty_ptr;

  return t;
}

typ_field*
typ_new_field (sym_symbol *name_ptr,
               typ_ty     *ty_ptr)
{
  typ_field *t = new (sizeof (*t));

  t->name = name_ptr;
  t->ty   = ty_ptr;

  return t;
}

/**
 * Compares to types. If equal return true else false.
 * Record type and nil type are matching types.
 *
 * @param typ1_ptr Type to compare.
 * @param typ2_ptr Type to compare.
 *
 * @return True on equal. False on not equal.
 */
bool
typ_cmpty (typ_ty *typ1_ptr,
           typ_ty *typ2_ptr)
{
  typ1_ptr = typ_actual_ty (typ1_ptr);
  typ2_ptr = typ_actual_ty (typ2_ptr);

  if (typ1_ptr == typ2_ptr)
    return true;

  if (typ1_ptr->kind == TYP_RECORD && typ2_ptr->kind == TYP_NIL)
    return true;
  if (typ1_ptr->kind == TYP_NIL && typ2_ptr->kind == TYP_RECORD)
    return true;

  return false;
}

/**
 * Looks up a symbol in type enviroment. Returns type.
 * Throws error message if not present.
 *
 * @param pos      The position in the source code.
 * @param sym_ptr  The symbol (name) to lookup.
 * @param tenv_ptr The enviroment (table) to lookup.
 *
 * @return The looked up type. May be NULL if not present.
 */
typ_ty*
typ_lookup (int         pos,
            sym_symbol *sym_ptr,
            sym_table  *tenv_ptr)
{
  typ_ty *typ;

  typ = sym_lookup (tenv_ptr, sym_ptr);
  if (typ == NULL)
    {
      errm_printf (pos, "type %s not declared", sym_name (sym_ptr));
      return typ;
    }

  return typ_actual_ty (typ);
}

/**
 * Looks up the type of a type.
 * In many cases the type is the type. But in case of a name type, it looks
 * up the 'actual' type.
 *
 * @param typ_ptr The type where the `actual` type is needed
 *
 * @return The type
 */
typ_ty*
typ_actual_ty (typ_ty *typ_ptr)
{
  typ_ty *original = typ_ptr;
  int max          = 1000;
  int counter      = 0;

  while (typ_ptr->kind == TYP_NAME)
    {
      typ_ptr = typ_ptr->u.name.ty;
      counter++;

      if (counter >= max)
        return original;
    }

  return typ_ptr;
}

typ_ty_list *
typ_new_ty_list (typ_ty *head,
                 typ_ty_list *tail)
{
  typ_ty_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

typ_field_list *
typ_new_field_list (typ_field *head,
                    typ_field_list *tail)
{
  typ_field_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

/*
  Printing functions for debugging.
 */

/* This will infinite loop on mutally recursive types */
void
typ_print_ty (typ_ty *ty_ptr)
{
  if (ty_ptr == NULL)
    {
      printf ("null");
    }
  else
    {
      printf ("%s", strty[ty_ptr->kind]);

      if (ty_ptr->kind == TYP_NAME)
        printf (", %s", sym_name(ty_ptr->u.name.sym));
    }
}

void
typ_print_ty_list (typ_ty_list *list_ptr)
{
  if (list_ptr == NULL)
    {
      printf ("null");
    }
  else
    {
      printf ("ty_list( ");
      typ_print_ty (list_ptr->head);
      printf (", ");
      typ_print_ty_list (list_ptr->tail);
      printf (")");
    }
}
