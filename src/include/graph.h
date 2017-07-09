/**
 * @file graph.h
 * Abstract Data Type (ADT) for directed graphs.
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdbool.h>
#include <stdio.h>

//#include "list.h"
#include "table.h"
#include "util.h"

typedef struct _graph_graph graph_graph;  /* The "graph" type */
typedef struct _graph_node  graph_node;    /* The "node" type */

typedef struct _graph_node_list graph_node_list;

/* The type of "tables" mapping graph-nodes to information */
typedef tab_table  graph_table;

struct
_graph_node_list
{
  graph_node      *head;
  graph_node_list *tail;
};

graph_node_list * graph_new_node_list (graph_node      *head,
                                       graph_node_list *tail);

/* Make a new graph */
graph_graph * graph_new_graph      (void);
/* Make a new node in graph "g", with associated "info" */
graph_node *  graph_get_graph_node (graph_graph *g,
                                    void        *info);

/* Get the list of nodes belonging to "g" */
graph_node_list * graph_nodes (graph_graph *g);

/* Tell if "a" is in the list "l" */
bool graph_in_node_list (graph_node      *a,
                         graph_node_list *l);

/* Make a new edge joining nodes "from" and "to", which must belong
    to the same graph */
void graph_add_edge (graph_node *from,
                     graph_node *to);

/* Delete the edge joining "from" and "to" */
void graph_rm_edge (graph_node *from,
                    graph_node *to);

/* Show all the nodes and edges in the graph, using the function "showInfo"
    to print the name of each node */
void graph_show (FILE            *out,
                 graph_node_list *p,
                 void showInfo (void *));

/* Get all the successors of node "n" */
graph_node_list * graph_succ (graph_node *n);

/* Get all the predecessors of node "n" */
graph_node_list * graph_pred (graph_node *n);

/* Tell if there is an edge from "from" to "to" */
bool graph_goes_to (graph_node *from,
                    graph_node *n);

/* Tell how many edges lead to or from "n" */
int graph_degree (graph_node *n);

/* Get all the successors and predecessors of "n" */
graph_node_list * graph_adj (graph_node *n);

/* Get the "info" associated with node "n" */
void * graph_node_info (graph_node *n);

/* Make a new table */
graph_table * graph_new_table (void);

/* Enter the mapping "node"->"value" to the table "t" */
void graph_bind (graph_table *t,
                 graph_node  *node,
                 void        *value);

/* Tell what "node" maps to in table "t" */
void * graph_lookup (graph_table *t,
                     graph_node  *node);

graph_node_list * graph_reverse_nodes (graph_node_list *l);

#endif /* _GRAPH_H_ */
