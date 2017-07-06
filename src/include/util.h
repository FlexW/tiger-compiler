/**
 * @file util.h
 * General helper functions.
 *
 * Global functions and variables start with util_.
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdbool.h>

#include "list.h"

typedef list util_bool_list;

void * new        (int sizeof_b);

char * string_new (char *s);

#endif // _UTIL_H_
