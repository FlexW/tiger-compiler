/**
 * @file x64frame.c
 * Creates stack frame layout for the x64 architecture.
 */

#include <assert.h>

#include "include/frame.h"
#include "include/errormsg.h"
#include "include/util.h"


#define MAX_REG_ARG 6 /* Max arg that goes trough registers on func call */

const int frm_word_size = 4;

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
  frm_access_list *formals;
  frm_access_list *locals;
  int              locals_cnt;
};

static frm_access *      in_frame              (int offset);

static frm_access *      in_reg                (temp_temp *reg_ptr);

static int               calc_offset           (int num_of_arg);

static frm_access *      alloc_formal          (int  num_of_arg,
                                                bool escape);

static frm_access_list * formals_esc_to_access (util_bool_list *formals_ptr);


/**
 * Returns the frame pointer register.
 *
 * @return Frame pointer register.
 */
temp_temp *
frm_frame_pointer (void)
{
  static temp_temp *frame_pointer = NULL;

  if (frame_pointer == NULL)
    frame_pointer = temp_new_temp ();

  return frame_pointer;
}

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
    }
  assert (0);
  return NULL;
}

frm_frame *
frm_new_frame (temp_label     *name_ptr,
               util_bool_list *formals_ptr)
{
  frm_frame *frame   = new (sizeof (*frame));
  frame->start_label = name_ptr;
  frame->formals     = formals_esc_to_access (formals_ptr);
  frame->locals      = NULL;
  frame->locals_cnt  = 2; /* Return and frame pointer adress */

  return frame;
}

/* Converts a bool list into a frm_access_list */
static frm_access_list *
formals_esc_to_access (util_bool_list *bool_list)
{
  frm_access_list *formals = NULL, *sformals = NULL;

  for (int i = 0; bool_list != NULL; bool_list = bool_list->tail, i++)
    {
      bool       *b      = bool_list->head;
      frm_access *access = alloc_formal (i + 1, b);

      if (formals == NULL)
        sformals = formals = list_new_list (access, NULL);
      else
        formals = formals->tail = list_new_list (access, NULL);
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
    access = in_frame (-(calc_offset (frame_ptr->locals_cnt++)));
  else
    access = in_reg (temp_new_temp ());

  /* Add element to frame struct */
  list_new_list (access, frame_ptr->locals);

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
  assert (0);
  return NULL;
}

tree_stm *
frm_proc_entry_exit3 (void)
{
  assert (0);
  return NULL;
}

frm_frag *
frm_new_str_frag (temp_label *label_ptr,
                  char       *str_ptr)
{
  frm_frag *str = new (sizeof (*str));

  str->u.str.str   = str_ptr;
  str->u.str.label = label_ptr;

  return str;
}

frm_frag *
frm_new_proc_frag (tree_stm  *body_ptr,
                   frm_frame *frame_ptr)
{
  frm_frag *proc = new (sizeof (*proc));

  proc->u.proc.body  = body_ptr;
  proc->u.proc.frame = frame_ptr;

  return proc;
}

/**
 * Returns a function return value location.
 *
 * @return Function return location.
 */
temp_temp *
frm_ret_val_location (void)
{
  static temp_temp *ret = NULL;

  if (ret == NULL)
    ret = temp_new_temp ();

  return ret;
}
