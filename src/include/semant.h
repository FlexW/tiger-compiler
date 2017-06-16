/**
 * @file semant.h
 * Provides function for semantic analyse.
 */

#ifndef _SEMANT_H_
#define _SEMANT_H_

#include "absyn.h"
#include "frame.h"

frm_frag_list * sem_trans_prog (absyn_exp *exp_ptr);

#endif
