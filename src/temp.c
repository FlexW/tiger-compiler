/**
 * @file temp.c
 * Functions for creating and managing temps and lables.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/temp.h"
#include "include/util.h"
#include "include/symbol.h"
#include "include/table.h"

#define BUFFER_SIZE 100

struct
_temp_temp
{
int num;
};

struct
_temp_map
{
  tab_table *tab;
  temp_map  *under;
};


static FILE *outfile;

static int labels = 0;
static int temps  = 100;

/* Local functions declarations */

static temp_map * new_map       (tab_table *tab_ptr,
                                 temp_map  *under_ptr);


/**
 * Returns the name of a label as string.
 *
 * @param s Label.
 *
 * @returns Name of label.
 */
char *
temp_label_str (temp_label *s)
{
  return sym_name (s);
}

/**
 * Returns a new temporary from an infinite set of temps.
 *
 * @return New Temp.
 */
temp_temp *
temp_new_temp (void)
{
  char buf[BUFFER_SIZE];
  temp_temp *new_temp = new (sizeof (*new_temp));
  new_temp->num = temps++;

  snprintf(buf, BUFFER_SIZE, "%d", new_temp->num);
  char *s = new (sizeof (strlen (buf) + 1));
  strcpy (s, buf);
  temp_bind_temp (temp_name (), new_temp, s);

  return new_temp;
}

/**
 * Returns a new label from an infinite set of label.
 *
 * @return New Label.
 */
temp_label *
temp_new_label (void)
{
  static char buf[BUFFER_SIZE];
  snprintf(buf, BUFFER_SIZE, "L%d", labels++);

  return temp_named_label (buf);
}

/**
 * Returns a new label whose assembly-language name is name_ptr.
 * New label will only be created if it not exists.
 *
 * @param name_ptr Name for the label.
 *
 * @return New Label.
 */
temp_label *
temp_named_label (char *name_ptr)
{
  return sym_new_symbol (name_ptr);
}

/**
 * Returns map. If map is not created. It will create one.
 *
 * @return The map.
 */
temp_map *
temp_name (void)
{
static temp_map *map = NULL;

if (map == NULL)
   map = temp_new_map ();

 return map;
}


static temp_map *
new_map (tab_table *tab_ptr,
         temp_map  *under_ptr)
{
  temp_map *map = new (sizeof(*map));
  map->tab   = tab_ptr;
  map->under = under_ptr;

  return map;
}

/**
 * Creates a new empty map with one table.
 *
 * @return New created map.
 */
temp_map *
temp_new_map (void)
{
  return new_map (tab_new_table (), NULL);
}

temp_map *
temp_layer_map (temp_map *over_ptr,
                temp_map *under_ptr)
{
if (over_ptr == NULL)
      return under_ptr;
  else
    return new_map (over_ptr->tab,
                    temp_layer_map (over_ptr->under, under_ptr));
}

/**
 * Binds a new temporary into the table.
 *
 * @param map_ptr  The map (table) where the value shoud be binded
 * @param temp_ptr The key for the value to bind.
 * @param str_ptr  The value.
 */
void
temp_bind_temp (temp_map  *map_ptr,
                temp_temp *temp_ptr,
                char      *str_ptr)
{
  tab_bind_value (map_ptr->tab, temp_ptr, str_ptr);
}

/**
 * Looks up a temporary in map.
 * If under map->tab nothing is found it looks under map->under .
 *
 * @param map_ptr  The map to lookup.
 * @param temp_ptr The temporary to look after.
 *
 * @return Name of temp_ptr or NULL.
 */
char *
temp_lookup (temp_map  *map_ptr,
             temp_temp *temp_ptr)
{
char * str;

  str = tab_lookup (map_ptr->tab, temp_ptr);
  if (str != NULL)
    return str;
  else if (map_ptr->under != NULL)
    return temp_lookup (map_ptr->under, temp_ptr);
  else
    return NULL;
}

temp_temp_list *
temp_reverse_list (temp_temp_list *t)
{
  if (t == NULL)
    return t;

  temp_temp_list *tl = NULL;
  for (; t; t = t->tail)
    tl = list_new_list (t->head, tl);

  return tl;
}

bool
temp_equal (temp_temp_list *ta,
            temp_temp_list *tb)
{
  temp_temp *t;
  temp_temp_list *tl = NULL;
  temp_map *m = temp_new_map ();
  int ca = 0, cb = 0;

  for (; ta; ta = ta->tail)
    {
      t = ta->head;
      temp_bind_temp (m, t, "e");
      ca++;
    }

  for (; tb; tb = tb->tail)
    {
      t = tb->head;
      if (temp_lookup (m, t) == NULL)
        {
          return false;
        }
    cb++;
  }
  return (ca == cb);
}

/* Debug functions */

void showit(temp_temp *temp_ptr,
            char      *r_ptr)
{
  fprintf(outfile, "t%d -> %s\n", temp_ptr->num, r_ptr);
}

void
temp_dump_map (FILE     *out_ptr,
               temp_map *map_ptr)
{
  outfile = out_ptr;

  tab_dump (map_ptr->tab, (void (*)(void *, void*))showit);
  if (map_ptr->under)
    {
     fprintf(out_ptr, "---------\n");
     temp_dump_map(out_ptr, map_ptr->under);
  }
}

void
temp_enter_ptr (temp_map  *m,
                temp_temp *t,
                void      *ptr)
{
  assert (m && m->tab);
  tab_bind_value (m->tab, t, ptr);
}

void *
temp_look_ptr (temp_map *m,
               temp_temp *t)
{
  assert (m && m->tab);
  void *s = tab_lookup (m->tab, t);
  if (s)
    return s;
  else if (m->under)
    return temp_look_ptr (m->under, t);
  else
    return NULL;
}

temp_temp_list *
temp_union (temp_temp_list *ta,
            temp_temp_list *tb)
{
  temp_temp *t;
  temp_temp_list *tl = NULL;
  temp_map *m = temp_new_map ();

  for (; ta; ta = ta->tail)
    {
      t = ta->head;
      if (temp_lookup (m, t) == NULL)
        {
          temp_bind_temp (m, t, "u");
          tl = list_new_list (t, tl);
        }
    }
  for (; tb; tb = tb->tail)
    {
      t = tb->head;
      if (temp_lookup (m, t) == NULL)
        {
          temp_bind_temp (m, t, "u");
          tl = list_new_list (t, tl);
        }
    }
  return tl;
}

temp_temp_list *
temp_intersect (temp_temp_list *ta,
                temp_temp_list *tb)
{
  temp_temp *t;
  temp_temp_list *tl = NULL;
  temp_map *m = temp_new_map ();

  for (; ta; ta = ta->tail)
    {
      t = ta->head;
      temp_bind_temp (m, t, "i");
    }
  for (; tb; tb = tb->tail)
    {
      t = tb->head;
      if (temp_lookup (m, t) != NULL)
        {
          tl = list_new_list (t, tl);
        }
    }
  return tl;
}

temp_temp_list *
temp_minus (temp_temp_list *ta,
            temp_temp_list *tb)
{
  temp_temp *t;
  temp_temp_list *tl = NULL;
  temp_map *m = temp_new_map ();

  for (; tb; tb = tb->tail)
    {
      t = tb->head;
      temp_bind_temp (m, t, "m");
    }
  for (; ta; ta = ta->tail)
    {
      t = ta->head;
      if (temp_lookup (m, t) == NULL)
        {
          tl = list_new_list (t, tl);
        }
    }
  return tl;
}

bool
temp_in_list (temp_temp      *t,
              temp_temp_list *tl)
{
  for (; tl; tl = tl->tail)
    {
      if (tl->head == t)
        {
          return true;
        }
    }
  return false;
}
