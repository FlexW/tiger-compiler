%{
#define YYDEBUG 1

#include <stdio.h>

#include "include/util.h"
#include "include/errormsg.h"
#include "include/symbol.h"
#include "include/absyn.h"
#include "include/list.h"

int yylex (void); /* function prototype */

absyn_exp *absyn_root;

void
yyerror (char *s)
{
  /*
  char* b_msg;

  b_msg = beautify_err_msg(s);
  if (b_msg != 0)
     s = b_msg;
  */
  errm_printf (errm_tok_pos, "%s", s);
}

/*
   This struct is necessary for getting in typeidsubscript
   rule two types through reduce step
*/
struct
typeid_subscript
{
    sym_symbol *sym;
    absyn_exp  *exp;
};

struct typeid_subscript*
new_typeid_subscript (sym_symbol *sym_ptr,
                      absyn_exp  *exp_ptr)
{
    struct typeid_subscript *t  = new (sizeof (*t));

    t->sym = sym_ptr;
    t->exp = exp_ptr;

    return t;
}
%}

%define parse.error verbose

%union {
	int                        pos;
	int                        ival;
	char                      *sval;
    absyn_var                 *var;
    absyn_exp                 *exp;
    absyn_dec                 *dec;
    absyn_ty                  *ty;
    absyn_dec_list            *declist;
    absyn_exp_list            *explist;
    absyn_field               *field;
    absyn_field_list          *fieldlist;
    absyn_fundec              *fundec;
    absyn_fundec_list         *fundeclist;
    absyn_name_ty             *namety;
    absyn_name_ty_list        *nametylist;
    absyn_efield              *efield;
    absyn_efield_list         *efieldlist;
    ABSYN_OP                   oper;
    sym_symbol                *symbol;
    struct typeid_subscript   *typeid_sub;
	}

%token <sval> ID STRING
%token <ival> INT

%token
COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
LBRACE RBRACE DOT
PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
AND OR ASSIGN
ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF
BREAK NIL
FUNCTION VAR TYPE

%start program

%nonassoc DO OF
%nonassoc THEN /* ELSE must come after THEN! */
%nonassoc ELSE
%left SEMICOLON
%left ASSIGN
%left OR
%left AND
%nonassoc EQ NEQ GT LT GE LE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS


%type <exp> exp program sequence negation funcall infix arrcreate
            reccreate assign ifthenelse ifthen while for let subscript

%type <dec> dec vardec

%type <declist> declist

%type <efield> efield

%type <efieldlist> efieldlist

%type <explist> funcallargslist expsemicolonlist

%type <field> fielddec

%type <fieldlist> fielddeclist

%type <fundec> fundec

%type <fundeclist> fundeclist

%type <namety> typedec

%type <nametylist> typedeclist

%type <symbol> typeid

%type <ty> type arrtype rectype

%type <var> lvalue

%type <typeid_sub> typeidsubscript

%%

program:
    exp
    {absyn_root = $1;};


expsemicolonlist:
    %empty
    {$$ = NULL;}

|   exp
    {
      $$ = list_new_list ($1, NULL);
    }

|   exp SEMICOLON expsemicolonlist
    {
      $$ = list_new_list ($1, $3);
    }

|   error SEMICOLON expsemicolonlist
    {
      $$ = list_new_list (NULL, $3);
    }
;


funcallargslist:
    %empty
    {$$ = NULL;}

|   exp
    {
      $$ = list_new_list ($1, NULL);
    }

|   exp COMMA funcallargslist
    {
      $$ = list_new_list ($1, $3);
    }

|   error COMMA funcallargslist
    {
      $$ = list_new_list (NULL, $3);
    }
;


exp:
    lvalue
    {$$ = absyn_new_var_exp (errm_tok_pos, $1);}

|   NIL
    {$$ = absyn_new_nil_exp (errm_tok_pos);}

|   INT
    {$$ = absyn_new_int_exp (errm_tok_pos, $1);}

|   STRING
    {$$ = absyn_new_str_exp (errm_tok_pos, $1);}

|   sequence
    {$$ = $1;}

|   negation
    {$$ = $1;}

|   funcall
    {$$ = $1;}

|   infix
    {$$ = $1;}

|   arrcreate
    {$$ = $1;}

|   reccreate
    {$$ = $1;}

|   assign
    {$$ = $1;}

|   ifthenelse
    {$$ = $1;}

|   ifthen
    {$$ = $1;}

|   while
    {$$ = $1;}

|   for
    {$$ = $1;}

|   BREAK
    {$$ = absyn_new_break_exp (errm_tok_pos);}

|   let
    {$$ = $1;}
;


sequence:
    LPAREN expsemicolonlist RPAREN
    {$$ = absyn_new_seq_exp (errm_tok_pos, $2);}

|   LPAREN error RPAREN
    {$$ = absyn_new_seq_exp (errm_tok_pos, NULL);}
;


negation:
    MINUS exp %prec UMINUS
    {
      $$ = absyn_new_op_exp (errm_tok_pos,
                             ABSYN_MINUS_OP,
                             absyn_new_int_exp (errm_tok_pos, 0),
                             $2);
    };


funcall:
    typeid LPAREN funcallargslist RPAREN
    {$$ = absyn_new_call_exp (errm_tok_pos, $1, $3);}

|   typeid LPAREN error RPAREN
    {$$ = absyn_new_call_exp (errm_tok_pos, $1, NULL);}
;


infix:
    exp PLUS exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_PLUS_OP, $1, $3);}

|   exp MINUS exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_MINUS_OP, $1, $3);}

|   exp TIMES exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_TIMES_OP, $1, $3);}

|   exp DIVIDE exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_DIVIDE_OP, $1, $3);}

|   exp AND exp
    {
      $$ = absyn_new_if_exp (errm_tok_pos,
                             $1,
                             $3,
                             absyn_new_int_exp (errm_tok_pos, 0));
    }

|   exp OR exp
    {
      $$ = absyn_new_if_exp (errm_tok_pos,
                             $1,
                             absyn_new_int_exp (errm_tok_pos, 1),
                             $3);
    }

|   exp EQ exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_EQ_OP, $1, $3);}

|   exp NEQ exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_NEQ_OP, $1, $3);}

|   exp GE exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_GE_OP, $1, $3);}

|   exp GT exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_GT_OP, $1, $3);}

|   exp LE exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_LE_OP, $1, $3);}

|   exp LT exp
    {$$ = absyn_new_op_exp (errm_tok_pos, ABSYN_LT_OP, $1, $3);}
;


lvalue:
    typeid
    {$$ = absyn_new_simple_var (errm_tok_pos, $1);}

|   lvalue subscript
    {$$ = absyn_new_subscript_var (errm_tok_pos, $1, $2);}

|   lvalue DOT typeid
    {$$ = absyn_new_field_var (errm_tok_pos, $1, $3);}

|   typeidsubscript
    {
      $$ = absyn_new_subscript_var (errm_tok_pos,
                                     absyn_new_simple_var (errm_tok_pos,
                                                           $1->sym),
                                     $1->exp);
    }
    ;


arrcreate:
    typeidsubscript OF exp
    {$$ = absyn_new_array_exp (errm_tok_pos, $1->sym, $1->exp, $3);};


typeidsubscript:
    typeid subscript
    {$$ = new_typeid_subscript ($1, $2);};

subscript:
    LBRACK exp RBRACK
    {$$ = $2;};

reccreate:
    typeid LBRACE efieldlist RBRACE
    {$$ = absyn_new_record_exp (errm_tok_pos, $1, $3);}

|   typeid LBRACE error RBRACE
    {$$ = absyn_new_record_exp (errm_tok_pos, $1, NULL);}
;


efieldlist:
    %empty
    {
      $$ = NULL;
    }

|   efield
    {
      $$ = list_new_list ($1, NULL);
    }

|   efield COMMA efieldlist
    {
      $$ = list_new_list ($1, $3);
    }

|   error COMMA efieldlist
    {
      $$ = list_new_list (NULL, $3);
    }
;


efield:
    typeid EQ exp
    {$$ = absyn_new_efield ($1, $3);};


assign:
    lvalue ASSIGN exp
    {$$ = absyn_new_assign_exp (errm_tok_pos, $1, $3);};


ifthenelse:
    IF exp THEN exp ELSE exp
    {$$ = absyn_new_if_exp (errm_tok_pos, $2, $4, $6);}

|   IF exp THEN error ELSE exp
    {$$ = absyn_new_if_exp (errm_tok_pos, $2, NULL, $6);}

|   IF error THEN exp ELSE exp
    {$$ = absyn_new_if_exp (errm_tok_pos, NULL, $4, $6);}

|   IF error THEN error ELSE exp
    {$$ = absyn_new_if_exp (errm_tok_pos, NULL, NULL, $6);}
;


ifthen:
    IF exp THEN exp
    {$$ = absyn_new_if_exp (errm_tok_pos, $2, $4, NULL);}

|   IF error THEN exp
    {$$ = absyn_new_if_exp (errm_tok_pos, NULL, $4, NULL);}
;


while:
    WHILE exp DO exp
    {$$ = absyn_new_while_exp (errm_tok_pos, $2, $4);}

|   WHILE error DO exp
    {$$ = absyn_new_while_exp (errm_tok_pos, NULL, $4);}
;


for:
    FOR typeid ASSIGN exp TO exp DO exp
    {$$ = absyn_new_for_exp (errm_tok_pos, $2, $4, $6, $8);}

|   FOR typeid ASSIGN error TO exp DO exp
    {$$ = absyn_new_for_exp (errm_tok_pos, $2, NULL, $6, $8);}

|   FOR typeid ASSIGN exp TO error DO exp
    {$$ = absyn_new_for_exp (errm_tok_pos, $2, $4, NULL, $8);}

|   FOR typeid ASSIGN error TO error DO exp
    {$$ = absyn_new_for_exp (errm_tok_pos, $2, NULL, NULL, $8);}
;


let:
    LET declist IN exp END
    {$$ = absyn_new_let_exp (errm_tok_pos, $2, $4);}

|   LET error IN exp END
    {$$ = absyn_new_let_exp (errm_tok_pos, NULL, $4);}

|   LET declist IN error END
    {$$ = absyn_new_let_exp (errm_tok_pos, $2, NULL);}

|   LET error IN error END
    {$$ = absyn_new_let_exp (errm_tok_pos, NULL, NULL);}

|   LET declist IN END
    {$$ = absyn_new_let_exp (errm_tok_pos, $2, NULL);}

|   LET error IN END
    {$$ = absyn_new_let_exp (errm_tok_pos, NULL, NULL);}
;


declist:
    dec
    {
      $$ = list_new_list ($1, NULL);
    }

|   dec declist
    {
      $$ = list_new_list ($1, $2);
    }

|   error declist
    {
       $$ = list_new_list (NULL, $2);
    }
;


dec:
    typedeclist
    {$$ = absyn_new_type_dec (errm_tok_pos, $1);}

|   vardec
    {$$ = $1;}

|   fundeclist
    {$$ = absyn_new_function_dec (errm_tok_pos, $1);}
;


fundeclist:
    fundec
    {
       $$ = list_new_list ($1, NULL);
    }

|   fundec fundeclist
    {
       $$ = list_new_list ($1, $2);
    }
;

typedeclist:
    typedec
    {
       $$ = list_new_list ($1, NULL);
    }

|   typedec typedeclist
    {
      $$ = list_new_list ($1, $2);
    }
;


typedec:
    TYPE typeid EQ type
    {$$ = absyn_new_namety ($2, $4);};


type:
    typeid
    {$$ = absyn_new_name_ty (errm_tok_pos, $1);}

|   arrtype
    {$$ = $1;}

|   rectype
    {$$ = $1;}
;


arrtype:
    ARRAY OF typeid
    {$$ = absyn_new_array_ty (errm_tok_pos, $3);};


rectype:
    LBRACE fielddeclist RBRACE
    {$$ = absyn_new_record_ty (errm_tok_pos, $2);}

|   LBRACE error RBRACE
    {$$ = absyn_new_record_ty (errm_tok_pos, NULL);}
;


fielddeclist:
    %empty
    {$$ = NULL;}

|   fielddec
    {
      $$ = list_new_list ($1, NULL);
    }

|   fielddec COMMA fielddeclist
    {
      $$ = list_new_list ($1, $3);
    }

|   error COMMA fielddeclist
    {
      $$ = list_new_list (NULL, $3);
    }
;


fielddec:
    typeid COLON typeid
    {$$ = absyn_new_field (errm_tok_pos, $1, $3);};


vardec:
    VAR typeid COLON typeid ASSIGN exp
    {$$ = absyn_new_var_dec (errm_tok_pos, $2, $4, $6);}

|   VAR typeid ASSIGN exp
    {$$ = absyn_new_var_dec (errm_tok_pos, $2, NULL, $4);}
;


fundec:
    FUNCTION typeid LPAREN fielddeclist RPAREN COLON typeid EQ exp
    {$$ = absyn_new_fundec (errm_tok_pos, $2, $4, $7, $9);}

|   FUNCTION typeid LPAREN fielddeclist RPAREN EQ exp
    {$$ = absyn_new_fundec (errm_tok_pos, $2, $4, NULL, $7);}

|   FUNCTION typeid LPAREN error RPAREN COLON typeid EQ exp
    {$$ = absyn_new_fundec (errm_tok_pos, $2, NULL, $7, $9);}

|   FUNCTION typeid LPAREN error RPAREN EQ exp
    {$$ = absyn_new_fundec (errm_tok_pos, $2, NULL, NULL, $7);}
;


typeid:
    ID
    {$$ = sym_new_symbol($1);};
