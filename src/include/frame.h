/**
 * @file frame.h
 * Provides a machine independet interface for creating frames.
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include <stdbool.h>

#include "frame.h"
#include "list.h"
#include "temp.h"
#include "util.h"
#include "tree.h"
#include "assem.h"

typedef struct _frm_frag   frm_frag;
typedef        list        frm_frag_list;
typedef struct _frm_frame  frm_frame;
typedef struct _frm_access frm_access;
typedef        list        frm_access_list;

extern const int frm_word_size;

extern temp_map *frm_temp_map;

struct
_frm_frag
{
  enum
    {
      FRM_STRING_FRAG,
      FRM_PROC_FRAG
    } kind;

  union
  {
    struct
    {
      temp_label *label;
      char       *str;
    } str;

    struct
    {
      tree_stm  *body;
      frm_frame *frame;
    } proc;
  } u;
};

frm_frag *         frm_new_str_frag     (temp_label *label_ptr,
                                         char       *str_ptr);

frm_frag *         frm_new_proc_frag    (tree_stm  *body_ptr,
                                         frm_frame *frame_ptr);

//temp_temp *        frm_frame_pointer    (void);

tree_exp *         frm_exp              (frm_access *acces_ptr,
                                         tree_exp   *frame_pointer_ptr);

frm_frame *        frm_new_frame        (temp_label     *name_ptr,
                                         util_bool_list *formals_ptr);

temp_label *       frm_name             (frm_frame *frame_ptr);

frm_access_list *  frm_formals          (frm_frame *frame_ptr);

frm_access *       frm_alloc_local      (frm_frame *frame_ptr,
                                         bool       escape);

tree_exp *         frm_external_call    (char          *name_ptr,
                                         tree_exp_list *args_ptr);

//temp_temp *        frm_ret_val_location (void);

tree_stm *         frm_proc_entry_exit1 (frm_frame *frame_ptr,
                                         tree_stm  *stm_ptr);

assem_instr_list * frm_proc_entry_exit2 (assem_instr_list *body);

assem_proc *       frm_proc_entry_exit3 (frm_frame        *frame,
                                         assem_instr_list *body);

//temp_map *         frm_get_temp_map     (void);

bool               frm_is_access_in_reg (frm_access *access);

//temp_temp_list *   frm_call_defs        (void);

temp_temp *        frm_fp                (void);

temp_temp *        frm_sp                (void);

temp_temp *        frm_zero              (void);

temp_temp *        frm_ra                (void);

temp_temp *        frm_rv                (void);

temp_temp *        frm_eax               (void);

temp_temp *        frm_edx               (void);

void               frm_init_registers    (void);

temp_map *         frm_initial_registers (frm_frame *f);

temp_temp_list *   frm_caller_saves      (void);

temp_temp_list *   frm_callee_saves      (void);

int                frm_access_offset     (frm_access *a);

temp_temp_list *   frm_registers         (void);

tree_exp *         frm_upper_static_link_exp (tree_exp *static_link);

tree_exp *         frm_static_link_exp       (tree_exp *frame_ptr);

tree_exp *         frm_exp_with_static_link (frm_access *acc,
                                             tree_exp   *static_link);
#endif /* _FRAME_H_ */
