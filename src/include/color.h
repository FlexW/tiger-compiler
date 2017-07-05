/**
 * @file color.h
 * Data structures and function prototypes for coloring algorithm
 * to determine register allocation.
 */

#ifndef _COLOR_H_
#define  _COLOR_H_

#include "temp.h"
#include "graph.h"
#include "assem.h"


struct
col_result
{
  temp_map         *coloring;
  temp_temp_list   *colored;
  temp_temp_list   *spills;
  assem_instr_list *coalesced_moves;
  temp_temp_list   *coalesced_nodes;
  graph_table      *alias;
};

struct col_result col_color (graph_graph      *ig,
                             temp_map         *initial,
                             temp_temp_list   *regs,
                             assem_instr_list *worklistMoves,
                             temp_map         *move_list,
                             temp_map         *spill_cost);

#endif /* _COLOR_H_ */
