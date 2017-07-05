/*
 * @file regalloc.c
 *Function prototype from regalloc.c
 */

#ifndef _REGALLOC_H_
#define _REGALLOC_H_

#include "assem.h"
#include "temp.h"
#include "frame.h"

#define RA_K 6

struct
regalloc_result
{
  temp_map         *coloring;
  assem_instr_list *il;
};

struct regalloc_result regalloc_do (frm_frame        *f,
                                    assem_instr_list *il);

#endif /* _REGALLOC_H_ */
