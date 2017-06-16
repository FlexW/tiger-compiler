/**
 * @file prabsyn.h
 * Function to print out abstract syntax tree.
 */

#ifndef _PRABSYN_H_
#define _PRABSYN_H_

#include <stdio.h>

#include "absyn.h"

void prabsyn_exp (FILE      *out_ptr,
                  absyn_exp *asyn_ptr,
                  int        indent);

#endif
