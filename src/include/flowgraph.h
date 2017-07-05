/**
 * @file flowgraph.h
 * Function prototypes to represent control flow graphs.
 */

#ifndef _FGRAPH_H_
#define _FGRAPH_H_

#include "graph.h"
#include "assem.h"
#include "temp.h"

temp_temp_list * fgraph_def              (graph_node *n);

temp_temp_list * fgraph_use              (graph_node *n);

bool             fgraph_is_move          (graph_node *n);

graph_graph *    fgraph_assem_flow_graph (assem_instr_list *il,
                                          frm_frame        *f);

assem_instr *    fgraph_inst             (graph_node *n);

#endif /* _FGRAPH_H_ */
