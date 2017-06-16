/**
 * @file errormsg.c
 * Functions to print error messages.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "include/util.h"
#include "include/errormsg.h"

typedef struct  _int_list int_list;

struct
_int_list
{
  int       i;
  int_list *rest;
};

extern FILE *yyin;

bool errm_any_errors = false;
int  errm_tok_pos = 0;

static char      *file_name = "";
static int        line_num  = 1;
static int_list  *line_pos  = NULL;


static int_list* new_int_list     (int       i,
                                   int_list *rest_ptr);

//static char*     beautify_err_msg (char* msg_ptr);

/**
 * Increments line number. Updates line_pos .
 */
void
errm_newline (void)
{
  line_num++;
  line_pos = new_int_list (errm_tok_pos, line_pos);
}

/**
 * Prints a error message with line und column number.
 *
 * @param pos     Position where the error occurs.
 * @param msg_ptr Pointer of the format string to print.
 * @param ...     Additional arguments for format string
 */
void
errm_printf (int   pos,
             char *msg_ptr,
             ...)
{
  va_list   ap;
  int_list *lines = line_pos;
  int       num   = line_num;

  errm_any_errors = true;

  while (lines && lines->i >= pos)
    {
      lines = lines->rest;
      num--;
    }

  if (file_name != NULL)
    fprintf (stderr, "%s:", file_name);
  if (lines != NULL)
    fprintf (stderr, "%d.%d: ", num, pos - lines->i);

  va_start (ap, msg_ptr);
  vfprintf (stderr, msg_ptr, ap);
  va_end (ap);
  fprintf (stderr, "\n");
}

/**
 * Prints a error message and then exits the application.
 *
 * @param msg_ptr Message to display.
 * @param ... Arguments for format string.
 */
void
errm_impossible (char* msg_ptr,
                 ...)
{
  va_list ap;

  va_start (ap, msg_ptr);
  vfprintf (stderr, msg_ptr, ap);
  va_end (ap);
  fprintf (stderr, "\nExit now..\n");

  exit (1);
}


/**
 * Resets the error module. Call this before using other errormsg functions.
 *
 * @param File on wich the error messages are about.
 *        Need this for printing file name.
 */
void
errm_reset (char *filename_ptr)
{
  errm_any_errors = false;
  file_name       = filename_ptr;
  line_num        = 1;
  line_pos        = new_int_list (0, NULL);

  yyin = fopen (filename_ptr,"r");
  if (!yyin)
    {
      errm_printf (0,"cannot open");
      exit (1);
    }
}

static int_list*
new_int_list (int       i,
              int_list *rest_ptr)
{
  int_list *l = new (sizeof (*l));

  l->i = i;
  l->rest = rest_ptr;

  return l;
}
