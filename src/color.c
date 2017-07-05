/**
 * @file color.c
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
#include "include/color.h"
#include "include/liveness.h"
#include "include/table.h"

typedef struct ctx col_ctx;

struct
ctx
{
  graph_graph    *nodes;
  temp_map       *precolored;
  temp_temp_list *initial;
  temp_temp_list *spill_work_list;
  temp_temp_list *freeze_work_list;
  temp_temp_list *simplify_work_list;
  temp_temp_list *spilled_nodes;
  temp_temp_list *coalesced_nodes;
  temp_temp_list *colored_nodes;
  temp_temp_list *select_stack;

  assem_instr_list *coalesced_moves;
  assem_instr_list *constrained_moves;
  assem_instr_list *frozen_moves;
  assem_instr_list *worklist_moves;
  assem_instr_list *active_moves;

  temp_map    *spill_cost;
  temp_map    *move_list;
  graph_table *alias;
  graph_table *degree;

  int k;
};

static col_ctx c;

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

static temp_temp_list *
clone_regs (temp_temp_list *regs)
{
  temp_temp_list *tl = NULL;
  for (; regs; regs = regs->tail)
    {
      tl = list_new_list (regs->head, tl);
    }
  return tl;
}

static temp_temp *
temp_head (temp_temp_list *temps)
{
  if (temps == NULL)
    return NULL;
  return temps->head;
}

static graph_node *
temp_to_node (temp_temp *t) {
  if (t == NULL)
    return NULL;
  graph_node_list *nodes = graph_nodes(c.nodes);
  graph_node_list *p;

  for(p=nodes; p!=NULL; p=p->tail)
    if (live_gtemp (p->head) == t)
      return p->head;

  return NULL;
}

static graph_node_list *
temp_list_to_node_list (temp_temp_list *tl)
{
  graph_node_list *nl = NULL;
  for (; tl; tl = tl->tail)
    {
      nl = list_new_list (temp_to_node (tl->head), nl);
    }
  return graph_reverse_nodes (nl);
}

static temp_temp *
node_to_temp (graph_node *n)
{
  if (n == NULL)
    return NULL;

  return live_gtemp (n);
}

static temp_temp_list *
node_list_to_temp_list (graph_node_list *nl)
{
  temp_temp_list *tl = NULL;
  for (; nl; nl = nl->tail)
    {
      tl = list_new_list (node_to_temp (nl->head), tl);
    }
  return temp_reverse_list(tl);
}

static char *
name_temp (temp_temp *t)
{
  return temp_lookup (temp_name (), t);
}

static temp_temp *
str_to_color (char           *color,
              temp_map       *regcolors,
              temp_temp_list *regs)
{
  for (; regs; regs = regs->tail)
    {
      char *s = temp_lookup (regcolors, regs->head);
      if (s && (strcmp(s, color) == 0))
        {
          return regs->head;
        }
    }
  //printf("register not found for given color: %s\n", color);
  return NULL;
}

static char *
color_to_str (temp_temp *reg,
              temp_map *regcolors)
{
  char *color = temp_lookup (regcolors, reg);
  if (color == NULL)
    {
      printf("color not found for given register: %s\n", name_temp (reg));
    }
  return color;
}

static int
count_temp (temp_temp_list *t)
{
  int cnt = 0;
  for (; t; t = t->tail)
    {
      cnt++;
    }
  return cnt;
};

static bool
equal_temp (temp_temp_list *ta,
            temp_temp_list *tb)
{
  return temp_equal (ta, tb);
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

static bool
in_temp (temp_temp      *t,
         temp_temp_list *tl)
{
  return temp_in_list (t, tl);
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
  return assem_instr_intersect(ta, tb);
}

static bool
inst_in (assem_instr      *i,
         assem_instr_list *il)
{
  return assem_instr_in_list (i, il);
}


static temp_temp_list *
adjacent (temp_temp *t)
{
  graph_node      *n    = temp_to_node (t);
  graph_node_list *adjn = graph_adj (n);
  temp_temp_list  *adjs = NULL;
  for (; adjn; adjn = adjn->tail)
    {
      adjs = list_new_list (node_to_temp (n), adjs);
    }
  adjs = minus_temp (adjs, union_temp (c.select_stack, c.coalesced_nodes));
  return adjs;
}

static void
add_edge (graph_node *nu,
          graph_node *nv)
{
  if (nu == nv)
    return;
  if (graph_goes_to (nu, nv) || graph_goes_to (nv, nu))
    return;

  graph_add_edge (nu, nv);

  temp_temp *u = node_to_temp (nu);
  temp_temp *v = node_to_temp (nv);

  if (temp_lookup (c.precolored, u) == NULL)
    {
      long d = (long)graph_lookup (c.degree, nu);
      d += 1;
      graph_bind (c.degree, nu, (void*)d);
  }

  if (temp_lookup (c.precolored, v) == NULL)
    {
      long d = (long)graph_lookup (c.degree, nv);
      d += 1;
      graph_bind (c.degree, nv, (void*)d);
  }
}

static assem_instr_list *
node_moves (temp_temp *t)
{
  assem_instr_list *ml = (assem_instr_list*)temp_look_ptr (c.move_list, t);
  return inst_intersect (ml, inst_union (c.active_moves, c.worklist_moves));
}

static bool
move_related (temp_temp *t)
{
  return node_moves (t) != NULL;
}

static void
make_work_list ()
{
  temp_temp_list *tl;
  for (tl = c.initial; tl; tl = tl->tail)
    {
      temp_temp *t = tl->head;
      graph_node *n = temp_to_node (t);
      c.initial = minus_temp (c.initial, list_new_list (t, NULL));

    if (graph_degree (n) >= c.k)
      {
        c.spill_work_list = union_temp (c.spill_work_list,
                                        list_new_list (t, NULL));
      }
    else if (move_related (t))
      {
        c.freeze_work_list = union_temp (c.freeze_work_list,
                                         list_new_list (t, NULL));
      }
    else
      {
        c.simplify_work_list = union_temp (c.simplify_work_list,
                                           list_new_list (t, NULL));
      }
    }
}

static void
remove_adj (graph_node *n)
{
  graph_node_list *adjs = graph_succ(n);
  for (; adjs; adjs = adjs->tail)
    {
      graph_node *m = adjs->head;
      graph_rm_edge (n, m);
    }

  adjs = graph_pred(n);
  for (; adjs; adjs = adjs->tail)
    {
      graph_rm_edge (adjs->head, n);
    }
}

static void
enable_moves (temp_temp_list *tl)
{
  for (; tl; tl = tl->tail)
    {
      assem_instr_list *il = node_moves (tl->head);
      for (; il; il = il->tail)
        {
          assem_instr *m = il->head;
          if (inst_in (m, c.active_moves))
            {
              c.active_moves = inst_minus (c.active_moves,
                                           list_new_list (m, NULL));
              c.worklist_moves = inst_union (c.worklist_moves,
                                             list_new_list (m, NULL));
            }
        }
    }
}

static void
decrement_degree (graph_node *n)
{
  temp_temp *t = node_to_temp (n);
  long d = (long)graph_lookup (c.degree, n);
  d -= 1;
  graph_bind (c.degree, n, (void*)d);

  if (d == c.k)
    {
      enable_moves (list_new_list (t, adjacent(t)));
      c.spill_work_list = minus_temp (c.spill_work_list,
                                      list_new_list (t, NULL));
      if (move_related (t))
        {
          c.freeze_work_list = union_temp (c.freeze_work_list,
                                           list_new_list (t, NULL));
        }
      else
        {
          c.simplify_work_list = union_temp (c.simplify_work_list,
                                             list_new_list (t, NULL));
        }
    }
}

static void
add_work_list (temp_temp *t)
{
  long degree = (long)graph_lookup (c.degree, temp_to_node (t));
  if (temp_lookup (c.precolored, t) == NULL
      && (!move_related (t))
      && (degree < c.k))
    {
      c.freeze_work_list = minus_temp (c.freeze_work_list,
                                       list_new_list (t, NULL));
      c.simplify_work_list = union_temp (c.simplify_work_list,
                                         list_new_list (t, NULL));
    }
}

static bool
ok (temp_temp *t,
    temp_temp *r)
{
  graph_node *nt = temp_to_node (t);
  graph_node *nr = temp_to_node (r);
  long degree = (long)graph_lookup (c.degree, nt);
  if (degree < c.k)
    {
      return true;
    }
  if (temp_lookup (c.precolored, t))
    {
      return true;
    }
  if (graph_goes_to (nt, nr) || graph_goes_to (nr, nt))
    {
      return true;
    }
  return false;
}

static bool
conservative(temp_temp_list *tl)
{
  graph_node_list *nl = temp_list_to_node_list (tl);
  int k = 0;
  for (; nl; nl = nl->tail)
    {
      long degree = (long)graph_lookup (c.degree, nl->head);
      if (degree >= c.k)
        {
          k++;
        }
    }
  return (k < c.k);
}

static graph_node *
get_alias (graph_node *n)
{
  temp_temp *t = node_to_temp (n);
  if (in_temp (t, c.coalesced_nodes))
    {
      graph_node *alias = (graph_node*)graph_lookup (c.alias, n);
      return get_alias (alias);
    }
  else
    {
      return n;
    }
}

static void
simplify ()
{
  if (c.simplify_work_list == NULL)
    {
      return;
    }

  temp_temp  *t = c.simplify_work_list->head;
  graph_node *n = temp_to_node (t);
  c.simplify_work_list = c.simplify_work_list->tail;

  c.select_stack = list_new_list (t, c.select_stack);  // push

  graph_node_list *adjs = graph_adj(n);
  for (; adjs; adjs = adjs->tail)
    {
      graph_node *m = adjs->head;
      decrement_degree (m);
    }
}

static void
combine (temp_temp *u,
         temp_temp *v)
{
  graph_node *nu = temp_to_node (u);
  graph_node *nv = temp_to_node (v);

  if (in_temp (v, c.freeze_work_list))
    {
      c.freeze_work_list = minus_temp (c.freeze_work_list,
                                       list_new_list (v, NULL));
    }
  else
    {
      c.spill_work_list = minus_temp (c.spill_work_list,
                                      list_new_list (v, NULL));
    }

  c.coalesced_nodes = union_temp (c.coalesced_nodes,
                                  list_new_list (v, NULL));
  graph_bind (c.alias, nv, (void*)nu);

  assem_instr_list *au = (assem_instr_list*)temp_look_ptr (c.move_list, u);
  assem_instr_list *av = (assem_instr_list*)temp_look_ptr (c.move_list, v);
  au = inst_union (au, av);
  temp_enter_ptr (c.move_list, u, (void*)au);

  enable_moves (list_new_list (v, NULL));

  //Temp_temp_list tadjs = adjacent(v);
  //graph_node_list adjs = temp_list_to_node_list (tadjs);
  graph_node_list *adjs = graph_adj (nv);
  temp_temp_list *tadjs = node_list_to_temp_list (adjs);
  for (; adjs; adjs = adjs->tail)
    {
      graph_node *nt = adjs->head;
      nt = get_alias (nt);
      add_edge (nt, nu);
      decrement_degree (nt);
    }
  tadjs = NULL;

  long degree = (long)graph_lookup (c.degree, nu);
  if (degree >= c.k && in_temp (u, c.freeze_work_list))
    {
      c.freeze_work_list = minus_temp (c.freeze_work_list,
                                       list_new_list (u, NULL));
      c.spill_work_list = union_temp (c.spill_work_list,
                                      list_new_list (u, NULL));
  }
}

static void
coalesce ()
{
  if (c.worklist_moves == NULL)
    {
      return;
    }

  assem_instr *m = c.worklist_moves->head;
  temp_temp *x = temp_head (inst_use (m));
  temp_temp *y = temp_head (inst_def (m));
  temp_temp *u, *v;

  x = node_to_temp (get_alias (temp_to_node (x)));
  y = node_to_temp (get_alias (temp_to_node (y)));

  if (temp_lookup (c.precolored, x) != NULL)
    {
      u = y; v = x;
    }
  else
    {
      u = x; v = y;
    }
  graph_node *nu = temp_to_node (u);
  graph_node *nv = temp_to_node (v);

  c.worklist_moves = inst_minus (c.worklist_moves, list_new_list (m, NULL));

  if (u == v)
    {
      c.coalesced_moves = inst_union (c.coalesced_moves,
                                      list_new_list (m, NULL));
      add_work_list (u);
    }
  else if (temp_lookup (c.precolored, v)
           || graph_goes_to (nu, nv)
           || graph_goes_to (nv, nu))
    {
      c.constrained_moves = inst_union (c.constrained_moves,
                                        list_new_list (m, NULL));
      add_work_list (u);
      add_work_list (v);
    }
  else
    {
      bool flag = false;
      if (temp_lookup (c.precolored, u))
        {
          flag = true;
          temp_temp_list *adj = adjacent (v);
          for (; adj; adj = adj->tail)
            {
              if (!ok (adj->head, u))
                {
                  flag = false;
                  break;
                }
            }
        }
      else
        {
          temp_temp_list *adju = adjacent(u);
          temp_temp_list *adjv = adjacent(v);
          temp_temp_list *adj  = union_temp (adju, adjv);
          flag = conservative(adj);
        }

    if (flag) {
      c.coalesced_moves = inst_union (c.coalesced_moves,
                                      list_new_list (m, NULL));
      combine(u, v);
      add_work_list (u);
    } else {
      c.active_moves = inst_union (c.active_moves,
                                   list_new_list (m, NULL));
    }
  }
}

static void
freeze_moves (temp_temp *u)
{
  assem_instr_list *il = node_moves (u);
  for (; il; il = il->tail)
    {
      assem_instr *m  = il->head;
      temp_temp   *x  = temp_head (inst_use (m));
      temp_temp   *y  = temp_head (inst_def (m));
      graph_node  *nx = temp_to_node (x);
      graph_node  *ny = temp_to_node (y);
      graph_node  *nv;

      if (get_alias (nx) == get_alias (ny))
        {
          nv = get_alias (nx);
        }
      else
        {
          nv = get_alias (ny);
        }
      temp_temp *v = node_to_temp (nv);

      c.active_moves = inst_minus (c.active_moves, list_new_list (m, NULL));
      c.frozen_moves = inst_union (c.frozen_moves, list_new_list (m, NULL));

      long degree = (long)graph_lookup (c.degree, nv);
      if (node_moves (v) == NULL && degree < c.k)
        {
          c.freeze_work_list = minus_temp (c.freeze_work_list,
                                           list_new_list (v, NULL));
          c.simplify_work_list = union_temp (c.simplify_work_list,
                                             list_new_list (v, NULL));
        }
    }
}

static void
freeze ()
{
  if (c.freeze_work_list == NULL)
    {
      return;
    }

  temp_temp *u = c.freeze_work_list->head;
  c.freeze_work_list = minus_temp (c.freeze_work_list, list_new_list (u, NULL));
  c.simplify_work_list = union_temp (c.simplify_work_list,
                                     list_new_list (u, NULL));
  freeze_moves (u);
}

static void
select_spill () {
  if (c.spill_work_list == NULL)
    {
      return;
    }

  temp_temp_list *tl                 = c.spill_work_list;
  float           min_spill_priority = 9999.0f;
  temp_temp      *m                  = NULL;

  for (; tl; tl = tl->tail)
    {
      temp_temp *t = tl->head;
      long cost = (long)temp_look_ptr (c.spill_cost, t);
      long degree = (long)graph_lookup (c.degree, temp_to_node (t));
      degree = (degree > 0) ? degree : 1;
      float priority = ((float)cost) / degree;
      if (priority < min_spill_priority)
        {
          min_spill_priority = priority;
          m = t;
        }
    }
  c.spill_work_list = minus_temp (c.spill_work_list, list_new_list (m, NULL));
  c.simplify_work_list = union_temp (c.simplify_work_list,
                                     list_new_list (m, NULL));
  freeze_moves (m);
}

static void
color_main ()
{
  make_work_list ();
  do
    {
      if (c.simplify_work_list != NULL)
        {
          simplify();
        }
      else if (c.worklist_moves != NULL)
        {
          coalesce();
        }
      else if (c.freeze_work_list != NULL)
        {
          freeze();
        } else if (c.spill_work_list != NULL)
        {
          select_spill ();
        }
    }
  while (c.simplify_work_list != NULL
         || c.worklist_moves != NULL
         || c.freeze_work_list != NULL
         || c.spill_work_list != NULL);
}

struct col_result
col_color (graph_graph      *ig,
           temp_map         *initial,
           temp_temp_list   *regs,
           assem_instr_list *worklist_moves,
           temp_map         *move_list,
           temp_map         *spill_cost)
{
  //your code here.
  struct col_result ret;

  c.precolored         = initial;
  c.initial            = NULL;
  c.simplify_work_list = NULL;
  c.freeze_work_list   = NULL;
  c.spill_work_list    = NULL;
  c.spilled_nodes      = NULL;
  c.coalesced_nodes    = NULL;
  c.colored_nodes      = NULL;
  c.select_stack       = NULL;

  c.coalesced_moves   = NULL;
  c.constrained_moves = NULL;
  c.frozen_moves      = NULL;
  c.worklist_moves    = worklist_moves;
  c.active_moves      = NULL;

  c.spill_cost = spill_cost;
  c.move_list  = move_list;
  c.degree     = graph_new_table ();
  c.alias      = graph_new_table ();
  c.nodes      = ig;

  c.k = count_temp (regs);


  temp_map *precolored = initial;
  temp_map *colors = temp_layer_map (temp_new_map (), initial);
  temp_temp_list *spilled_nodes = NULL, *colored_nodes = NULL;
  graph_node_list *nodes = graph_nodes (ig);
  graph_node_list *temps = NULL;

  graph_node_list *nl;
  for (nl = nodes; nl; nl = nl->tail)
    {
      long degree = graph_degree(nl->head);
      graph_bind (c.degree, nl->head, (void*)degree);

      if (temp_lookup (precolored, node_to_temp (nl->head)))
        {
          graph_bind (c.degree, nl->head, (void*)999);
          continue;
        }
      c.initial = list_new_list (node_to_temp (nl->head), c.initial);
    }

  color_main ();

  // for (nl = nodes; nl; nl = nl->tail) {
  //   if (temp_lookup (precolored, node_to_temp (nl->head))) {
  //     continue;
  //   }
  //   c.select_stack = L(node_to_temp (nl->head), c.select_stack);
  // }

  while (c.select_stack != NULL)
    {
      temp_temp *t = c.select_stack->head; // pop
      graph_node *n = temp_to_node (t);
      c.select_stack = c.select_stack->tail;

    temp_temp_list *ok_colors = clone_regs (regs);
    graph_node_list *adjs = graph_adj(n);
    graph_node *nw;
    temp_temp *w;
    char *color;

    int *it = (int*)t;
    if (*it == 119)
      {
        color = NULL;
      }

    for (; adjs; adjs = adjs->tail)
      {
        nw = adjs->head;
        w = node_to_temp (nw);
        graph_node *nw_alias = get_alias (nw);
        temp_temp *w_alias = node_to_temp (nw_alias);
        if ((color = temp_lookup (colors, w_alias)) != NULL)
          {
            temp_temp *colorTemp = str_to_color (color, precolored, regs);
            if (colorTemp)
              {
                ok_colors = minus_temp (ok_colors,
                                        list_new_list (colorTemp, NULL));
              }
          }
      }

    if (ok_colors == NULL)
      {
        c.spilled_nodes = list_new_list (t, c.spilled_nodes);
      }
    else
      {
        colored_nodes = list_new_list (t, colored_nodes);
        temp_bind_temp (colors, t, color_to_str (ok_colors->head, precolored));
      }
    }

  temp_temp_list *tl;
  for (tl = c.coalesced_nodes; tl; tl = tl->tail)
    {
      graph_node *alias = get_alias (temp_to_node (tl->head));
      char *color = temp_lookup (colors, node_to_temp (alias));
      temp_bind_temp (colors, tl->head, color);
    }

  ret.coloring = colors;

  ret.colored = NULL;
  for (; colored_nodes; colored_nodes = colored_nodes->tail)
    {
      ret.colored = list_new_list (colored_nodes->head, ret.colored);
    }

  ret.spills = NULL;
  for (; c.spilled_nodes; c.spilled_nodes = c.spilled_nodes->tail)
    {
      printf ("spilled: %s\n", name_temp (c.spilled_nodes->head));
      ret.spills = list_new_list (c.spilled_nodes->head, ret.spills);
    }

  ret.coalesced_moves = c.coalesced_moves;
  ret.coalesced_nodes = c.coalesced_nodes;
  ret.alias = c.alias;

  return ret;
}

static struct col_result
col_color2 (graph_graph      *ig,
            temp_map         *initial,
            temp_temp_list   *regs,
            assem_instr_list *worklist_moves,
            temp_map         *move_list,
            temp_map         *spill_cost)
{
  //your code here.
  struct col_result ret;

  temp_map *precolored = initial;
  temp_map *colors = temp_layer_map (temp_new_map (), initial);
  graph_node_list *spilled_nodes = NULL, *colored_nodes = NULL;
  graph_node_list *nodes = graph_nodes (ig);
  graph_node_list *temps = NULL;

  graph_node_list *nl;
  for (nl = nodes; nl; nl = nl->tail)
    {
      if (temp_lookup (precolored, node_to_temp (nl->head)))
        {
          continue;
        }
      temps = list_new_list (nl->head, temps);
    }

  while (temps != NULL)
    {
      graph_node      *n         = temps->head;
      temp_temp_list  *ok_colors = clone_regs (regs);
      graph_node_list *adjs      = graph_adj(n);
      graph_node      *adj;
      char            *color;

      for (; adjs; adjs = adjs->tail)
        {
          adj = adjs->head;
          if ((color = temp_lookup (colors, node_to_temp (adj))) != NULL)
            {
              temp_temp *color_temp = str_to_color (color, precolored, regs);
              if (color_temp)
                {
                  ok_colors = minus_temp (ok_colors,
                                          list_new_list (color_temp, NULL));
                }
            }
        }

      if (ok_colors == NULL)
        {
          spilled_nodes = list_new_list (n, spilled_nodes);
        } else {
      colored_nodes = list_new_list (n, colored_nodes);
      temp_bind_temp (colors,
                      node_to_temp (n),
                      color_to_str (ok_colors->head, precolored));
    }

    // Next
    temps = temps->tail;
  }

  ret.coloring = colors;

  ret.colored = NULL;
  for (; colored_nodes; colored_nodes = colored_nodes->tail)
    {
      ret.colored = list_new_list (node_to_temp (colored_nodes->head),
                                   ret.colored);
    }

  ret.spills = NULL;
  for (; spilled_nodes; spilled_nodes = spilled_nodes->tail)
    {
      printf("spilled: %s\n",
             name_temp (node_to_temp (spilled_nodes->head)));
      ret.spills = list_new_list (node_to_temp (spilled_nodes->head),
                                  ret.spills);
    }
  ret.alias = graph_new_table ();
  ret.coalesced_moves = NULL;
  ret.coalesced_nodes = NULL;

  return ret;
}
