%{
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "include/util.h"
#include "include/errormsg.h"

#include "include/tokens.h"

int char_pos = 1;

const int    INIT_BUFFER_LENGTH = 32;
char        *str_buf;
unsigned int str_buf_len = 0;
unsigned int str_buf_pos = 0;

int comment_deep = 0;


int
yywrap (void)
{
  char_pos = 1;
  return 1;
}

/* track char position for error msg */
void
adjust (void)
{
  errm_tok_pos = char_pos;
  char_pos    += yyleng;
}

/* string state functions */
void
init_str_buf (void)
{
  str_buf     = new (INIT_BUFFER_LENGTH);
  str_buf[0]  = '\0';
  str_buf_len = INIT_BUFFER_LENGTH;
  str_buf_pos = 0;
}

void
str_buf_add (char c)
{
  char* tmp;

  if ((str_buf_pos++) == str_buf_len)
    {
      tmp = new ((str_buf_len *= 2));
      memcpy (tmp, str_buf, str_buf_len);
      free (str_buf);
      str_buf = tmp;
    }
  str_buf[str_buf_pos - 1] = c;
  str_buf[str_buf_pos]     = '\0';
}

%}

%x STRING_STATE COMMENT

%%
 /* carriage return, tab space */
[ \r\t] {adjust(); continue;}

 /* newline */
<INITIAL,COMMENT>\n {adjust(); errm_newline(); continue;}

 /* symbols */
","  {adjust(); return COMMA;}
":"  {adjust(); return COLON;}
";"  {adjust(); return SEMICOLON;}
"("  {adjust(); return LPAREN;}
")"  {adjust(); return RPAREN;}
"["  {adjust(); return LBRACK;}
"]"  {adjust(); return RBRACK;}
"{"  {adjust(); return LBRACE;}
"}"  {adjust(); return RBRACE;}
"."  {adjust(); return DOT;}
"+"  {adjust(); return PLUS;}
"-"  {adjust(); return MINUS;}
"*"  {adjust(); return TIMES;}
"/"  {adjust(); return DIVIDE;}
"="  {adjust(); return EQ;}
"<>" {adjust(); return NEQ;}
"<"  {adjust(); return LT;}
"<=" {adjust(); return LE;}
">"  {adjust(); return GT;}
">=" {adjust(); return GE;}
"&"  {adjust(); return AND;}
"|"  {adjust(); return OR;}
":=" {adjust(); return ASSIGN;}

 /* keywords */
while    {adjust(); return WHILE;}
for      {adjust(); return FOR;}
to       {adjust(); return TO;}
break    {adjust(); return BREAK;}
let      {adjust(); return LET;}
in       {adjust(); return IN;}
end      {adjust(); return END;}
function {adjust(); return FUNCTION;}
var      {adjust(); return VAR;}
type     {adjust(); return TYPE;}
array    {adjust(); return ARRAY;}
if       {adjust(); return IF;}
then     {adjust(); return THEN;}
else     {adjust(); return ELSE;}
do       {adjust(); return DO;}
of       {adjust(); return OF;}
nil      {adjust(); return NIL;}


 /* unsigned integer */
[0-9]+ {
         adjust ();
         yylval.ival = atoi (yytext);
         return INT;
       }

 /* id */
[a-zA-Z]+[_0-9a-zA-Z]* {
                         adjust ();
                         yylval.sval = strdup (yytext);
                         return ID;
                       }

 /* start string */
\" {adjust (); init_str_buf (); BEGIN (STRING_STATE);}

 /* start comment */
"/*" {adjust (); comment_deep++; BEGIN (COMMENT);}

 /* end comment before start -> error */
"*/" {
       adjust ();
       errm_printf (errm_tok_pos, "No comment opened. Illegal comment close!\n");
       yyterminate ();
     }

 /* rest */
. {adjust (); errm_printf (errm_tok_pos, "illegal token"); yyterminate ();}


<COMMENT>{
  /* open */
  "/*" {adjust (); comment_deep++; continue;}

  /* close */
  "*/" {comment_deep--; if (comment_deep == 0) BEGIN (INITIAL);}

  <<EOF>> {
            errm_printf (errm_tok_pos, "Comment still open at end of file!");
            yyterminate ();
          }

  /* rest */
  . {adjust();}
 }

<STRING_STATE>{
  /* close */
  \" {
       adjust();
       yylval.sval = strdup (str_buf);
       BEGIN (INITIAL);
       return STRING;
     }

  /* new line -> error */
  \n {
       adjust();
       errm_printf(errm_tok_pos, "Unterminated string constant!");
       yyterminate();
     }

  /* escape sequences */
  /* single character in ascii code */
  \\[0-9]{3} {
               int code;

               adjust();
               sscanf(yytext + 1, "%d", &code);
               if (code > 0xff)
                {
                  errm_printf(errm_tok_pos,
                              "ASCII decimal value out of bounds!");
                  yyterminate();
                }
               str_buf_add(code);
             }

  /* new line */
  \\n {adjust (); str_buf_add ('\n');}

  /* tab */
  \\t {adjust (); str_buf_add ('\t');}

  /* ascii control character */
  "\^"[@A-Z\[\\\]\^_?] {adjust (); str_buf_add (yytext[1] - '@');}

  /* double quote */
  "\\\"" {adjust (); str_buf_add ('"');}

  /* backslash */
  "\\\\" {adjust (); str_buf_add ('\\');}

  /* \f...f\ sequence. f...f ignored. allows multiline strings with \ */
  \\[ \t\n\f]+\\ {
                    int i;

                    adjust();
                    for (i = 0; yytext[i]; i++)
                      {
                         if (yytext[i] == '\n') {
                         errm_newline();
                      }
                   }
                   continue;
                 }

  <<EOF>> {
            errm_printf (errm_tok_pos,
                         "String not closed at end of file!");
            yyterminate();
          }

  /* normal text */
  [^\\\n\"]* {
               char *yptr;

               adjust();
               yptr = yytext;

               while (*yptr)
                 str_buf_add(*yptr++);
             }
 }
