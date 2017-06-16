/**
 * @file errormsg.h
 * Helper functions for displaying correct error messages
 *
 * Global functions and variables start with errm_
 */

#ifndef _ERRORMSG_H_
#define _ERRORMSG_H_

#include <stdbool.h>

extern bool errm_any_errors;
extern int  errm_tok_pos;

void errm_printf     (int   pos,
                      char *msg_ptr,
                      ...);

void errm_impossible (char* msg_ptr,
                      ...);

void errm_reset      (char* filename_ptr);

void errm_newline    (void);

#endif
