/**
 * @file linked_list.h
 * Functions to create and operate on generic linked list.
 */

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdbool.h>

typedef struct _linked_list linked_list;

linked_list * linked_list_new    (void);

int           linked_list_add    (linked_list *list,
                                  void        *element);

int           linked_list_length (linked_list *list);

void *        linked_list_get    (linked_list *list,
                                  int          index);

int           linked_list_free   (linked_list *list);

bool          linked_list_remove (linked_list *list,
                                  int          index);

bool          linked_list_insert (linked_list *list,
                                  int          index,
                                  void        *element);

int           linked_list_cat_list (linked_list *dst,
                                   linked_list *src);

int           linked_list_catn_list (linked_list *dst,
                                     linked_list *src,
                                     int          src_index);
/**
 * DEPRECATED
 * Macro to help with for each loops.
 * First call LINKED_LIST_FOR_BEGIN then put the logic.
 * At the end call LINKED_LIST_FOR_END.
 *
 * @param item Variable where the element gets saved.
 * @param list List to process.
 */
#define LINKED_LIST_FOR_BEGIN(item, list)         \
  int len = linked_list_length ((list));          \
  for (int i  = 0; i < len; i++)                  \
    {                                             \
      (item) = linked_list_get ((list), i);

#define LINKED_LIST_FOR_END }

/**
 * Macro to emulate for each loops.
 * LINKED_LIST_FOR_EACH (item, list)
 *  {
 *    Operations go here
 *  }
 *
 * @param item Variable where the element gets saved.
 * @param list List to process.
 */
#define LINKED_LIST_FOR_EACH(item, list)               \
  int len = linked_list_length ((list));               \
  (item) = linked_list_get ((list), 0);                \
  for (int i = 0;                                      \
       i < len;                                        \
       i++, (item) = linked_list_get ((list), i))

#endif /* _LINKED_LIST_H_ */
