/**
 * @file temp.h
 *
 */

#ifndef _TEMP_H_
#define _TEMP_H_

#include <stdio.h>

#include "list.h"
#include "symbol.h"

/*
  temp:
  Value that is temporarily held
  in a register; abstract names for local variables.

  label:
  Some machine-language location whose
  exact address is yet to be determined; abstract names
  for static memory addresses
*/

typedef sym_symbol temp_label;

typedef struct _temp_temp temp_temp;
typedef        list       temp_temp_list;
typedef struct _temp_map  temp_map;
typedef        list       temp_label_list;


temp_temp *       temp_new_temp        (void);

temp_label *      temp_new_label      (void);

temp_label *      temp_named_label    (char *name_ptr);

char *            temp_label_str      (temp_label *label_ptr);

temp_map *        temp_new_map        (void);

temp_map *        temp_layer_map      (temp_map *over_ptr,
                                       temp_map *under_ptr);

void              temp_bind_temp      (temp_map *map_ptr,
                                       temp_temp *tail_ptr,
                                       char *str_ptr);

char *            temp_lookup         (temp_map *map_ptr,
                                       temp_temp *temp_ptr);

void              temp_dump_map       (FILE *out_ptr,
                                       temp_map *map_ptr);

temp_map *        temp_name           (void);

temp_temp_list *  temp_reverse_list   (temp_temp_list *t);

#endif /* _TEMP_H_ */
