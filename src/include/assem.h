/**
 * @file assem.h
 * Function prototypes to translate to Assem-instructions using Maximal Munch.
 */

#ifndef _ASSEM_H_
#define _ASSEM_H_

#include "temp.h"
#include "tree.h"
#include "list.h"

typedef struct _assem_targets assem_targets;
typedef        list           assem_instr_list;
typedef struct _assem_instr   assem_instr;
typedef struct _assem_proc    assem_proc;

#include "frame.h"

struct
_assem_targets
{
  temp_label_list *labels;
};

struct
_assem_instr
{
  enum
    {
      I_OPER,
      I_LABEL,
      I_MOVE
    } kind;
  union
  {
    struct
    {
      char           *assem;
      temp_temp_list *dst, *src;
      assem_targets  *jumps;
    } oper;
    struct
    {
      char       *assem;
      temp_label *label;
    } label;
    struct
    {
      char           *assem;
      temp_temp_list *dst, *src;
    } move;
  } u;
};


struct
_assem_proc
{
  char             *prolog;
  assem_instr_list *body;
  char             *epilog;
};

assem_targets *    assem_new_targets      (temp_label_list *labels);

assem_instr *      assem_new_oper        (char           *a,
                                          temp_temp_list *d,
                                          temp_temp_list *s,
                                          assem_targets  *j);

assem_instr *      assem_new_label        (char       *a,
                                           temp_label *label);

assem_instr *      assem_new_move         (char           *a,
                                           temp_temp_list *d,
                                           temp_temp_list *s);

void               assem_print            (FILE        *out,
                                           assem_instr *i,
                                           temp_map    *m);

assem_instr_list * assem_splice           (assem_instr_list *a,
                                           assem_instr_list *b);

void               assem_print_instr_list (FILE             *out,
                                           assem_instr_list *ilist,
                                           temp_map         *m);

assem_proc *       assem_new_proc         (char             *p,
                                           assem_instr_list *b,
                                           char             *e);

//assem_instr_list * assem_codegen          (frm_frame     *frame,
//                                         tree_stm_list *stm_list);

//char *             assem_get_asm_str      (void);

//void               assem_fmt_asm_str      (char *fmt,
//                                         ...);
#endif /* _ASSEM_H_ */
