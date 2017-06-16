/**
 * @file: symbol.h
 * Functions to save a symbol (variable).
 *
 * Helper (wrapper) functions to insert symbol in hash table.
 */

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "table.h"

typedef struct _sym_symbol sym_symbol;
typedef struct tab_table   sym_table;


struct _sym_symbol
{
  char       *name;
  sym_symbol *next;
};

sym_symbol* sym_new_symbol       (char *sym_ptr);

char*       sym_name             (sym_symbol *sym_ptr);

sym_table*  sym_new_table        (void);

void        sym_bind_symbol      (sym_table  *table_ptr,
                                  sym_symbol *sym_ptr,
                                  void       *value_ptr);

void*       sym_lookup           (sym_table  *table_ptr,
                                  sym_symbol *sym_ptr);

void        sym_begin_scope      (sym_table *table_ptr);

void        sym_end_scope        (sym_table *table_ptr);

void*       sym_lookup_till_mark (sym_table  *table_ptr,
                                  sym_symbol *key_ptr);
#endif
