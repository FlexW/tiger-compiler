/**
 * @file regalloc.c
 * Register allocation for x86.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/errormsg.h"
#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/absyn.h"
#include "include/assem.h"
#include "include/frame.h"
#include "include/graph.h"
#include "include/color.h"
#include "include/flowgraph.h"
#include "include/liveness.h"
#include "include/regalloc.h"
#include "include/table.h"

static void
print_temp (void* t) {
  temp_map *m = temp_name();
  printf("node: %s\n", temp_lookup (m, (temp_temp*)t));
}

static void print_inst (void *info)
{
  assem_instr *inst = (assem_instr*)info;
  assem_print (stdout, inst, temp_name ());
}

static assem_instr_list *
reverse_instr_list (assem_instr_list *il)
{
  assem_instr_list *rl = NULL;
  for (; il; il = il->tail)
    {
      rl = list_new_list (il->head, rl);
    }
  return rl;
}

static temp_temp_list *
inst_def (assem_instr *inst)
{
  switch (inst->kind)
    {
    case I_OPER:
      return inst->u.oper.dst;
    case I_LABEL:
      return NULL;
    case I_MOVE:
      return inst->u.move.dst;
    default:
      assert (0);
    }
}

static temp_temp_list *
inst_use (assem_instr *inst)
{
  switch (inst->kind)
    {
    case I_OPER:
      return inst->u.oper.src;
    case I_LABEL:
      return NULL;
    case I_MOVE:
      return inst->u.move.src;
    }
  return NULL;
}

static graph_node *
temp_to_node (temp_temp   *t,
              graph_graph *g)
{
  if (t == NULL)
    return NULL;

  graph_node_list *nodes = graph_nodes (g);
  graph_node_list *p;

  for (p = nodes; p != NULL; p = p->tail)
    if (live_gtemp (p->head)==t)
      return p->head;

  return NULL;
}

static temp_temp *
node_to_temp (graph_node *n)
{
  if (n == NULL)
    return NULL;
  return live_gtemp (n);
}
/*
static bool
equal_temp (temp_temp_list *ta,
            temp_temp_list *tb)
{
  return temp_equal (ta, tb);
}

static temp_temp_list * minus_temp(temp_temp_list ta, temp_temp_list tb) {
  return Temp_minus(ta, tb);
}
*/
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
  return temp_intersect(ta, tb);
}

static bool
temp_in (temp_temp      *t,
         temp_temp_list *tl)
{
  return temp_in_list (t, tl);
}
/*
static temp_temp_list L(Temp_temp h, temp_temp_list t) {
  return Temp_TempList(h, t);
}
*/
static bool inst_in (assem_instr      *i,
                     assem_instr_list *il)
{
  return assem_instr_in_list (i, il);
}


static graph_node *
get_alias (graph_node     *n,
           graph_table    *aliases,
           temp_temp_list *coalesced_nodes)
{
  temp_temp *t = node_to_temp (n);

  if (temp_in (t, coalesced_nodes))
    {
      graph_node *alias = (graph_node*)graph_lookup (aliases, n);
      return get_alias (alias, aliases, coalesced_nodes);
    }
  else
    {
      return n;
    }
}

static temp_temp_list *
aliased (temp_temp_list *tl,
         graph_graph    *ig,
         graph_table    *aliases,
         temp_temp_list *cn)
{
  temp_temp_list *al = NULL;
  for (; tl; tl = tl->tail)
    {
      temp_temp  *t = tl->head;
      graph_node *n = temp_to_node (t, ig);
      graph_node *alias = get_alias (n, aliases, cn);
      t = node_to_temp (n);
      al = list_new_list (t, al);
    }
  return union_temp (al, NULL);
}

struct regalloc_result
regalloc_do (frm_frame        *f,
             assem_instr_list *il)
{
  //your code here.
  struct regalloc_result ret;
  bool ra_finished = false;

  graph_graph *flow;
  struct live_graph live;
  temp_map *initial;
  struct col_result col;
  assem_instr_list *rewrite_list;

  int try = 0;
  while (++try < 7)
    {
      flow = fgraph_assem_flow_graph (il, f);
      graph_show (stdout, graph_nodes(flow), print_inst);
      live = live_liveness (flow);
      graph_show (stdout, graph_nodes(live.graph), print_temp);
      initial = frm_initial_registers (f);
      col = col_color (live.graph, initial, frm_registers (),
                       live.worklist_moves, live.move_list, live.spill_cost);

    if (col.spills == NULL)
      {
        break;
      }

    temp_temp_list *spilled = col.spills;
    rewrite_list = NULL;

    // Assign locals in memory
    temp_temp_list *tl;
    tab_table *spilled_local = tab_new_table ();
    for (tl = spilled; tl; tl = tl->tail)
      {
        frm_access *local = frm_alloc_local (f, true);
        tab_bind_value (spilled_local, tl->head, local);
      }

    // Rewrite instructions
    for (; il; il = il->tail)
      {
        assem_instr *inst = il->head;
        temp_temp_list *use_spilled = intersect_temp (aliased(inst_use(inst),
                                                              live.graph,
                                                              col.alias,
                                                              col.coalesced_nodes),
                                                      spilled);
        temp_temp_list *def_spilled = intersect_temp (aliased(inst_def(inst),
                                                              live.graph,
                                                              col.alias,
                                                              col.coalesced_nodes),
                                                      spilled);
        temp_temp_list *temp_spilled = union_temp (use_spilled, def_spilled);

      // Skip unspilled instructions
      if (temp_spilled == NULL)
        {
          rewrite_list = list_new_list (inst, rewrite_list);
          continue;
        }

      for (tl = use_spilled; tl; tl = tl->tail)
        {
          char buf[128];
          temp_temp *temp = tl->head;
          frm_access *local = (frm_access*)tab_lookup (spilled_local, temp);
          sprintf(buf, "movl %d(`s0), `d0  # spilled\n",
                  frm_access_offset (local));
          char *c = new (sizeof (buf));
          memcpy (c, buf, sizeof (buf));
          rewrite_list = list_new_list (assem_new_oper (c,
                                                       list_new_list (temp,
                                                                      NULL),
                                                       list_new_list (frm_fp (),
                                                                      NULL),
                                                       NULL),
                                       rewrite_list);
      }

      rewrite_list = list_new_list (inst, rewrite_list);

      for (tl = def_spilled; tl; tl = tl->tail)
        {
          char buf[128];
          temp_temp *temp = tl->head;
          frm_access *local = (frm_access*)tab_lookup (spilled_local, temp);
          sprintf(buf, "movl `s0, %d(`s1)  # spilled\n",
                  frm_access_offset (local));
          char *c = new (sizeof (buf));
          memcpy (c, buf, sizeof (buf));
          rewrite_list = list_new_list (assem_new_oper (c,
                                                       NULL,
                                                       list_new_list (temp,
                                                                      list_new_list (frm_fp(),
                                                                                     NULL)),
                                                       NULL),
                                       rewrite_list);
      }
    }

    il = reverse_instr_list (rewrite_list);
  }

  if (col.spills != NULL)
    {
      errm_printf (0, "fail to allocate registers");
    }

  if (col.coalesced_moves != NULL)
    {
      rewrite_list = NULL;
      for (; il; il = il->tail)
        {
          assem_instr *inst = il->head;

          // Remove coalesced moves
          if (inst_in (inst, col.coalesced_moves))
            {
              char buf[1024];
              sprintf(buf, "# ");
              strcat(buf, inst->u.oper.assem);
              char *c = new (sizeof (buf));
              memcpy (c, buf, sizeof (buf));
              inst->u.oper.assem = c;
              //continue;
            }
          rewrite_list = list_new_list (inst, rewrite_list);
    }
    il = reverse_instr_list (rewrite_list);
  }
  ret.coloring = col.coloring;
  ret.il = il;

  // temp_temp_list precolored = NULL;
  // temp_temp_list initial = NULL;
  // temp_temp_list simplifyWorklist = NULL;
  // temp_temp_list freezeWorklist = NULL;
  // temp_temp_list spillWorklist = NULL;
  // temp_temp_list spilledNodes = NULL;
  // temp_temp_list coalesced_nodes = NULL;   // Coalesce
  // temp_temp_list coloredNodes = NULL;
  // temp_temp_list selectStack = NULL;

  // temp_temp_list worklistMoves = NULL; // Coalesce

  // do {

  // } while (simplifyWorklist != NULL || worklistMoves != NULL);

  return ret;
}
