/**
 * @file table.h
 * Generic hash table.
 *
 * No algorithm should use these functions directly, because
 * programming with void* is too error-prone.  Instead,
 * each module should make "wrapper" functions that take
 * well-typed arguments and call the tab_ functions.
 */

#ifndef _TABLE_H_
#define _TABLE_H_

typedef struct _tab_table tab_table;

tab_table* tab_new_table        (void);

void       tab_bind_value       (tab_table *table_ptr,
                                 void      *key_ptr,
                                 void      *value_ptr);

void*      tab_lookup           (tab_table *table_ptr,
                                 void      *key_ptr);

void*      tab_pop              (tab_table *table_ptr);

void       tab_dump             (tab_table *table_ptr,
                                 void     (*show_ptr)(void *key_ptr,
                                                      void *value_ptr));

void*      tab_lookup_till_mark (tab_table *table_ptr,
                                 void      *key_ptr,
                                 void      *mark_ptr);
#endif
