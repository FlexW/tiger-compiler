/**
 * @file list.h
 * Generic linked list declarations.
 */

#ifndef _LIST_H_
#define _LIST_H_

typedef struct _list list;

struct _list
{
  void *head;
  list *tail;
};

list * list_new_list (void *head,
                      list *tail);

int    list_length   (list *list);

#endif /* _LIST_H_ */
