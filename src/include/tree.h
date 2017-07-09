/**
 * @file tree.h
 * Declarations for intermediate representation (IR) trees.
 */

#ifndef _TREE_H_
#define _TREE_H_

//#include "list.h"
#include "temp.h"

typedef struct _tree_stm      tree_stm;
typedef struct _tree_exp      tree_exp;
typedef struct _tree_exp_list tree_exp_list;
typedef struct _tree_stm_list tree_stm_list;

struct
_tree_exp_list
{
  tree_exp *head;
  tree_exp_list *tail;
};

struct
_tree_stm_list
{
  tree_stm *head;
  tree_stm_list *tail;
};

/* Binary operators */
typedef enum
  {
    TREE_PLUS,
    TREE_MINUS,
    TREE_TIMES,
    TREE_DIVIDE,
    TREE_AND,
    TREE_OR,
    TREE_LSHIFT,
    TREE_RSHIFT,
    TREE_ARSHIFT,
    TREE_XOR
  } tree_bin_op;

typedef enum
  {
    TREE_EQ,
    TREE_NEQ,
    TREE_LT,
    TREE_GT,
    TREE_LE,
    TREE_GE,
    TREE_ULT,
    TREE_ULE,
    TREE_UGT,
    TREE_UGE
  } tree_rel_op;

/**
 * Structure to hold information about a statement in the intermediate language.
 * A statement is everything that performs control flow and side effects.
 */
struct
_tree_stm
{
  enum
    {
      TREE_SEQ,   /* Sequentiel expression */
      TREE_LABEL, /* Machine code address label */
      TREE_JUMP,  /* Do jump to label */
      TREE_CJUMP, /* Conditional jump */
      TREE_MOVE,  /* Move value in memory or reg */
      TREE_EXP    /* Evaluate exp, discard result */
    } kind;

  union
  {
    temp_label *label;
    tree_exp   *exp;

    struct
    {
      tree_stm *left;
      tree_stm *right;
    } seq;

    struct
    {
      tree_exp        *exp;
      temp_label_list *jumps;
    } jmp;

    struct
    {
      tree_rel_op op;
      tree_exp   *left;
      tree_exp   *right;
      temp_label *truee;
      temp_label *falsee;
    } cjump;

    struct
    {
      tree_exp *dst;
      tree_exp *src;
    } move;
  } u;
};

/**
 * Structure to hold infromation about a expression in intermediate language.
 * A expression is everything that computes a value
 * (with possible side effects).
 */
struct _tree_exp
{
  enum
    {
      TREE_BINOP, /* Binary operator */
      TREE_MEM,   /* Memory */
      TREE_TEMP,  /* Temporary (Register) */
      TREE_ESEQ,  /* List of expressions */
      TREE_NAME,  /* Symbolic Constant */
      TREE_CONST, /* Integer constant */
      TREE_CALL   /* Procedure call */
    } kind;

  union
  {
    tree_exp   *mem;
    temp_temp  *temp;
    temp_label *name;
    int         constt;

    struct
    {
      tree_bin_op op;
      tree_exp   *left;
      tree_exp   *right;
    } bin_op;

    struct {
      tree_stm *stm;
      tree_exp *exp;
    } eseq;

    struct
    {
      tree_exp      *fun;
      tree_exp_list *args;
    } call;
  } u;
};


tree_stm * tree_new_seq   (tree_stm*,
                           tree_stm*);

tree_stm * tree_new_label (temp_label*);

tree_stm * tree_new_jump  (tree_exp*,
                           temp_label_list*);

tree_stm * tree_new_cjump (tree_rel_op,
                           tree_exp*,
                           tree_exp*,
                           temp_label*,
                           temp_label*);

tree_stm * tree_new_move  (tree_exp*,
                           tree_exp*);

tree_stm * tree_new_exp   (tree_exp*);


tree_exp * tree_new_bin_op (tree_bin_op,
                           tree_exp*,
                           tree_exp*);

tree_exp * tree_new_mem   (tree_exp*);

tree_exp * tree_new_temp  (temp_temp*);

tree_exp * tree_new_eseq  (tree_stm*,
                           tree_exp*);

tree_exp * tree_new_name  (temp_label*);

tree_exp * tree_new_const (int);

tree_exp * tree_new_call  (tree_exp*,
                           tree_exp_list*);



/* a op b    ==     not(a notRel(op) b)  */
tree_rel_op tree_not_rel (tree_rel_op);

/* a op b    ==    b commute(op) a       */
tree_rel_op tree_commute (tree_rel_op);

tree_stm_list * tree_new_stm_list (tree_stm *head,
                                   tree_stm_list *tail);

tree_exp_list * tree_new_exp_list (tree_exp *head,
                                   tree_exp_list *tail);


#endif /* _TREE_H_ */
