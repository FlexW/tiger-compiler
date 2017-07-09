/**
 * @file absyn.c
 * Abstract syntax functions.
 *
 * Most functions create an instance of an abstract syntax rule.
 */

#include "include/util.h"
#include "include/symbol.h"
#include "include/absyn.h"

absyn_var*
absyn_new_simple_var (int pos,
                      sym_symbol *sym_ptr)
{
  absyn_var *v = new (sizeof(*v));

  v->kind     = ABSYN_SIMPLE_VAR;
  v->pos      = pos;
  v->u.simple = sym_ptr;

  return v;
}

absyn_var*
absyn_new_field_var (int         pos,
                     absyn_var  *var_ptr,
                     sym_symbol *sym_ptr)
{
  absyn_var *v = new (sizeof(*v));

  v->kind        = ABSYN_FIELD_VAR;
  v->pos         = pos;
  v->u.field.var = var_ptr;
  v->u.field.sym = sym_ptr;

  return v;
}

absyn_var*
absyn_new_subscript_var (int        pos,
                         absyn_var *var_ptr,
                         absyn_exp *exp_ptr)
{
  absyn_var *v = new (sizeof(*v));

  v->kind            = ABSYN_SUBSCRIPT_VAR;
  v->pos             = pos;
  v->u.subscript.var = var_ptr;
  v->u.subscript.exp = exp_ptr;

  return v;
}

absyn_exp*
absyn_new_var_exp (int        pos,
                   absyn_var *var_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind  = ABSYN_VAR_EXP;
  e->pos   = pos;
  e->u.var = var_ptr;

  return e;
}

absyn_exp*
absyn_new_nil_exp (int pos)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind = ABSYN_NIL_EXP;
  e->pos  = pos;

  return e;
}


absyn_exp*
absyn_new_int_exp (int pos,
                   int i)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind   = ABSYN_INT_EXP;
  e->pos    = pos;
  e->u.intt = i;

  return e;
}


absyn_exp*
absyn_new_str_exp (int   pos,
                   char *s_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind  = ABSYN_STR_EXP;
  e->pos   = pos;
  e->u.str = s_ptr;

  return e;
}

absyn_exp*
absyn_new_call_exp (int             pos,
                    sym_symbol     *func_ptr,
                    absyn_exp_list *args_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind        = ABSYN_CALL_EXP;
  e->pos         = pos;
  e->u.call.func = func_ptr;
  e->u.call.args = args_ptr;

  return e;
}

absyn_exp*
absyn_new_op_exp (int        pos,
                  ABSYN_OP   op,
                  absyn_exp *left_ptr,
                  absyn_exp *right_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind       = ABSYN_OP_EXP;
  e->pos        = pos;
  e->u.op.op    = op;
  e->u.op.left  = left_ptr;
  e->u.op.right = right_ptr;

  return e;
}

absyn_exp*
absyn_new_record_exp (int                pos,
                      sym_symbol        *typ_ptr,
                      absyn_efield_list *fields_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind            = ABSYN_RECORD_EXP;
  e->pos             = pos;
  e->u.record.typ    = typ_ptr;
  e->u.record.fields = fields_ptr;

  return e;
}

absyn_exp*
absyn_new_seq_exp (int             pos,
                   absyn_exp_list *seq_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind  = ABSYN_SEQ_EXP;
  e->pos   = pos;
  e->u.seq = seq_ptr;

  return e;
}

absyn_exp*
absyn_new_assign_exp (int        pos,
                      absyn_var *var_ptr,
                      absyn_exp *exp_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind         = ABSYN_ASSIGN_EXP;
  e->pos          = pos;
  e->u.assign.var = var_ptr;
  e->u.assign.exp = exp_ptr;

  return e;
}

absyn_exp*
absyn_new_if_exp (int        pos,
                  absyn_exp *test_ptr,
                  absyn_exp *then_ptr,
                  absyn_exp *elsee_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind        = ABSYN_IF_EXP;
  e->pos         = pos;
  e->u.iff.test  = test_ptr;
  e->u.iff.then  = then_ptr;
  e->u.iff.elsee = elsee_ptr;

  return e;
}

absyn_exp*
absyn_new_while_exp (int        pos,
                     absyn_exp *test_ptr,
                     absyn_exp *body_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind          = ABSYN_WHILE_EXP;
  e->pos           = pos;
  e->u.whilee.test = test_ptr;
  e->u.whilee.body = body_ptr;

  return e;
}

absyn_exp*
absyn_new_for_exp (int         pos,
                   sym_symbol *var_ptr,
                   absyn_exp  *lo_ptr,
                   absyn_exp  *hi_ptr,
                   absyn_exp  *body_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind          = ABSYN_FOR_EXP;
  e->pos           = pos;
  e->u.forr.var    = var_ptr;
  e->u.forr.lo     = lo_ptr;
  e->u.forr.hi     = hi_ptr;
  e->u.forr.body   = body_ptr;
  e->u.forr.escape = true;

  return e;
}

absyn_exp*
absyn_new_break_exp (int pos)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind = ABSYN_BREAK_EXP;
  e->pos  = pos;

  return e;
}

absyn_exp*
absyn_new_let_exp (int             pos,
                   absyn_dec_list *decs_ptr,
                   absyn_exp      *body_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind       = ABSYN_LET_EXP;
  e->pos        = pos;
  e->u.let.decs = decs_ptr;
  e->u.let.body = body_ptr;

  return e;
}

absyn_exp*
absyn_new_array_exp (int         pos,
                     sym_symbol *typ_ptr,
                     absyn_exp  *size_ptr,
                     absyn_exp  *init_ptr)
{
  absyn_exp *e = new (sizeof(*e));

  e->kind         = ABSYN_ARRAY_EXP;
  e->pos          = pos;
  e->u.array.typ  = typ_ptr;
  e->u.array.size = size_ptr;
  e->u.array.init = init_ptr;

  return e;
}

absyn_dec*
absyn_new_function_dec (int                pos,
                        absyn_fundec_list *func_ptr)
{
  absyn_dec *d = new (sizeof(*d));

  d->kind       = ABSYN_FUNCTION_DEC;
  d->pos        = pos;
  d->u.function = func_ptr;

  return d;
}

absyn_dec*
absyn_new_var_dec (int         pos,
                   sym_symbol *var_ptr,
                   sym_symbol *typ_ptr,
                   absyn_exp  *init_ptr)
{
  absyn_dec *d = new (sizeof(*d));

  d->kind         = ABSYN_VAR_DEC;
  d->pos          = pos;
  d->u.var.var    = var_ptr;
  d->u.var.typ    = typ_ptr;
  d->u.var.init   = init_ptr;
  d->u.var.escape = true;

  return d;
}

absyn_dec*
absyn_new_type_dec (int                 pos,
                    absyn_name_ty_list *type_ptr)
{
  absyn_dec *d = new (sizeof(*d));

  d->kind   = ABSYN_TYPE_DEC;
  d->pos    = pos;
  d->u.type = type_ptr;

  return d;
}

absyn_ty*
absyn_new_name_ty (int         pos,
                   sym_symbol *name_ptr)
{
  absyn_ty *t = new (sizeof(*t));

  t->kind   = ABSYN_NAME_TY;
  t->pos    = pos;
  t->u.name = name_ptr;

  return t;
}

absyn_ty*
absyn_new_record_ty (int               pos,
                     absyn_field_list *record_ptr)
{
  absyn_ty *t = new (sizeof(*t));

  t->kind     = ABSYN_RECORD_TY;
  t->pos      = pos;
  t->u.record = record_ptr;

  return t;
}

absyn_ty*
absyn_new_array_ty (int        pos,
                    sym_symbol *array_ptr)
{
  absyn_ty *t = new (sizeof(*t));

  t->kind    = ABSYN_ARRAY_TY;
  t->pos     = pos;
  t->u.array = array_ptr;

  return t;
}

absyn_field*
absyn_new_field (int         pos,
                 sym_symbol *name_ptr,
                 sym_symbol *typ_ptr)
{
  absyn_field *f = new (sizeof(*f));

  f->pos    = pos;
  f->name   = name_ptr;
  f->typ    = typ_ptr;
  f->escape = true;

  return f;
}

absyn_fundec*
absyn_new_fundec (int               pos,
                  sym_symbol       *name_ptr,
                  absyn_field_list *params_ptr,
                  sym_symbol       *result_ptr,
                  absyn_exp        *body_ptr)
{
  absyn_fundec *f = new (sizeof (*f));

  f->pos    = pos;
  f->name   = name_ptr;
  f->params = params_ptr;
  f->result = result_ptr;
  f->body   = body_ptr;

  return f;
}

absyn_name_ty*
absyn_new_namety (sym_symbol *name_ptr,
                  absyn_ty   *ty_ptr)
{
  absyn_name_ty *t = new (sizeof (*t));

  t->name = name_ptr;
  t->ty   = ty_ptr;

  return t;
}

absyn_efield*
absyn_new_efield (sym_symbol *name_ptr,
                  absyn_exp  *exp_ptr)
{
  absyn_efield *f = new (sizeof (*f));

  f->name = name_ptr;
  f->exp  = exp_ptr;

  return f;
}

absyn_dec_list *
absyn_new_dec_list (absyn_dec      *head,
                    absyn_dec_list *tail)
{
  absyn_dec_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
};

absyn_exp_list *
absyn_new_exp_list (absyn_exp      *head,
                    absyn_exp_list *tail)
{
  absyn_exp_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
};

absyn_field_list *
absyn_new_field_list (absyn_field      *head,
                      absyn_field_list *tail)
{
  absyn_field_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
};

absyn_fundec_list *
absyn_new_fundec_list (absyn_fundec      *head,
                       absyn_fundec_list *tail)
{
  absyn_fundec_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
};

absyn_name_ty_list *
absyn_new_name_ty_list (absyn_name_ty      *head,
                        absyn_name_ty_list *tail)
{
  absyn_name_ty_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
};

absyn_efield_list *
absyn_new_efield_list (absyn_efield      *head,
                       absyn_efield_list *tail)
{
  absyn_efield_list *l = new (sizeof (*l));
  l->head = head;
  l->tail = tail;
  return l;
};
