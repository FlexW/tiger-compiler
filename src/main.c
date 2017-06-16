/**
 * @file main.c
 * Main program.
 */

#include <stdio.h>
#include <stdlib.h>

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


/*
  Parse source file fname;
  Return abstract syntax data structure
*/
absyn_exp*
parse(char *fname_ptr)
{
  errm_reset(fname_ptr);
  //yydebug = 1;
  if (yyparse() == 0) /* parsing worked */
    return absyn_root;
  else
    return NULL;
}

int
main (int    argc,
      char **argv)
{
  FILE *fp;

  if (argc!=2)
    {
      fprintf (stderr,"usage: a.out filename\n");
      exit(1);
    }

  //parse (argv[1]);
  //prabsyn_exp (stdout, parse (argv[1]), 4);
  sem_trans_prog (parse (argv[1]));
  printf("done!\n");
  return 0;
}
