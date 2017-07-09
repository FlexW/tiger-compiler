/**
 * @file canon.h
 * Definitions for canon.c which converts the intermediate tree language into
 * basic blocks and traces.
 */

#ifndef _CANON_H_
#define _CANON_H_

#include "list.h"
#include "temp.h"
#include "tree.h"

typedef struct _canon_stmlist_list canon_stmlist_list;
typedef struct _canon_block        canon_block;

struct
_canon_stmlist_list
{
  tree_stm_list      *head;
  canon_stmlist_list *tail;
};

struct _canon_block
{
  canon_stmlist_list *stm_lists;
  temp_label         *label;
};

canon_stmlist_list * canon_new_stmlist_list (tree_stm_list      *head,
                                             canon_stmlist_list *tail);

tree_stm_list * canon_linearize (tree_stm *stm);
        /* From an arbitrary Tree statement, produce a list of cleaned trees
	   satisfying the following properties:
	      1.  No SEQ's or ESEQ's
	      2.  The parent of every CALL is an EXP(..) or a MOVE(TEMP t,..)
        */

canon_block    canon_basic_blocks (tree_stm_list *stm_list);
        /* basicBlocks : Tree.stm list -> (Tree.stm list list * Tree.label)
	       From a list of cleaned trees, produce a list of
	 basic blocks satisfying the following properties:
	      1. and 2. as above;
	      3.  Every block begins with a LABEL;
              4.  A LABEL appears only at the beginning of a block;
              5.  Any JUMP or CJUMP is the last stm in a block;
              6.  Every block ends with a JUMP or CJUMP;
           Also produce the "label" to which control will be passed
           upon exit.
        */

tree_stm_list * canon_trace_schedule (canon_block block);
         /* traceSchedule : Tree.stm list list * Tree.label -> Tree.stm list
            From a list of basic blocks satisfying properties 1-6,
            along with an "exit" label,
	    produce a list of stms such that:
	      1. and 2. as above;
              7. Every CJUMP(_,t,f) is immediately followed by LABEL f.
            The blocks are reordered to satisfy property 7; also
	    in this reordering as many JUMP(T.NAME(lab)) statements
            as possible are eliminated by falling through into T.LABEL(lab).
         */

#endif /* _CANON_H_ */
