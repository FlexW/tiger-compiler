/**
 * @file list.c
 * Generic linked list definitions.
 */
#include <stdlib.h>

#include "include/list.h"
#include "include/util.h"

list *
list_new_list (void *head,
               list *tail)
{
  list *list = new (sizeof (*list));

  list->head = head;
  list->tail = tail;

  return list;
}

int
list_length (list *list)
{
  int len = 0;
  for (; list != NULL; list = list->tail)
    len++;
  return len;
}
