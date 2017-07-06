/**
 * @file util.c
 *
 * Commonly used functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/util.h"

void*
new (int b_sizeof)
{
  void *p = malloc (b_sizeof);
  if (p == NULL)
  {
    fprintf (stderr, "\nRan out of memory!\n");
    exit(1);
  }
  return p;
}

char *
string_new (char *s)
{
  char *p = new (strlen(s)+1);
  strcpy(p,s);
  return p;
}
