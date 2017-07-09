/**
 * @file graph.c
 * Functions to manipulate and create control flow and
 * interference graphs.
 */

#include <assert.h>
#include <stdio.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/absyn.h"
#include "include/assem.h"
#include "include/frame.h"
#include "include/graph.h"
#include "include/errormsg.h"
#include "include/table.h"

struct
_graph_graph
{
  int              nodecount;
  graph_node_list *mynodes;
  graph_node_list *mylast;
};

struct
_graph_node
{
  graph_graph     *mygraph;
  int              mykey;
  graph_node_list *succs;
  graph_node_list *preds;
  void            *info;
};

graph_node_list *
graph_new_node_list (graph_node      *head,
                     graph_node_list *tail)
{
  graph_node_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

graph_graph *
graph_new_graph (void)
{
  graph_graph *g = new (sizeof *g);

  g->nodecount = 0;
  g->mynodes   = NULL;
  g->mylast    = NULL;

  return g;
}

/* generic creation of G_node */
graph_node *
graph_get_graph_node (graph_graph *g,
                      void        *info)
{
  graph_node *n = new (sizeof *n);

  graph_node_list *p = graph_new_node_list (n, NULL);

  assert (g);
  n->mygraph = g;
  n->mykey   = g->nodecount++;

  if (g->mylast == NULL)
    g->mynodes = g->mylast = p;
  else
    g->mylast = g->mylast->tail = p;

  n->succs = NULL;
  n->preds = NULL;
  n->info  = info;
  return n;
}

graph_node_list *
graph_nodes (graph_graph *g)
{
  assert (g);
  return g->mynodes;
}

/* return true if a is in l list */
bool
graph_in_node_list (graph_node      *a,
                    graph_node_list *l)
{
  graph_node_list *p;

  for (p = l; p != NULL; p = p->tail)
    if (p->head == a)
      return true;

  return false;
}

void
graph_add_edge (graph_node *from,
                graph_node *to)
{
  assert (from);
  assert (to);
  assert (from->mygraph == to->mygraph);

  if (graph_goes_to (from, to))
    return;

  to->preds   = graph_new_node_list (from, to->preds);
  from->succs = graph_new_node_list (to, from->succs);
}

static graph_node_list *
delete (graph_node      *a,
        graph_node_list *l)
{
  assert(a && l);

  if (a == l->head)
    return l->tail;
  else
    return graph_new_node_list (l->head, delete(a, l->tail));
}

void
graph_rm_edge (graph_node *from,
               graph_node *to)
{
  assert (from && to);

  to->preds   = delete (from, to->preds);
  from->succs = delete (to, from->succs);
}

 /**
  * Print a human-readable dump for debugging.
  */
void graph_show (FILE            *out,
                 graph_node_list *p,
                 void show_info (void *))
{
  for (; p != NULL; p = p->tail)
    {
      graph_node *n = p->head;
      graph_node_list *q;

      assert(n);

      if (show_info)
        show_info (n->info);

      fprintf(out, " (%d): ", n->mykey);
      for (q = graph_succ (n); q != NULL; q = q->tail)
        {
          graph_node *node = q->head;
          fprintf (out, "%d ", node->mykey);
        }
      fprintf(out, "\n");
  }
}

graph_node_list *
graph_succ (graph_node *n)
{
  assert(n);
  return n->succs;
}

graph_node_list *
graph_pred (graph_node *n)
{
  assert(n);
  return n->preds;
 }

bool
graph_goes_to (graph_node *from,
               graph_node *n)
{
  return graph_in_node_list (n, graph_succ (from));
}

/* return length of predecessor list for node n */
static int
in_degree (graph_node *n)
{
  int deg = 0;

  graph_node_list *p;
  for (p = graph_pred (n); p != NULL; p = p->tail)
    deg++;

  return deg;
}

/* return length of successor list for node n */
static int
out_degree (graph_node *n)
{
  int deg = 0;

  graph_node_list *p;
  for (p = graph_succ (n); p != NULL; p = p->tail)
    deg++;

  return deg;
}

int
graph_degree (graph_node *n)
{
  return in_degree(n) + out_degree (n);
}

/* put list b at the back of list a and return the concatenated list */
static graph_node_list *
cat (graph_node_list *a,
     graph_node_list *b)
{
  if (a == NULL)
    return b;
  else
    return graph_new_node_list (a->head, cat(a->tail, b));
}

/* create the adjacency list for node n by combining the successor and
 * predecessor lists of node n */
graph_node_list *
graph_adj (graph_node *n)
{
  return cat (graph_succ (n), graph_pred (n));
}

void *
graph_node_info (graph_node *n)
{
  return n->info;
}



/* G_node table functions */
graph_table *
graph_new_table (void)
{
  return tab_new_table ();
}

void
graph_bind (graph_table *t,
            graph_node  *node,
            void        *value)
{
  tab_bind_value (t, node, value);
}

void *
graph_lookup (graph_table *t,
              graph_node  *node)
{
  return tab_lookup (t, node);
}

graph_node_list *
graph_reverse_nodes (graph_node_list *l)
{
  graph_node_list *nl = NULL;
  for (; l; l = l->tail)
    nl = graph_new_node_list (l->head, nl);
  return nl;
}
