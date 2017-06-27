/**
 * @file table.c
 * Functions to manipulate generic tables.
 */

#include <stdio.h>
#include <assert.h>

#include "include/util.h"
#include "include/table.h"

#define TAB_SIZE 371

typedef struct _binder binder;

/**
 * Represents a binding in the table.
 */
struct _binder
{
  void   *key;
  void   *value;
  binder *next;
  void   *prev_top;
};

/**
 * Structure that holds the generic table.
 */
struct _tab_table
{
  binder *table[TAB_SIZE];
  void   *top;
};

static binder* new_binding (void *key_ptr,
                            void *value_ptr,
                            binder *next_ptr,
                            void *prev_top_ptr);

tab_table*
tab_new_table (void)
{
  tab_table *t = new (sizeof (*t));

  t->top = NULL;
  for (int i = 0; i < TAB_SIZE; i++)
    {
      t->table[i] = NULL;
    }

  return t;
}

/**
 * Binds a value in the table.
 *
 * The cast from pointer to integer in the expression
 *   ((unsigned)key) % TABSIZE
 * may lead to a warning message.  However, the code is safe,
 * and will still operate correctly.  This line is just hashing
 * a pointer value into an integer value, and no matter how the
 * conversion is done, as long as it is done consistently, a
 * reasonable and repeatable index into the table will result.
 *
 * @param table_ptr  Pointer to table where the binding should be added.
 * @param key_ptr    Pointer to a key that shoud be assigned to value.
 * @param value _ptr The Value that should be binded in the table.
 */
void
tab_bind_value (tab_table *table_ptr,
                void      *key_ptr,
                void      *value_ptr)
{
  int index;

  assert (table_ptr && key_ptr);

  index                   = ((unsigned)key_ptr) % TAB_SIZE;
  table_ptr->table[index] = new_binding (key_ptr,
                                         value_ptr,
                                         table_ptr->table[index],
                                         table_ptr->top);
  table_ptr->top          = key_ptr;
}

/**
 * Lookup a value in table.
 *
 * @param table_ptr Table in that should be looked up.
 * @param key_ptr   Key that matches the value.
 *
 * @return Value that maps to key. If not found NULL.
 */
void*
tab_lookup (tab_table *table_ptr,
            void      *key_ptr)
{
  int    index;
  binder *b;

  assert (table_ptr && key_ptr);

  index = ((unsigned)key_ptr) % TAB_SIZE;
  for(b = table_ptr->table[index]; b != NULL; b = b->next)
    {
      if (b->key == key_ptr)
        return b->value;
    }

  return NULL;
}

/**
 * Removes the last element from table.
 *
 * @param table_ptr Table where element should popped.
 *
 * @return Removed element.
 */
void*
tab_pop (tab_table *table_ptr)
{
  void   *k;
  binder *b;
  int     index;

  assert (table_ptr);

  k = table_ptr->top;
  assert (k);

  index = ((unsigned)k) % TAB_SIZE;
  b     = table_ptr->table[index];
  assert(b);

  table_ptr->table[index] = b->next;
  table_ptr->top          = b->prev_top;

  return b->key;
}

/**
 * Goes through the complete table. Calls the given function (show_ptr),
 *
 * @param table_ptr Table to show.
 * @param show_ptr  Function that gets called on each entry.
 */
void
tab_dump (tab_table *table_ptr,
          void     (*show_ptr)(void *key_ptr,
                               void *value_ptr))
{
  void   *k     = table_ptr->top;
  int     index = ((unsigned)k) % TAB_SIZE;
  binder *b     = table_ptr->table[index];

  if (b == NULL)
    return;

  table_ptr->table[index] = b->next;
  table_ptr->top          = b->prev_top;

  show_ptr (b->key, b->value);

  tab_dump (table_ptr, show_ptr);

  assert (table_ptr->top == b->prev_top && table_ptr->table[index] == b->next);

  table_ptr->top          = k;
  table_ptr->table[index] = b;
}

/**
 * Looks up a symbol till a special mark.
 *
 * @param table_ptr The table where to lookup.
 * @param key_ptr   The key to search.
 * @param mark_ptr  The mark to stop.
 *
 * @return NULL if not found or mark reached. key_ptr if found.
 */
void*
tab_lookup_till_mark (tab_table *table_ptr,
                      void      *key_ptr,
                      void      *mark_ptr)
{
  void   *res;
  void   *k     = table_ptr->top;
  int     index = ((unsigned)k) % TAB_SIZE;
  binder *b     = table_ptr->table[index];

  if (b == NULL)
    return NULL;

  table_ptr->table[index] = b->next;
  table_ptr->top          = b->prev_top;

  if (b->key == mark_ptr)
    {
      table_ptr->top          = k;
      table_ptr->table[index] = b;

      return NULL;
    }
  else if (b->key == key_ptr)
    {
      table_ptr->top          = k;
      table_ptr->table[index] = b;

      return key_ptr;
    }

  res = tab_lookup_till_mark (table_ptr, key_ptr, mark_ptr);

  table_ptr->top          = k;
  table_ptr->table[index] = b;

  return res;
}

static binder*
new_binding (void *key_ptr,
             void *value_ptr,
             binder *next_ptr,
             void *prev_top_ptr)
{
  binder *b = new (sizeof (*b));

  b->key      = key_ptr;
  b->value    = value_ptr;
  b->next     = next_ptr;
  b->prev_top = prev_top_ptr;

  return b;
}
