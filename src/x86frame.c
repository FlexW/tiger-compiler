/**
 * @file x64frame.c
 * Creates stack frame layout for the x64 architecture.
 */

#include <assert.h>
#include <string.h>

#include "include/assem.h"
#include "include/frame.h"
#include "include/errormsg.h"
#include "include/util.h"


#define MAX_REG_ARG 6 /* Max arg that goes trough registers on func call */

const int frm_word_size = 4;

temp_map * frm_temp_map = NULL;

static temp_temp * eax = NULL;
static temp_temp * ecx = NULL;
static temp_temp * edx = NULL;
static temp_temp * ebx = NULL;
static temp_temp * esi = NULL;
static temp_temp * edi = NULL;

static temp_temp * fp = NULL;
static temp_temp * sp = NULL;
static temp_temp * zero = NULL;
static temp_temp * ra = NULL;
static temp_temp * rv = NULL;

static temp_temp_list *registers = NULL;
static temp_temp_list *specialregs = NULL;

/**
 * Saves information about how to access a variable
 * in the stack frame (register or stack).
 */
struct
_frm_access
{
  enum
    {
      IN_FRAME,
      IN_REG
    } kind;

  union {
    int        offset; /* IN_FRAME */
    temp_temp *reg;    /* IN_REG */
  } u;
};

/**
 * Holds information about a functions stackframe
 *
 * start_label: Machine code start.
 * formals:     The formals of the function.
 * locals:      The locals (so far) of the function.
 * locals_cnt:  Number of locals.
 */
struct
_frm_frame
{
  temp_label      *start_label;
  temp_map        *temp;
  frm_access_list *formals;
  frm_access_list *locals;
  //int              locals_cnt;
};

static frm_access *      in_frame              (int offset);

static frm_access *      in_reg                (temp_temp *reg_ptr);

static int               calc_offset           (int num_of_arg);

static frm_access *      alloc_formal          (int  num_of_arg,
                                                bool escape);

static frm_access_list * formals_esc_to_access (util_bool_list *formals_ptr);

static void              bind_temp             (temp_temp *label,
                                                char      *name);

frm_frame_list *
frm_new_frame_list (frm_frame *head,
                    frm_frame_list *tail)
{
  frm_frame_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

frm_frag_list *
frm_new_frag_list (frm_frag *head,
                   frm_frag_list *tail)
{
  frm_frag_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

frm_access_list *
frm_new_access_list (frm_access *head,
                     frm_access_list *tail)
{
  frm_access_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
}

/**
 * Returns the frame pointer register.
 *
 * @return Frame pointer register.
 *//*
temp_temp *
frm_frame_pointer (void)
{
  static temp_temp *frame_pointer = NULL;

  if (frame_pointer == NULL)
    {
      frame_pointer = temp_new_temp ();
      bind_temp (frame_pointer, "FP");
    }
  return frame_pointer;
  }*/

/**
 * Gets a list with all named temporaries.
 *
 * @returns Map.
 *//*
temp_map *
frm_get_temp_map (void)
{
  static temp_map *frm_temp_map = NULL;

  if (frm_temp_map == NULL)
    frm_temp_map = temp_new_map ();

  return frm_temp_map;
  }*/

/**
 * Turns a frm_access into the intermediate tree representation.
 *
 * @param access_ptr        The access structure.
 * @param frame_pointer_ptr The frame pointer where the access lives in.
 *
 * @return The intermediate tree representation.
 */
tree_exp *
frm_exp (frm_access *access_ptr,
         tree_exp   *frame_pointer_ptr)
{
  switch (access_ptr->kind)
    {
    case IN_REG: /* Variable is in register */
      return tree_new_temp (access_ptr->u.reg);

    case IN_FRAME:
      {
        /* Variable is on stack, calculate offset */
        tree_exp *offset = tree_new_const (access_ptr->u.offset);
        tree_exp *bin_op = tree_new_bin_op (TREE_PLUS,
                                            frame_pointer_ptr,
                                            offset);
        return tree_new_mem (bin_op);
      }

    default:
      assert (0);
    }
  return NULL;
}

static frm_frame_list * frame_stack = NULL;

frm_frame *
frm_new_frame (temp_label     *name_ptr,
               util_bool_list *formals_ptr)
{
  /*
    %ebp:     old %ebp
    %ebp + 4: static link
    arguments start from %ebp + 12:
    local variables start from %ebp - 4 - 12 (calle saves)
   */
  frm_frame *frame   = new (sizeof (*frame));
  frame->start_label = name_ptr;
  //frame->formals     = formals_esc_to_access (formals_ptr);
  int              offset     = 8;
  util_bool_list  *formal_esc = formals_ptr;
  frm_access_list *formal     = NULL;
  while (formal_esc)
    {
      offset += 4;
      formal = frm_new_access_list (in_frame (offset), formal);
      formal_esc = formal_esc->tail;
    }
  frame->formals = formal;
  frame->locals  = NULL;
  frame->temp    = temp_new_map ();

  frame_stack = frm_new_frame_list (frame, frame_stack);
  //frame->locals_cnt  = 2; /* Return and frame pointer adress */

  return frame;
}

/* Converts a bool list into a frm_access_list */
static frm_access_list *
formals_esc_to_access (util_bool_list *bool_list)
{
  frm_access_list *formals = NULL, *sformals = NULL;

  for (int i = 0; bool_list != NULL; bool_list = bool_list->tail, i++)
    {
      bool        b      = bool_list->head;
      frm_access *access = alloc_formal (i + 1, b);

      if (formals == NULL)
        sformals = formals = frm_new_access_list (access, NULL);
      else
        formals = formals->tail = frm_new_access_list (access, NULL);
    }
  return sformals;
}

/**
 * Allocates a local variable on the frame (true) or register (false).
 *
 * @param escape    true if on the frame, false if in register.
 *
 * @return frm_access struct with an offset from the frame pointer or register.
 */
frm_access *
frm_alloc_local (frm_frame *frame_ptr,
                 bool       escape)
{
  frm_access *access;

  /* Allocate on frame or register */
  if (escape)
    {
      //access = in_frame (-(calc_offset (frame_ptr->locals_cnt++)));
      // Locals start from %ebp - 4 - 12 (callee save)
      int              offset = -4 - 12;
      frm_access_list *locals = frame_ptr->locals;
      while (locals != NULL)
        {
          frm_access *local = locals->head;
          if (local->kind == IN_FRAME)
            offset -= 4;
          locals = locals->tail;
        }

      frm_access *l = in_frame (offset);
      frame_ptr->locals = frm_new_access_list (l, frame_ptr->locals);
      return l;
    }
  else
    {
      access = in_reg (temp_new_temp ());
      /* Add element to frame struct */
      frm_new_access_list (access, frame_ptr->locals);
    }
  return access;
}

/* Allocates a formal on stack or register */
static frm_access *
alloc_formal (int  num_of_arg,
              bool escape)
{
  if (escape || num_of_arg > MAX_REG_ARG)
    return in_frame (calc_offset (num_of_arg));

  return in_reg (temp_new_temp ());
}

int
frm_access_offset (frm_access *a)
{
  if (a->kind != IN_FRAME)
    errm_printf (0, "Offset of a reg access is invalid");

  return a->u.offset;
}

temp_temp *
frm_access_reg (frm_access *a)
{
  if (a->kind != IN_REG)
    errm_printf (0, "Reg of a frame access is invalid");

  return a->u.reg;
}

char *
frm_string (temp_label *lab,
            char       *str)
{
  char *buf = new (sizeof(char) * (strlen(str) + 100));
  sprintf(buf, "%s: .ascii \"%s\"\n", temp_label_str (lab), str);
  return buf;
}


/*
  Indicates a memory location at offset offset from the frame pointer.
*/
static frm_access *
in_frame (int offset)
{
  frm_access *access = new (sizeof (*access));

  access->kind     = IN_FRAME;
  access->u.offset = offset;

  return access;
}

static assem_instr_list *
append_callee_save (assem_instr_list *il)
{
  temp_temp_list *callee_saves = temp_reverse_list (frm_callee_saves ());

  assem_instr_list *ail = il;
  for (; callee_saves; callee_saves = callee_saves->tail)
    {
      ail = assem_new_instr_list (assem_new_oper ("pushl `s0\n",
                                                  temp_new_temp_list (frm_sp (), NULL),
                                                  temp_new_temp_list (callee_saves->head,
                                                                 NULL),
                                                  NULL),
                           ail);
    }
  return ail;
}

static assem_instr_list *
restore_callee_save (assem_instr_list *il)
{
  temp_temp_list *callee_saves = frm_callee_saves ();

  assem_instr_list *ail = NULL;
  for (; callee_saves; callee_saves = callee_saves->tail)
    {
      ail = assem_new_instr_list (assem_new_oper ("popl `s0\n",
                                                  temp_new_temp_list (frm_sp (), NULL),
                                                  temp_new_temp_list (callee_saves->head,
                                                          NULL),
                                           NULL),
                           ail);
    }
  return assem_splice (ail, il);
}

/*
  Indicates that local variable or formal will be held in reg_ptr register
 */
static frm_access *
in_reg (temp_temp *reg_ptr)
{
  frm_access *access = new (sizeof (*access));

  access->kind  = IN_REG;
  access->u.reg = reg_ptr;

  return access;
}

/* Calculates the offset from frame pointer */
static int
calc_offset (int num_of_arg)
{
  return num_of_arg * frm_word_size;
}

/**
 * Extract formals from frame.
 *
 * @param frame_ptr The frame.
 *
 * @return List of the accesses.
 */
frm_access_list *
frm_formals (frm_frame *frame_ptr)
{
  return frame_ptr->formals;
}

/**
 * Creates the intermediate tree represenation for an external
 * function call.
 *
 * @param name_ptr Name of function.
 * @param args_ptr Function arguments.
 */
tree_exp *
frm_external_call (char          *name_ptr,
                   tree_exp_list *args_ptr)
{
  return tree_new_call (tree_new_name (temp_named_label (name_ptr)), args_ptr);
}

tree_stm *
frm_proc_entry_exit1 (frm_frame *frame_ptr,
                      tree_stm  *stm_ptr)
{
  frame_stack = frame_stack->tail;
  return stm_ptr;
}

static temp_temp_list *return_sink = NULL;

assem_instr_list *
frm_proc_entry_exit2 (assem_instr_list *body)
{
  temp_temp_list *callee_saves = frm_callee_saves();
  if (!return_sink)
    return_sink = temp_new_temp_list (frm_ra(),
                                 temp_new_temp_list (frm_sp (), callee_saves));

  char inst_add[128];
  int frame_size = 100;//frameSize(frame);
  sprintf (inst_add, "addl $%d, `s0\n", frame_size);

  return assem_splice (body,
                       assem_new_instr_list (assem_new_oper (string_new (inst_add),
                                                             temp_new_temp_list (frm_sp (),
                                                                                 NULL),
                                                             temp_new_temp_list (frm_sp (),
                                                                                 NULL),
                                                             NULL),
                                      restore_callee_save (assem_new_instr_list (assem_new_oper ("leave\n", temp_new_temp_list (frm_sp(), temp_new_temp_list (frm_fp(), NULL)), temp_new_temp_list (frm_sp(), NULL), NULL), assem_new_instr_list (assem_new_oper ("ret\n", NULL, return_sink, NULL), NULL)))));
}

assem_proc *
frm_proc_entry_exit3 (frm_frame        *frame,
                      assem_instr_list *body)
{
  char buf[1024], inst_lbl[128], inst_sub[128];
  int frame_size = 100;//frameSize(frame);

  sprintf(buf, "# PROCEDURE %s\n", sym_name (frame->start_label));
  sprintf(inst_lbl, "%s:\n", sym_name(frame->start_label));
  // sprintf(buf, "%s    pushl %%ebp\n", buf);
  // sprintf(buf, "%s    movl %%esp, %%ebp\n", buf);
  sprintf(inst_sub, "subl $%d, `s0\n", frame_size);

  body = assem_new_instr_list (assem_new_label(string_new (inst_lbl), frame->start_label),
            assem_new_instr_list (assem_new_oper ("pushl `s0\n", temp_new_temp_list (frm_fp(), temp_new_temp_list (frm_sp(), NULL)), temp_new_temp_list (frm_fp(), NULL), NULL),
              assem_new_instr_list (assem_new_move ("movl `s0, `d0\n", temp_new_temp_list (frm_fp(), NULL), temp_new_temp_list (frm_sp(), NULL)),
                append_callee_save(
                                   assem_new_instr_list (assem_new_oper (string_new (inst_sub), temp_new_temp_list (frm_sp(), NULL), temp_new_temp_list (frm_sp(), NULL), NULL),
                                  body)))));
  return assem_new_proc (string_new (buf), body, "# END\n");
}

frm_frag *
frm_new_str_frag (temp_label *label_ptr,
                  char       *str_ptr)
{
  frm_frag *str = new (sizeof (*str));

  str->kind        = FRM_STRING_FRAG;
  str->u.str.str   = str_ptr;
  str->u.str.label = label_ptr;

  return str;
}

frm_frag *
frm_new_proc_frag (tree_stm  *body_ptr,
                   frm_frame *frame_ptr)
{
  frm_frag *proc = new (sizeof (*proc));

  proc->kind         = FRM_PROC_FRAG;
  proc->u.proc.body  = body_ptr;
  proc->u.proc.frame = frame_ptr;

  return proc;
}

/**
 * Returns a function return value location.
 *
 * @return Function return location.
 *//*
temp_temp *
frm_ret_val_location (void)
{
  static temp_temp *ret = NULL;

  if (ret == NULL)
    {
      ret = temp_new_temp ();
      bind_temp (ret, "RV");
    }
  return ret;
}*/

/**
 * Determines if a given access is allocated on the frame
 * or in a register.
 *
 * @param access Frame access struct.
 *
 * @returns True if is in register. False if on stack.
 */
bool
frm_is_access_in_reg (frm_access *access)
{
  switch (access->kind)
    {
    case IN_FRAME:
      return false;
    case IN_REG:
      return true;
    default:
      assert (0);
    }
}
/*
static temp_temp_list *
caller_saves ()
{
   // assist-function of calldefs()

    temp_temp *rcx = temp_new_temp (),
      // *rbx = temp_new_temp (),
              *rdx = temp_new_temp (),
              *rdi = temp_new_temp (),
              *rsi = temp_new_temp (),
              *r8  = temp_new_temp (),
              *r9  = temp_new_temp ();

    //temp_bind_temp (frm_get_temp_map (), rbx, "rbx");
    temp_bind_temp (frm_get_temp_map (), rcx, "rcx");
    temp_bind_temp (frm_get_temp_map (), rdx, "rdx");
    temp_bind_temp (frm_get_temp_map (), rdi, "rdi");
    temp_bind_temp (frm_get_temp_map (), rsi, "rsi");
    temp_bind_temp (frm_get_temp_map (), r8, "r8");
    temp_bind_temp (frm_get_temp_map (), r9, "r9");

    temp_temp_list * list = list_new_list (rsi, NULL);
    list                  = list_new_list (rdi, list);
    list                  = list_new_list (rdx, list);
    list                  = list_new_list (rcx, list);
    list                  = list_new_list (r8, list);
    list                  = list_new_list (r9, list);
    //list                  = list_new_list (rbx, list);
    list                  = list_new_list (frm_ret_val_location (), list);

    return list;;
}
*/
/**
 * Gets a list of all registers that may be modified.
 *
 * @return List of register.
 */
temp_temp_list *
frm_caller_saves (void)
{
  if (fp == NULL)
    frm_init_registers();

  return //list_new_list(eax,
            temp_new_temp_list (edx,
              temp_new_temp_list (ecx, NULL));//);
}

temp_temp_list *
frm_callee_saves(void)
{
  if (fp == NULL)
    frm_init_registers();

  return temp_new_temp_list (ebx,
            temp_new_temp_list (esi,
              temp_new_temp_list (edi, NULL)));
}

temp_temp_list *
frm_registers(void)
{
  if (fp == NULL)
    frm_init_registers();

  return //list_new_list(eax,
            temp_new_temp_list (ecx,
              temp_new_temp_list (edx,
                temp_new_temp_list (ebx,
                  temp_new_temp_list (esi,
                    temp_new_temp_list (edi, NULL)))));//);
}


temp_temp *
frm_fp (void)
{
  if (fp == NULL)
    frm_init_registers();
  return fp;
}

temp_temp *
frm_sp(void)
{
  if (sp == NULL)
    frm_init_registers();

  return sp;
}

// Zero register (not available in x86)
temp_temp *
frm_zero (void)
{
  if (zero == NULL)
    frm_init_registers();

  return zero;
}

// Return address (not available in x86)
temp_temp *
frm_ra(void)
{
  if (ra == NULL)
    frm_init_registers();

  return ra;
}

// Return value
temp_temp *
frm_rv(void)
{
  if (rv == NULL)
    frm_init_registers();

  return rv;
}

temp_temp *
frm_eax(void)
{
  if (eax == NULL)
    frm_init_registers();

  return eax;
}

temp_temp *
frm_edx(void)
{
  if (edx == NULL)
    frm_init_registers();

  return edx;
}

void
frm_init_registers (void)
{
  fp = temp_new_temp ();
  sp = temp_new_temp ();
  zero = temp_new_temp ();
  ra = temp_new_temp ();
  rv = temp_new_temp ();
  eax = temp_new_temp ();
  ecx = temp_new_temp ();
  edx = temp_new_temp ();
  ebx = temp_new_temp ();
  esi = temp_new_temp ();
  edi = temp_new_temp ();

  temp_bind_temp(temp_name (), eax, "%eax");
  temp_bind_temp(temp_name (), ecx, "%ecx");
  temp_bind_temp(temp_name (), edx, "%edx");
  temp_bind_temp(temp_name (), ebx, "%ebx");
  temp_bind_temp(temp_name (), esi, "%esi");
  temp_bind_temp(temp_name (), edi, "%edi");

  specialregs = temp_new_temp_list (rv,
                  temp_new_temp_list (fp,
                    temp_new_temp_list (ra, NULL)));
}

temp_map *
frm_initial_registers (frm_frame *f)
{
  temp_map *m = temp_new_map ();

  temp_bind_temp(m, fp, "%ebp");
  temp_bind_temp(m, sp, "%esp");
  temp_bind_temp(m, rv, "%eax");

  temp_bind_temp(m, eax, "%eax");
  temp_bind_temp(m, ecx, "%ecx");
  temp_bind_temp(m, edx, "%edx");
  temp_bind_temp(m, ebx, "%ebx");
  temp_bind_temp(m, esi, "%esi");
  temp_bind_temp(m, edi, "%edi");

  return m;
}

temp_label *
frm_name (frm_frame *frame)
{
  return frame->start_label;
}

/** Binds a temporary in a map to specified name */
static void
bind_temp (temp_temp *temp,
           char      *name)
{
  temp_bind_temp (frm_temp_map, temp, name);
}

tree_exp *
frm_static_link_exp (tree_exp *frame_ptr)
{
  // static link at fp + 8
  return tree_new_bin_op (TREE_PLUS,
                          frame_ptr,
                          tree_new_const (2 * frm_word_size));
}

tree_exp *
frm_upper_static_link_exp (tree_exp *static_link)
{
  return tree_new_mem (static_link);
}

tree_exp *
frm_exp_with_static_link (frm_access *acc,
                          tree_exp   *static_link)
{
  if (acc->kind == IN_REG)
    {
      return tree_new_temp (frm_access_reg (acc));
    }
  return tree_new_mem (tree_new_bin_op(TREE_PLUS,
                                       static_link,
                                       tree_new_const (frm_access_offset(acc) - 8)));
}
