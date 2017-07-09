/**
 * @file util.h
 * General helper functions.
 *
 * Global functions and variables start with util_.
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdbool.h>

//#include "list.h"

typedef struct _util_bool_list util_bool_list;

struct
_util_bool_list
{
  bool head;
  util_bool_list *tail;
};

void * new        (int sizeof_b);

char * string_new (char *s);

util_bool_list * util_new_bool_list (bool head, util_bool_list *tail);

#endif // _UTIL_H_
