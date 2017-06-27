/**
 * @file codegen.h
 */

#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include "assem.h"
#include "frame.h"
#include "tree.h"

assem_instr_list * codegen (frm_frame     *f,
                            tree_stm_list *stm_list);

#endif // _CODEGEN_H_
