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
#include "include/regalloc.h"
#include "include/prtree.h"

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
  assem_proc       *proc;
  tree_stm_list    *stm_list;
  assem_instr_list *ilist;

 frm_temp_map = temp_new_map ();
 //print_stm_list (stdout, tree_new_stm_list(body, NULL));

 stm_list = canon_linearize (body);
 stm_list = canon_trace_schedule (canon_basic_blocks (stm_list));

 if (check_cmd_line_arg (PR_TREE))
    {
      print_stm_list (stdout, stm_list);
      fprintf(stdout, "\n");
    }
 ilist  = codegen (frame, stm_list); /* 9 */
 // assem_print_instr_list (out, ilist, temp_layer_map(frm_temp_map, temp_name()));

 struct regalloc_result ra = regalloc_do (frame, ilist);  /* 10, 11 */
 ilist = ra.il;

 ilist = frm_proc_entry_exit2 (ilist);
 proc = frm_proc_entry_exit3 (frame, ilist);

 fprintf(out, "%s\n", proc->prolog);
 assem_print_instr_list (out,
                         proc->body,
                         temp_layer_map (frm_temp_map,
                                         temp_layer_map (ra.coloring,
                                                         temp_name())));
 fprintf(out, "%s\n", proc->epilog);

//  fprintf(out, "BEGIN function\n");
//  assem_printInstrList (out, iList,
//                     Temp_layerMap(F_tempMap,
//                                   Temp_layerMap(ra.coloring, Temp_name())));
//  fprintf(out, "END function\n\n");
}

char *
expand_escapes (const char* src)
{
  char* str = new (2 * strlen (src) + 10);

  char* dest = str;
  char c;

  while ((c = *(src++)))
    {
      switch(c)
        {
        case '\a':
          *(dest++) = '\\';
          *(dest++) = 'a';
          break;
        case '\b':
          *(dest++) = '\\';
          *(dest++) = 'b';
          break;
        case '\t':
          *(dest++) = '\\';
          *(dest++) = 't';
          break;
        case '\n':
          *(dest++) = '\\';
          *(dest++) = 'n';
          break;
        case '\v':
          *(dest++) = '\\';
          *(dest++) = 'v';
          break;
        case '\f':
          *(dest++) = '\\';
          *(dest++) = 'f';
          break;
        case '\r':
          *(dest++) = '\\';
          *(dest++) = 'r';
          break;
        case '\\':
          *(dest++) = '\\';
          *(dest++) = '\\';
          break;
        case '\"':
          *(dest++) = '\\';
          *(dest++) = '\"';
          break;
        default:
          *(dest++) = c;
        }
    }
  *(dest++) = '\\';
  *(dest++) = '0';

  *(dest++) = '\\';
  *(dest++) = '0';

  *(dest++) = '\\';
  *(dest++) = '0';

  *(dest++) = '\0'; /* Ensure nul terminator */
  return str;
}

static void
do_str (FILE       *out,
        char       *str,
        temp_label *label)
{
  fprintf (out, "%s:\n", temp_label_str (label));
  fprintf (out, "    .long 0x%lx\n", strlen (str));
  fprintf (out, "    .ascii \"%s\"\n", expand_escapes (str));
  fprintf (out, "\n");
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
      prabsyn_exp (stdout, root, 4);
      fprintf(stdout, "\n");
    }

  frm_frag_list *frag_list = sem_trans_prog (root);

  if (errm_any_errors)
    return 1;

  /* Convert filename */
  //sprintf (outfile, "%s.s", argv[1]);
  //out = fopen(outfile, "w");

  fprintf(out, ".globl tigermain\n\n");
  fprintf(out, ".text\n\n");
  for (frm_frag_list *fl = frag_list; fl != NULL; fl = fl->tail)
    {
      frm_frag *frag = frag_list->head;
      if (frag->kind == FRM_PROC_FRAG)
        do_proc (out, frag->u.proc.frame, frag->u.proc.body);
    }
  fprintf(out, ".data\n\n");
  for (frm_frag_list *fl = frag_list; fl != NULL; fl = fl->tail)
    {
      frm_frag *frag = frag_list->head;
      if (frag->kind == FRM_STRING_FRAG)
        do_str (out, frag->u.str.str, frag->u.str.label);
    }
  //fclose (out);
  return 0;
}
