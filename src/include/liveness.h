/**
 * @file liveness.h
 * Definitions for liveness analyse.
 */

 #ifndef _LIVENESS_H_
 #define _LIVENESS_H_

#include "assem.h"
#include "list.h"
#include "graph.h"
#include "temp.h"

typedef struct _live_move_list live_move_list;

struct
_live_move_list
{
	graph_node *src, *dst;
	live_move_list *tail;
};

struct
live_graph
{
	graph_graph      *graph;
	live_move_list   *moves;
	assem_instr_list *worklist_moves;
	temp_map         *move_list;
	temp_map         *spill_cost;
};

live_move_list *  live_new_move_list (graph_node     *src,
                                      graph_node     *dst,
                                      live_move_list *tail);

temp_temp *       live_gtemp         (graph_node *n);

struct live_graph live_liveness      (graph_graph *flow);

#endif /* _LIVENESS_H_ */
