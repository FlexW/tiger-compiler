/**
 * @file temp.c
 * Functions for creating and managing temps and lables.
 */

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

static temp_map * new_empty_map (void);

static temp_map * new_map       (tab_table *tab_ptr,
                                 temp_map  *under_ptr);


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
  temp_bind_temp (temp_name (), new_temp, buf);

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
  char buf[BUFFER_SIZE];
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
   map = new_empty_map ();

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
static temp_map *
new_empty_map (void)
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
 * Binds a new temprary into the table.
 *
 * @map_ptr  The map (table) where the value shoud be binded
 * @temp_ptr The key for the value to bind.
 * @str_ptr  The value.
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
temp_lookup (temp_map *map_ptr,
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

/* Debug functions */

void showit(temp_temp *temp_ptr,
            char      *r_ptr)
{
  fprintf(outfile, "t%d -> %s\n", temp_ptr->num, r_ptr);
}

void
temp_dump_map(FILE *out_ptr,
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
