/**
 * @file linked_list.c
 * Generic linked list.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "include/util.h"
#include "include/linked_list.h"

typedef struct _node node;

struct
_linked_list
{
  int   length;
  node *first;
  node *last;
};

struct _node
{
  node *prev;
  node *next;
  void *element;
};

static node *         new_node         (node *next,
                                        node *prev,
                                        void *element);

static void          for_each          (linked_list *list,
                                        bool operate (void*));

static node *        get_node          (linked_list *list,
                                        int          index);

static bool          free_node         (node *node);

static int           add_first_element (linked_list *list,
                                        void        *element);

static void          increment_length  (linked_list *list);

static void          decrement_length  (linked_list *list);

static bool          check_borders     (linked_list *list,
                                        int          index);


linked_list *
linked_list_new (void)
{
  linked_list *list = new (sizeof (*list));

  list->length = 0;
  list->first  = NULL;
  list->last   = NULL;

  return list;
}

/**
 * Gets the last element of a list.
 *
 * @param list Linked list.
 *
 * @return Last element.
 */
void *
linked_list_first (linked_list *list)
{
  return list->last;
}

/**
 * Adds a element to the given linked list.
 *
 * @param list    Linked list.
 * @param element Element to add.
 *
 * @return Index of added element.
 */
int
linked_list_add (linked_list *list,
                 void        *element)
{
  if (linked_list_length (list) == 0)
    return add_first_element (list, element);

  node *last_node = list->last;
  node *node      = new_node (NULL, last_node, element);
  last_node->next = node;
  list->last      = node;

  increment_length (list);

  return linked_list_length (list);
}

/**
 * Gets the length of linked list.
 *
 * @param list List.
 *
 * @return Length.
 */
int
linked_list_length (linked_list *list)
{
  return list->length;
}

/**
 * Gets the element at index 'index' of given linked list.
 *
 * @param index Index of element.
 * @param list  Linked list.
 *
 * @return Element, if not found NULL.
 */
void *
linked_list_get (linked_list *list,
                 int          index)
{
  node *node = get_node (list, index);

  if (node != NULL)
    return node->element;

  return NULL;
}

/**
 * Removes a node from given linked list.
 * Caution: Elements get not free.
 *
 * @param list  Linked list.
 * @param index Index of element to remove.
 *
 * @return If removed true.
 */
bool
linked_list_remove (linked_list *list,
                    int          index)
{
  node *node = get_node (list, index);

  if (node == NULL)
    return false;

  if (node->next != NULL)
    node->next->prev = node->prev;
  else /* It is the last element */
    list->last = node->prev;

  if (node->prev != NULL)
    node->prev->next = node->next;
  else /* It is the first element */
    list->first = node->next;

  if (free_node (node))
    {
      decrement_length (list);
      return true;
    }
  return false;
}

/**
 * Inserts a element into list. If index out of range. Element gets added.
 *
 * @param list    List.
 * @param index   Index.
 * @param element Element to add.
 *
 * @return If insert true.
 */
bool
linked_list_insert (linked_list *list,
                    int          index,
                    void        *element)
{
  node *onode = get_node (list, index);

  if (onode == NULL)
    {
      linked_list_add (list, element);
      return true;
    }
  node *nnode = new_node (onode, onode->prev, element);

  if (onode->prev != NULL)
    onode->prev->next = nnode;
  else /* New first element */
    list->first = nnode;

  onode->prev = nnode;

  increment_length (list);

  return true;
}

/**
 * Frees the complete list.
 *
 * @param list List to free.
 *
 * @return Number of elements removed.
 */
int
linked_list_free (linked_list *list)
{
  node *next;
  node *node = list->first;
  int   cnt  = 0;

  while (node != NULL)
    {
      next = node->next;
      free_node (node);
      node = next;
      cnt++;
    }

  free (list);
  return cnt;
}

/**
 * Concatenate list src to list dst.
 *
 * @param List where elements should be added.
 * @param List that should be added.
 *
 * @returns Number of elements added.
 */
int
linked_list_cat_list (linked_list *dst,
                      linked_list *src)
{
  int cnt = 0;

  void *element;
  LINKED_LIST_FOR_EACH (element, src)
    {
      linked_list_add (dst, element);
      cnt++;
    }

  return cnt;
}

/**
 * Concatenate list src from src_index to list dst.
 *
 * @param List where elements should be added.
 * @param List that should be added.
 * @param Index at wich element the list should be added.
 *
 * @returns Number of elements added.
 */
int
linked_list_catn_list (linked_list *dst,
                       linked_list *src,
                       int          src_index)
{
  int cnt = 0;

  if (src == NULL)
    assert(0);

  int len = linked_list_length (src);
  for (int i = src_index; i < len; i++)
    {
      void *element = linked_list_get (src, i);
      linked_list_add (dst, element);
      cnt++;
    }

  return cnt;
}

static node *
new_node (node *next,
          node *prev,
          void *element)
{
  node *node = new (sizeof (*node));

  node->next    = next;
  node->prev    = prev;
  node->element = element;

  return node;
}

/**
 * Goes trough each element of list. If operate returns false loop breaks.
 *
 * @param list    The list to go trough.
 * @param operate Function that gets called for each node.
 */
static void
for_each (linked_list *list,
          bool operate (void*))
{
  node *node = list->first;

  for (; node != NULL; node = node->next)
    {
      if (!operate (node->element))
        break;
    }
}

/* Gets a node, if not found returns NULL */
static node *
get_node (linked_list *list,
          int          index)
{
  node *node = list->first;

  if (!check_borders (list, index))
    return NULL;

  for (int i = 0;
       node != NULL && i <= index;
       node = node->next, i++)
    {
      if (i == index)
        return node;
    }

  return NULL;
}

static bool
free_node (node *node)
{
  if (node == NULL)
    return false;

  free (node);
  return true;
}


static int
add_first_element (linked_list *list,
                   void        *element)
{
  node *node = new_node (NULL, NULL, element);

  list->first  = node;
  list->last   = node;
  list->length = list->length + 1;

  return linked_list_length (list);
}

static void
increment_length (linked_list *list)
{
  list->length = list->length + 1;
}

static void
decrement_length (linked_list *list)
{
  list->length = list->length - 1;
}

static bool
check_borders (linked_list *list,
               int          index)
{
  int length = linked_list_length (list);

  if (index < length && index >= 0)
    return true;

  return false;
}
