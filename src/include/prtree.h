/**
 * @file prtree.h
 * Function declaration for printing intermediate code.
 */

#ifndef _PRTREE_H_
#define _PRTREE_H_

#include <stdio.h>

#include "tree.h"


void print_stm_list (FILE *out,
                     tree_stm_list *stm_list);

#endif /* _PRTREE_H_ */
