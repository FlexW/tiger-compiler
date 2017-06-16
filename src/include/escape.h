/**
 * @file escape.h
 * Searches for escaping variables in abstract syntax tree.
 *
 * Local variables that do not escape can be allocated in a register; escaping
 * variables must be allocated in the frame.
 */

#include "absyn.h"


void esc_find_escaping_var (absyn_exp *exp_ptr);
