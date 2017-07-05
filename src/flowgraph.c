/**
 * @file flowgraph.c
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/util.h"
#include "include/symbol.h"
#include "include/temp.h"
#include "include/tree.h"
#include "include/absyn.h"
#include "include/assem.h"
#include "include/frame.h"
#include "include/graph.h"
#include "include/flowgraph.h"
#include "include/errormsg.h"
#include "include/table.h"


temp_temp_list *
fgraph_def (graph_node *n)
{
  assem_instr *inst = (assem_instr*)graph_node_info (n);
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

temp_temp_list *
fgraph_use (graph_node *n)
{
  assem_instr *inst = (assem_instr*)graph_node_info (n);
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

bool
fgraph_is_move (graph_node *n)
{
  assem_instr *inst = (assem_instr*)graph_node_info (n);
  return (inst->kind == I_MOVE);
}

assem_instr *
fgraph_inst (graph_node *n)
{
  return (assem_instr*)graph_node_info (n);
}

static graph_node *
find_labeled_node (temp_label      *lab,
                   graph_node_list *nl,
                   temp_label_list *ll)
{
  graph_node *result = NULL;
  for (; nl && ll; nl = nl->tail, ll = ll->tail)
    {
      if (ll->head == lab)
        {
          result = nl->head;
          return result;
        }
    }
  return result;
}

graph_graph *
fgraph_assem_flow_graph (assem_instr_list *il,
                        frm_frame        *f)
{
  graph_graph *g = graph_new_graph ();
  graph_node_list *nl = NULL, *jumpnl = NULL;
  temp_label_list *ll = NULL, *jl = NULL, *last_lbl = NULL;
  graph_node *n = NULL, *last_n = NULL, *jump_n = NULL;
  assem_instr *inst = NULL, *last_inst = NULL, *last_nonlbl_inst = NULL;

   // Iterate and add instructions to graph
   for (; il; il = il->tail)
     {
       inst = il->head;
       if (inst->kind != I_LABEL)
         {
           n = graph_get_graph_node (g, (void*)inst);

           if (last_inst)
             {
               if (last_inst->kind == I_LABEL)
                 {
                   nl = list_new_list (n, nl);
                   ll = list_new_list (last_inst->u.label.label, ll);
                   if (last_nonlbl_inst)
                     {
                       graph_add_edge (last_n, n);
                     }
                 }
              else if (last_inst->kind == I_OPER
                && last_inst->u.oper.jumps != NULL)
                {
                  // add edge for conditional jumps
                  if (strstr(last_inst->u.oper.assem, "jmp")
                    != last_inst->u.oper.assem)
                    {
                      graph_add_edge (last_n, n);
                    }
                }
              else
                {
                  graph_add_edge (last_n, n);
                }
             }
       if (inst->kind == I_OPER && inst->u.oper.jumps != NULL)
         {
           jumpnl = list_new_list (n, jumpnl);
         }
       last_n = n;
       last_nonlbl_inst = inst;
      }
      last_inst = inst;
    }

   // Handle jump instructions
   for (; jumpnl; jumpnl = jumpnl->tail)
     {
       n = jumpnl->head;
       inst = (assem_instr*)graph_node_info (n);
       for (jl = inst->u.oper.jumps->labels; jl; jl = jl->tail)
         {
           jump_n = find_labeled_node (jl->head, nl, ll);
           if (jump_n)
             {
               graph_add_edge (n, jump_n);
             }
           else
             {
               errm_printf (0,
                            "fail to find node for label %s",
                            temp_label_str (jl->head));
             }
         }
     }
   return g;
 }
