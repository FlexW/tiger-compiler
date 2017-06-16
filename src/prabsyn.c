/**
 * @file prabsyn.c
 * Print Abstract Syntax structures.
 */

#include <stdio.h>
#include <assert.h>

#include "include/util.h"
#include "include/symbol.h" /* symbol table data structures */
#include "include/absyn.h"  /* abstract syntax data structures */
#include "include/prabsyn.h" /* function prototype */

static char str_oper[][12] = {
   "PLUS", "MINUS", "TIMES", "DIVIDE",
   "EQUAL", "NOTEQUAL", "LESSTHAN", "LESSEQ", "GREAT", "GREATEQ"};


/* local function prototypes */
static void pr_var         (FILE      *out_ptr,
                           absyn_var *v_ptr,
                           int        d);

static void pr_dec         (FILE      *out_ptr,
                           absyn_dec *v_ptr,
                           int        d);

static void pr_ty          (FILE     *out_ptr,
                           absyn_ty *v_ptr,
                           int       d);

static void pr_field       (FILE        *out_ptr,
                           absyn_field *v_ptr,
                           int          d);

static void pr_field_list  (FILE             *out_ptr,
                           absyn_field_list *v_ptr,
                           int               d);

static void pr_exp_list    (FILE           *out_ptr,
                           absyn_exp_list *v_ptr,
                           int             d);

static void pr_fundec      (FILE         *out_ptr,
                           absyn_fundec *v_ptr,
                           int           d);

static void pr_fundec_list (FILE              *out_ptr,
                           absyn_fundec_list *v_ptr,
                           int                d);

static void pr_dec_list    (FILE           *out_ptr,
                           absyn_dec_list *v_ptr,
                           int             d);

static void pr_namety      (FILE          *out_ptr,
                           absyn_name_ty *v_ptr,
                           int            d);

static void pr_namety_list (FILE              *out_ptr,
                           absyn_name_ty_list *v_ptr,
                           int                 d);

static void pr_efield     (FILE         *out_ptr,
                           absyn_efield *v_ptr,
                           int           d);

static void pr_efield_list (FILE              *out_ptr,
                           absyn_efield_list *v_ptr,
                           int                d);


static void
indent (FILE *out_ptr,
        int d)
{
  int i;
  for (i = 0; i <= d; i++)
    fprintf(out_ptr, " ");
}

/* Print A_var types. Indent d spaces. */
static void
pr_var (FILE      *out_ptr,
        absyn_var *v_ptr,
        int        d)
{
  indent(out_ptr, d);
  switch (v_ptr->kind)
    {
    case ABSYN_SIMPLE_VAR:
      fprintf (out_ptr, "simpleVar(%s)", sym_name (v_ptr->u.simple));
      break;

    case ABSYN_FIELD_VAR:
      fprintf (out_ptr, "%s\n", "fieldVar(");

      pr_var (out_ptr, v_ptr->u.field.var, d+1);
      fprintf(out_ptr, "%s\n", ",");

      indent (out_ptr, d+1);
      fprintf(out_ptr, "%s)", sym_name (v_ptr->u.field.sym));
      break;

    case ABSYN_SUBSCRIPT_VAR:
      fprintf (out_ptr, "%s\n", "subscriptVar(");

      pr_var (out_ptr, v_ptr->u.subscript.var, d+1);
      fprintf(out_ptr, "%s\n", ",");

      prabsyn_exp (out_ptr, v_ptr->u.subscript.exp, d+1);
      fprintf(out_ptr, "%s", ")");
      break;

    default:
       assert(0);
    }
}

static void
pr_oper (FILE *out_ptr, ABSYN_OP d)
{
  fprintf(out_ptr, "%s", str_oper[d]);
}

/* Print A_var types. Indent d spaces. */
void
prabsyn_exp (FILE       *out_ptr,
             absyn_exp *v_ptr,
             int        d)
{
 indent(out_ptr, d);
 switch (v_ptr->kind) {
 case ABSYN_VAR_EXP:
   fprintf(out_ptr, "varExp(\n"); pr_var(out_ptr, v_ptr->u.var, d+1);
   fprintf(out_ptr, "%s", ")");
   break;
 case ABSYN_NIL_EXP:
   fprintf(out_ptr, "nilExp()");
   break;
 case ABSYN_INT_EXP:
   fprintf(out_ptr, "intExp(%d)", v_ptr->u.intt);
   break;
 case ABSYN_STR_EXP:
   fprintf(out_ptr, "stringExp(%s)", v_ptr->u.str);
   break;
 case ABSYN_CALL_EXP:
   fprintf(out_ptr, "callExp(%s,\n", sym_name(v_ptr->u.call.func));
   pr_exp_list(out_ptr, v_ptr->u.call.args, d+1); fprintf(out_ptr, ")");
   break;
 case ABSYN_OP_EXP:
   fprintf(out_ptr, "opExp(\n");
   indent(out_ptr, d+1); pr_oper(out_ptr, v_ptr->u.op.op); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.op.left, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.op.right, d+1); fprintf(out_ptr, ")");
   break;
 case ABSYN_RECORD_EXP:
   fprintf(out_ptr, "recordExp(%s,\n", sym_name(v_ptr->u.record.typ));
   pr_efield_list(out_ptr, v_ptr->u.record.fields, d+1); fprintf(out_ptr, ")");
   break;
 case ABSYN_SEQ_EXP:
   fprintf(out_ptr, "seqExp(\n");
   pr_exp_list(out_ptr, v_ptr->u.seq, d+1); fprintf(out_ptr, ")");
   break;
 case ABSYN_ASSIGN_EXP:
   fprintf(out_ptr, "assignExp(\n");
   pr_var(out_ptr, v_ptr->u.assign.var, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.assign.exp, d+1); fprintf(out_ptr, ")");
   break;
 case ABSYN_IF_EXP:
   fprintf(out_ptr, "iffExp(\n");
   prabsyn_exp(out_ptr, v_ptr->u.iff.test, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.iff.then, d+1);
   if (v_ptr->u.iff.elsee) { /* else is optional */
      fprintf(out_ptr, ",\n");
      prabsyn_exp(out_ptr, v_ptr->u.iff.elsee, d+1);
   }
   fprintf(out_ptr, ")");
   break;
 case ABSYN_WHILE_EXP:
   fprintf(out_ptr, "whileExp(\n");
   prabsyn_exp(out_ptr, v_ptr->u.whilee.test, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.whilee.body, d+1); fprintf(out_ptr, ")\n");
   break;
 case ABSYN_FOR_EXP:
   fprintf(out_ptr, "forExp(%s,\n", sym_name(v_ptr->u.forr.var));
   prabsyn_exp(out_ptr, v_ptr->u.forr.lo, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.forr.hi, d+1); fprintf(out_ptr, "%s\n", ",");
   prabsyn_exp(out_ptr, v_ptr->u.forr.body, d+1); fprintf(out_ptr, ",\n");
   indent(out_ptr, d+1); fprintf(out_ptr, "%s", v_ptr->u.forr.escape ? "TRUE)" : "FALSE)");
   break;
 case ABSYN_BREAK_EXP:
   fprintf(out_ptr, "breakExp()");
   break;
 case ABSYN_LET_EXP:
   fprintf(out_ptr, "letExp(\n");
   pr_dec_list(out_ptr, v_ptr->u.let.decs, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.let.body, d+1); fprintf(out_ptr, ")");
   break;
 case ABSYN_ARRAY_EXP:
   fprintf(out_ptr, "arrayExp(%s,\n", sym_name(v_ptr->u.array.typ));
   prabsyn_exp(out_ptr, v_ptr->u.array.size, d+1); fprintf(out_ptr, ",\n");
   prabsyn_exp(out_ptr, v_ptr->u.array.init, d+1); fprintf(out_ptr, ")");
   break;
 default:
   assert(0);
 }
}

static void pr_dec(FILE *out, absyn_dec *v, int d) {
 indent(out, d);
 switch (v->kind) {
 case ABSYN_FUNCTION_DEC:
   fprintf(out, "functionDec(\n");
   pr_fundec_list(out, v->u.function, d+1); fprintf(out, ")");
   break;
 case ABSYN_VAR_DEC:
   fprintf(out, "varDec(%s,\n", sym_name(v->u.var.var));
   if (v->u.var.typ) {
     indent(out, d+1); fprintf(out, "%s,\n", sym_name(v->u.var.typ));
   }
   prabsyn_exp(out, v->u.var.init, d+1); fprintf(out, ",\n");
   indent(out, d+1); fprintf(out, "%s", v->u.var.escape ? "TRUE)" : "FALSE)");
   break;
 case ABSYN_TYPE_DEC:
   fprintf(out, "typeDec(\n");
   pr_namety_list(out, v->u.type, d+1); fprintf(out, ")");
   break;
 default:
   assert(0);
 }
}

static void pr_ty(FILE *out, absyn_ty *v, int d) {
 indent(out, d);
 switch (v->kind) {
 case ABSYN_NAME_TY:
   fprintf(out, "nameTy(%s)", sym_name(v->u.name));
   break;
 case ABSYN_RECORD_TY:
   fprintf(out, "recordTy(\n");
   pr_field_list(out, v->u.record, d+1); fprintf(out, ")");
   break;
 case ABSYN_ARRAY_TY:
   fprintf(out, "arrayTy(%s)", sym_name(v->u.array));
   break;
 default:
   assert(0);
 }
}

static void pr_field(FILE *out, absyn_field *v, int d) {
 indent(out, d);
 fprintf(out, "field(%s,\n", sym_name(v->name));
 indent(out, d+1); fprintf(out, "%s,\n", sym_name(v->typ));
 indent(out, d+1); fprintf(out, "%s", v->escape ? "TRUE)" : "FALSE)");
}

static void pr_field_list(FILE *out, absyn_field_list *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "fieldList(\n");
   pr_field(out, v->head, d+1); fprintf(out, ",\n");
   pr_field_list(out, v->tail, d+1); fprintf(out, ")");
 }
 else fprintf(out, "fieldList()");
}

static void pr_exp_list(FILE *out, absyn_exp_list *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "expList(\n");
   prabsyn_exp(out, v->head, d+1); fprintf(out, ",\n");
   pr_exp_list(out, v->tail, d+1);
   fprintf(out, ")");
 }
 else fprintf(out, "expList()");

}

static void pr_fundec(FILE *out, absyn_fundec *v, int d) {
 indent(out, d);
 fprintf(out, "fundec(%s,\n", sym_name(v->name));
 pr_field_list(out, v->params, d+1); fprintf(out, ",\n");
 if (v->result) {
   indent(out, d+1); fprintf(out, "%s,\n", sym_name(v->result));
 }
 prabsyn_exp(out, v->body, d+1); fprintf(out, ")");
}

static void pr_fundec_list(FILE *out, absyn_fundec_list *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "fundec_list(\n");
   pr_fundec(out, v->head, d+1); fprintf(out, ",\n");
   pr_fundec_list(out, v->tail, d+1); fprintf(out, ")");
 }
 else fprintf(out, "fundecList()");
}

static void pr_dec_list(FILE *out, absyn_dec_list *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "decList(\n");
   pr_dec(out, v->head, d+1); fprintf(out, ",\n");
   pr_dec_list(out, v->tail, d+1);
   fprintf(out, ")");
 }
 else fprintf(out, "decList()");

}

static void pr_namety(FILE *out, absyn_name_ty *v, int d) {
 indent(out, d);
 fprintf(out, "namety(%s,\n", sym_name(v->name));
 pr_ty(out, v->ty, d+1); fprintf(out, ")");
}

static void pr_namety_list(FILE *out, absyn_name_ty_list *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "nametyList(\n");
   pr_namety(out, v->head, d+1); fprintf(out, ",\n");
   pr_namety_list(out, v->tail, d+1); fprintf(out, ")");
 }
 else fprintf(out, "nametyList()");
}

static void pr_efield (FILE *out, absyn_efield *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "efield(%s,\n", sym_name(v->name));
   prabsyn_exp(out, v->exp, d+1); fprintf(out, ")");
 }
 else fprintf(out, "efield()");
}

static void pr_efield_list(FILE *out, absyn_efield_list *v, int d) {
 indent(out, d);
 if (v) {
   fprintf(out, "efieldList(\n");
   pr_efield(out, v->head, d+1); fprintf(out, ",\n");
   pr_efield_list(out, v->tail, d+1); fprintf(out, ")");
 }
 else fprintf(out, "efieldList()");
}
