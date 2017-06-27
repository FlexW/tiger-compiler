/**
 * @file main.c
 * Main program.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/assem.h"
#include "include/codegen.h"
#include "include/canon.h"
#include "include/util.h"
#include "include/symbol.h"
#include "include/errormsg.h"
#include "include/prabsyn.h"
#include "include/semant.h"
#include "include/absyn.h"
#include "include/debug.h"


extern int yyparse(void);

extern absyn_exp *absyn_root;
extern int        yydebug;

#define NUM_CMD_LINE_ARGS 3 /* Increment if you add a arg */

/* Valid cmd line args */
#define PR_PARSE "--prparse"
#define PR_ABSYN "--prabsyn"
#define PR_TREE  "--prtree"

/* Global variable for cmd line args */
int    gargc;
char** gargv;

/* Checks if given argument is a valid cmd line arg */
static bool
check_cmd_line_arg (char* arg)
{
  bool is_valid = false;

  for (int i = 0; i < gargc; i++)
    {
      if (!strcmp(arg, gargv[i]))
        is_valid = true;
    }
  return is_valid;
}

/*
  Parse source file fname;
  Return abstract syntax data structure
*/
static absyn_exp*
parse (char *fname_ptr)
{
  errm_reset(fname_ptr);

  if (check_cmd_line_arg(PR_PARSE))
    yydebug = 1;

  if (yyparse() == 0) /* parsing worked */
    return absyn_root;

  fprintf(stderr, "Parsing didn't worked\n");
  exit(1);
}

/* Prints out a procedure in assembly language */
static void
do_proc (FILE      *out,
         frm_frame *frame,
         tree_stm  *body)
{
  frm_temp_map = temp_new_map ();

  tree_stm_list *stm_list = canon_linearize (body);
  stm_list = canon_trace_schedule (canon_basic_blocks (stm_list));

  assem_instr_list *instr_list = codegen (frame, stm_list);

  fprintf (out, "BEGIN %s\n", temp_label_str (frm_name (frame)));
  assem_print_instr_list (out,
                          instr_list,
                          temp_layer_map (frm_temp_map, temp_name ()));
  fprintf (out, "END %s\n\n", temp_label_str (frm_name (frame)));
}

int
main (int    argc,
      char **argv)
{
  char  outfile[100];
  FILE *out = stdout;
  gargc = argc;
  gargv = argv;

  if (argc < 2)
    {
      fprintf (stderr, "usage: %s filename\n", argv[0]);
      exit(1);
    }

  absyn_exp *root = parse (argv[1]);
  if (check_cmd_line_arg (PR_ABSYN))
    {
      //prabsyn_exp (stdout, root, 4);
      fprintf(stdout, "\n");
    }

  frm_frag_list *frag_list = sem_trans_prog (root);

  if (errm_any_errors)
    return 1;

  /* Convert filename */
  //sprintf (outfile, "%s.s", argv[1]);
  //out = fopen(outfile, "w");

  for (; frag_list != NULL; frag_list = frag_list->tail)
    {
      frm_frag *frag = frag_list->head;
      switch (frag->kind)
        {
        case FRM_PROC_FRAG:
          do_proc (out, frag->u.proc.frame, frag->u.proc.body);
          break;

        case FRM_STRING_FRAG:
          fprintf (out, "%s\n", frag->u.str.str);
          break;

        default:
          assert (0);
        }
    }
  //fclose (out);
  return 0;
}
