/**
 * @file liveness.c
 * Liveness analyse.
 */
#include <stdio.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/absyn.h"
#include "include/assem.h"
#include "include/frame.h"
#include "include/graph.h"
#include "include/flowgraph.h"
#include "include/liveness.h"
#include "include/table.h"


live_move_list *
live_new_move_list (graph_node     *src,
                    graph_node     *dst,
                    live_move_list *tail)
{
  live_move_list *lm = new (sizeof (*lm));

  lm->src  = src;
	lm->dst  = dst;
	lm->tail = tail;

  return lm;
}

temp_temp *
live_gtemp (graph_node *n)
{
  return (temp_temp*)graph_node_info (n);
}

static bool
equal_temp (temp_temp_list *ta,
            temp_temp_list *tb)
{
  return temp_equal(ta, tb);
}

static temp_temp_list *
minus_temp (temp_temp_list *ta,
            temp_temp_list *tb)
{
  return temp_minus (ta, tb);
}

static temp_temp_list *
union_temp (temp_temp_list *ta,
            temp_temp_list *tb)
{
  return temp_union (ta, tb);
}

static temp_temp_list *
intersect_temp (temp_temp_list *ta,
                temp_temp_list *tb)
{
  return temp_intersect (ta, tb);
}

static assem_instr_list *
inst_minus (assem_instr_list *ta,
            assem_instr_list *tb)
{
  return assem_instr_minus (ta, tb);
}

static assem_instr_list *
inst_union (assem_instr_list *ta,
            assem_instr_list *tb)
{
  return assem_instr_union (ta, tb);
}

static assem_instr_list *
inst_intersect (assem_instr_list *ta,
                assem_instr_list *tb)
{
  return assem_instr_intersect (ta, tb);
}

static void
enter_live_map (graph_table    *t,
                graph_node     *flow_node,
                temp_temp_list *temps)
{
  graph_bind (t, flow_node, temps);
}

static temp_temp_list *
lookup_live_map (graph_table *t,
                 graph_node  *flownode)
{
  return (temp_temp_list*)graph_lookup (t, flownode);
}

static void
get_live_map (graph_graph *flow,
              graph_table *in,
              graph_table *out)
{
  graph_node_list *fl, *sl;
  graph_node *n, *sn;
	graph_table *last_in = graph_new_table ();
  graph_table *last_out = graph_new_table ();
  temp_temp_list *ci, *co, *li, *lo;
  bool flag = true;

	// Loop
	while (flag)
    {
      for (fl = graph_nodes (flow); fl; fl = fl->tail)
        {
		      n = fl->head;
          li = lookup_live_map (in, n);
          lo = lookup_live_map (out, n);
          enter_live_map (last_in, n, li);
          enter_live_map (last_out, n, lo);

          ci = union_temp (fgraph_use (n), minus_temp (lo, fgraph_def (n)));
          co = NULL;
          for (sl = graph_succ (n); sl; sl = sl->tail)
            {
              sn = sl->head;
              co = union_temp (co, lookup_live_map (in, sn));
            }
          enter_live_map (in, n, ci);
          enter_live_map (out, n, co);
	      }
      flag = false;
      for (fl = graph_nodes (flow); fl; fl = fl->tail)
        {
          n = fl->head;
          li = lookup_live_map (in, n);
          lo = lookup_live_map (out, n);
          ci = lookup_live_map (last_in, n);
          co = lookup_live_map (last_out, n);

          if (!equal_temp (li, ci) || !equal_temp (lo, co))
            {
              flag = true;
              break;
            }
        }
	  }
}

static graph_node *
find_or_create_node (temp_temp   *t,
                     graph_graph *g,
                     tab_table   *tab)
{
  graph_node *ln = (graph_node*)tab_lookup (tab, t);
  if (ln == NULL)
    {
      ln = graph_get_graph_node (g, t);
      tab_bind_value (tab, t, ln);
    }
  return ln;
}

static void
solve_liveness (struct live_graph *lg,
                graph_graph       *flow,
                graph_table       *in,
                graph_table       *out)
{
  graph_graph *g = graph_new_graph ();
  tab_table *tab = tab_new_table ();
  live_move_list *ml = NULL;
  graph_node_list *fl;
  graph_node *n, *ndef, *nedge, *move_src, *move_dst;
  temp_temp_list *tdef, *tout, *tuse, *t, *tedge;

  temp_map *move_list = temp_new_map ();
  temp_map *spill_cost = temp_new_map ();
  assem_instr *inst;
  assem_instr_list *worklist_moves = NULL;

  // Traverse node
  for (fl = graph_nodes (flow); fl; fl = fl->tail)
    {
      n = fl->head;
      inst = fgraph_inst (n);
      tout = lookup_live_map (out, n);
      tdef = fgraph_def (n);
      tuse = fgraph_use (n);

    temp_temp_list *defuse = union_temp (tuse, tdef);

    // Spill Cost
    for (t = defuse; t; t = t->tail)
      {
        temp_temp *ti = t->head;
        long spills = (long)temp_look_ptr(spill_cost, ti);
        spills++;
        temp_enter_ptr (spill_cost, ti, (void*)spills);
      }

    // Move instruction?
    if (fgraph_is_move (n))
      {
        for (; defuse; defuse = defuse->tail)
          {
            temp_temp *t = defuse->head;
            find_or_create_node (t, g, tab);
            assem_instr_list *ml =
              (assem_instr_list*)temp_look_ptr (move_list, t);
            ml = inst_union (ml, list_new_list (inst, NULL));
            temp_enter_ptr (move_list, t, (void*)ml);
          }
        worklist_moves = inst_union (worklist_moves, list_new_list (inst, NULL));
      }

    // Traverse defined vars
    for (t = tout; t; t = t->tail)
      {
        ndef = find_or_create_node (t->head, g, tab);
        // Add edges between output vars and defined var
        for (tedge = tout; tedge; tedge = tedge->tail)
          {
            nedge = find_or_create_node (tedge->head, g, tab);
            // Skip if edge is added
            if (ndef == nedge
                || graph_goes_to (ndef, nedge)
                || graph_goes_to (nedge, ndef))
              {
                continue;
              }
            // Skip src for move instruction
            if (fgraph_is_move (n) && nedge == move_src)
              {
                continue;
              }
        graph_add_edge (ndef, nedge);
      }
    }
  }

  lg->graph          = g;
  lg->worklist_moves = worklist_moves;
  lg->move_list      = move_list;
  lg->spill_cost     = spill_cost;
}
/*
static void
solve_liveness3 (struct live_graph *lg,
                 graph_graph       *flow,
                 graph_table       *in,
                 graph_table       *out)
{
  graph_node_list *fl;
  graph_node *n, *ndef, *nedge, *move_src, *move_dst;
  assem_instr *inst;
  temp_temp_list *tdef, *tuse, *tout, *tl, *tedge, *live = NULL;

  graph_graph      *g              = graph_new_graph ();
  tab_table        *tab            = tab_new_table ();
  temp_map         *move_list      = temp_new_map ();
  temp_map         *spill_cost     = temp_new_map ();
  assem_instr_list *worklist_moves = NULL;
  bool              block_start    = true;
  // Traverse node
  fl = graph_reverse_nodes (graph_nodes (flow));
  //if (fl) live = lookup_live_map(out, fl->head);
  for (; fl; fl = fl->tail)
    {
      if (block_start)
        {
          live = lookup_live_map (out, fl->head);
          block_start = false;
        }
      n    = fl->head;
      inst = fgraph_inst (n);
      tuse = fgraph_use (n);
      tdef = fgraph_def (n);

      if (inst->kind == I_LABEL)
        {
          block_start = true;
          continue;
        }
      temp_temp_list *defuse = union_temp (tuse, tdef);
      // Spill Cost
      for (tl = defuse; tl; tl = tl->tail)
        {
          temp_temp *ti = tl->head;
          long spills = (long)temp_look_ptr (spill_cost, ti);
          spills++;
          temp_enter_ptr (spill_cost, ti, (void*)spills);
        }
      // Move instruction?
      if (fgraph_is_move (n))
        {
          live = minus_temp (live, tuse);
          for (; defuse; defuse = defuse->tail)
            {
              temp_temp *t = defuse->head;
              assem_instr_list *ml =
                (assem_instr_list*)temp_look_ptr (move_list, t);
              ml = inst_union (ml, list_new_list (inst, NULL));
              temp_enter_ptr (move_list, t, (void*)ml);
            }
          worklist_moves = inst_union (worklist_moves,
                                       list_new_list (inst, NULL));
        }
      live = union_temp (live, tdef);
      // Traverse defined vars
      for (tl = tdef; tl; tl = tl->tail)
        {
          ndef = find_or_create_node (tl->head, g, tab);
          // Add edges between output vars and defined var
          for (tedge = live; tedge; tedge = tedge->tail)
            {
              nedge = find_or_create_node (tedge->head, g, tab);
              // Skip if edge is added
              if (ndef == nedge
                || graph_goes_to(ndef, nedge)
                || graph_goes_to(nedge, ndef))
                {
                  continue;
                }
              graph_add_edge(ndef, nedge);
            }
          }
      live = union_temp (tuse, minus_temp (live, tdef));
  }
  lg->graph          = g;
  lg->worklist_moves = worklist_moves;
  lg->move_list      = move_list;
  lg->spill_cost     = spill_cost;
}

static void
solve_liveness2 (struct live_graph *lg,
                 graph_graph       *flow,
                 graph_table       *in,
                 graph_table       *out)
{
  graph_graph    *g   = graph_new_graph ();
  tab_table      *tab = tab_new_table ();
  live_move_list *ml  = NULL;
  graph_node_list *fl;
  graph_node *n, *ndef, *nedge, *move_src, *move_dst;
  temp_temp_list *tdef, *tout, *t, *tedge;

  // Traverse node
  for (fl = graph_nodes (flow); fl; fl = fl->tail)
    {
      n    = fl->head;
      tout = lookup_live_map(out, n);
      tdef = fgraph_def (n);
      // Move instruction?
      if (fgraph_is_move (n))
        {
          move_src = find_or_create_node (fgraph_use (n)->head, g, tab);
          move_dst = find_or_create_node (fgraph_def (n)->head, g, tab);
          ml       = live_new_move_list (move_src, move_dst, ml);
        }
      // Traverse defined vars
      for (t = tout; t; t = t->tail)
        {
          ndef = find_or_create_node (t->head, g, tab);
          // Add edges between output vars and defined var
          for (tedge = tout; tedge; tedge = tedge->tail)
            {
              nedge = find_or_create_node (tedge->head, g, tab);
              // Skip if edge is added
              if (ndef == nedge
                  || graph_goes_to (ndef, nedge)
                  || graph_goes_to (nedge, ndef))
                  {
                    continue;
                  }
              // Skip src for move instruction
              if (fgraph_is_move (n)
                  && nedge == move_src)
                {
                  continue;
                }
              graph_add_edge (ndef, nedge);
            }
        }
  }

  lg->graph = g;
  lg->moves = ml;
}
*/
struct live_graph
live_liveness(graph_graph *flow)
{
  graph_table *in = graph_new_table (), *out = graph_new_table ();
	get_live_map (flow, in, out);
  // Construct interference graph
	struct live_graph lg;
  solve_liveness (&lg, flow, in, out);
	return lg;
}
