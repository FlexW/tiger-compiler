/**
 * @file symbol.c
 * Wrapper functions to insert a symbol in table (table.c).
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/debug.h"
#include "include/util.h"
#include "include/symbol.h"
#include "include/table.h"

#define SIZE 109 /* should be prime */

sym_symbol  marksym = {"<mark>", NULL};

static sym_symbol *hashtable[SIZE];


static sym_symbol*  new_symbol (char       *name_ptr,
                               sym_symbol *next_ptr);

static unsigned int hash       (char *s0_ptr);


/**
 * Creates new symbol. If Symbol is already there it creates not a new object.
 *
 * @param name_ptr Name of the new symbol.
 *
 * @return The new symbol.
 */
sym_symbol*
sym_new_symbol (char* name_ptr)
{
  int         index = hash (name_ptr) % SIZE;
  sym_symbol *syms;
  sym_symbol *sym;

  syms  = (sym_symbol*)hashtable[index];

  for (sym = syms; sym != NULL; sym = sym->next)
    {
    if (!strcmp (sym->name, name_ptr))
      return sym;
    }

  sym              = new_symbol (name_ptr,syms);
  hashtable[index] = sym;

  return sym;
}

/**
 * Returns the name of a symbol.
 *
 * @param sym_ptr Symbol.
 *
 * @return Name of symbol.
 */
char*
sym_name (sym_symbol *sym_ptr)
{
  assert (sym_ptr);
  return sym_ptr->name;
}

/**
 * Creates a new empty table.
 *
 * @return New empty table.
 */
sym_table*
sym_new_table (void)
{
  return (sym_table*)tab_new_table ();
}

/**
 * Binds new symbol in table.
 *
 * @param table_ptr Table where symbol should binded.
 * @param sym_ptr   Symbol to bind.
 * @param value_ptr Value that should map to symbol.
 */
void
sym_bind_symbol (sym_table  *table_ptr,
                 sym_symbol *sym_ptr,
                 void       *value_ptr)
{
  tab_bind_value ((tab_table*)table_ptr, sym_ptr, value_ptr);
}

/**
 * Looks up a value that maps to a symbol in table.
 *
 * @param table_ptr Table where symbol lies.
 * @param sym_ptr   Symbol to find.
 *
 * @return Value found. If not found NULL.
 */
void*
sym_lookup (sym_table *table_ptr,
            sym_symbol *sym_ptr)
{
  return tab_lookup ((tab_table*)table_ptr, sym_ptr);
}

/**
 * Starts new scope by marking the current point in table with special sign.
 *
 * @param table_ptr Table to mark.
 */
void
sym_begin_scope (sym_table *table_ptr)
{
  sym_bind_symbol (table_ptr, &marksym, NULL);
}

/**
 * Drops all symbols from table till special sign.
 *
 * @param table_ptr Table to drop symbols.
 */
void
sym_end_scope (sym_table *table_ptr)
{
  sym_symbol *s;

  do
    s = tab_pop ((tab_table*)table_ptr);
  while (s != &marksym);
}

/**
 * Dumps every entry in table.
 *
 * See tab_dump() in table.c.
 */
void
sym_dump (sym_table *table_ptr,
          void (*show)(sym_symbol *sym_ptr,
                       void *binding))
{
  tab_dump ((tab_table*)table_ptr, (void (*)(void*, void*)) show);
}

/**
 * See tab_lookup_till_mark() in table.c
 */
void*
sym_lookup_till_mark (sym_table  *table_ptr,
                      sym_symbol *key_ptr)
{
  return tab_lookup_till_mark ((tab_table*)table_ptr, key_ptr, &marksym);
}

/**
 * Generates hash value of given char.
 *
 * @param s0_ptr Key to generate hash value.
 */
static unsigned int
hash (char *s0_ptr)
{
  unsigned int h = 0;
  char        *s;

  for (s = s0_ptr; *s; s++)
       h = h*65599 + *s;

 return h;
}

static sym_symbol*
new_symbol (char       *name_ptr,
            sym_symbol *next_ptr)
{
  sym_symbol *s = new (sizeof (*s));

  s->name = name_ptr;
  s->next = next_ptr;

  return s;
}
