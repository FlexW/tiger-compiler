/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "../../src/tiger_grm.y" /* yacc.c:339  */

#define YYDEBUG 1

#include <stdio.h>

#include "include/util.h"
#include "include/errormsg.h"
#include "include/symbol.h"
#include "include/absyn.h"
//#include "include/list.h"

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

#line 118 "tiger_grm.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_TIGER_GRM_H_INCLUDED
# define YY_YY_TIGER_GRM_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    STRING = 259,
    INT = 260,
    COMMA = 261,
    COLON = 262,
    SEMICOLON = 263,
    LPAREN = 264,
    RPAREN = 265,
    LBRACK = 266,
    RBRACK = 267,
    LBRACE = 268,
    RBRACE = 269,
    DOT = 270,
    PLUS = 271,
    MINUS = 272,
    TIMES = 273,
    DIVIDE = 274,
    EQ = 275,
    NEQ = 276,
    LT = 277,
    LE = 278,
    GT = 279,
    GE = 280,
    AND = 281,
    OR = 282,
    ASSIGN = 283,
    ARRAY = 284,
    IF = 285,
    THEN = 286,
    ELSE = 287,
    WHILE = 288,
    FOR = 289,
    TO = 290,
    DO = 291,
    LET = 292,
    IN = 293,
    END = 294,
    OF = 295,
    BREAK = 296,
    NIL = 297,
    FUNCTION = 298,
    VAR = 299,
    TYPE = 300,
    UMINUS = 301
  };
#endif
/* Tokens.  */
#define ID 258
#define STRING 259
#define INT 260
#define COMMA 261
#define COLON 262
#define SEMICOLON 263
#define LPAREN 264
#define RPAREN 265
#define LBRACK 266
#define RBRACK 267
#define LBRACE 268
#define RBRACE 269
#define DOT 270
#define PLUS 271
#define MINUS 272
#define TIMES 273
#define DIVIDE 274
#define EQ 275
#define NEQ 276
#define LT 277
#define LE 278
#define GT 279
#define GE 280
#define AND 281
#define OR 282
#define ASSIGN 283
#define ARRAY 284
#define IF 285
#define THEN 286
#define ELSE 287
#define WHILE 288
#define FOR 289
#define TO 290
#define DO 291
#define LET 292
#define IN 293
#define END 294
#define OF 295
#define BREAK 296
#define NIL 297
#define FUNCTION 298
#define VAR 299
#define TYPE 300
#define UMINUS 301

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 55 "../../src/tiger_grm.y" /* yacc.c:355  */

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
	

#line 274 "tiger_grm.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TIGER_GRM_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 291 "tiger_grm.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  49
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   740

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  217

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   141,   141,   146,   149,   154,   159,   167,   170,   175,
     180,   188,   191,   194,   197,   200,   203,   206,   209,   212,
     215,   218,   221,   224,   227,   230,   233,   236,   242,   245,
     251,   261,   264,   270,   273,   276,   279,   282,   290,   298,
     301,   304,   307,   310,   313,   319,   322,   325,   328,   339,
     344,   348,   352,   355,   361,   366,   371,   376,   384,   389,
     394,   397,   400,   403,   409,   412,   418,   421,   427,   430,
     433,   436,   442,   445,   448,   451,   454,   457,   463,   468,
     473,   481,   484,   487,   493,   498,   505,   510,   518,   523,
     526,   529,   535,   540,   543,   549,   552,   557,   562,   570,
     575,   578,   584,   587,   590,   593,   599
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "STRING", "INT", "COMMA", "COLON",
  "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE",
  "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT",
  "GE", "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "WHILE",
  "FOR", "TO", "DO", "LET", "IN", "END", "OF", "BREAK", "NIL", "FUNCTION",
  "VAR", "TYPE", "UMINUS", "$accept", "program", "expsemicolonlist",
  "funcallargslist", "exp", "sequence", "negation", "funcall", "infix",
  "lvalue", "arrcreate", "typeidsubscript", "subscript", "reccreate",
  "efieldlist", "efield", "assign", "ifthenelse", "ifthen", "while", "for",
  "let", "declist", "dec", "fundeclist", "typedeclist", "typedec", "type",
  "arrtype", "rectype", "fielddeclist", "fielddec", "vardec", "fundec",
  "typeid", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

#define YYPACT_NINF -154

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-154)))

#define YYTABLE_NINF -96

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-96)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     471,  -154,  -154,  -154,    85,   471,   327,   346,    44,    28,
    -154,  -154,    16,   703,  -154,  -154,  -154,  -154,     4,  -154,
      47,  -154,  -154,  -154,  -154,  -154,  -154,  -154,    88,   141,
      38,   609,  -154,    34,   675,    55,   546,    68,     1,    44,
      44,    44,    62,    25,  -154,  -154,    76,  -154,    93,  -154,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
     471,   471,   471,    44,   471,  -154,   471,   167,     8,  -154,
     220,  -154,  -154,   220,   369,   388,   471,   471,   411,    39,
     262,  -154,   130,    -4,   120,   285,  -154,  -154,  -154,    24,
      24,  -154,  -154,   534,   534,   534,   534,   534,   534,   715,
     510,   691,  -154,   703,   703,     7,   140,    87,   110,   138,
     148,   135,   149,  -154,  -154,   124,   641,   128,   658,   703,
     703,   126,   621,   123,  -154,   498,   122,    44,   471,     5,
     125,  -154,   522,  -154,   304,  -154,  -154,   304,    35,  -154,
    -154,    35,   471,   471,   471,   471,   471,   430,   453,  -154,
    -154,    31,   153,   159,   152,   139,   703,    78,   129,  -154,
    -154,  -154,  -154,  -154,  -154,   160,  -154,  -154,   168,  -154,
    -154,   703,   703,   703,   703,   703,   137,   567,   142,   588,
      11,     3,    13,    11,    44,   471,   127,   161,    44,   471,
     471,   471,   471,   173,  -154,    44,   471,    44,   471,  -154,
    -154,   703,  -154,  -154,  -154,   703,   703,   703,   703,   162,
     703,   163,   703,   471,   471,   703,   703
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   106,    14,    13,     0,     0,     0,     0,     0,     0,
      26,    12,     0,     2,    15,    16,    17,    18,    11,    19,
      48,    20,    21,    22,    23,    24,    25,    27,    45,     0,
       0,     4,    30,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    81,    86,    82,    84,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    46,     0,     0,     0,    50,
       0,    29,    28,     0,     0,     0,     0,     0,     0,     0,
       0,    80,     0,     0,     0,     0,    79,    87,    85,    33,
      34,    35,    36,    39,    40,    44,    43,    42,    41,    37,
      38,     0,    47,    59,    49,     0,     0,     8,     0,     0,
      55,     0,     0,     6,     5,     0,    65,     0,    64,    67,
      66,     0,     0,     0,    77,     0,     0,     0,     0,     0,
       0,    76,     0,    51,     0,    32,    31,     0,     0,    53,
      52,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      73,     0,     0,    96,     0,     0,   101,     0,     0,    88,
      90,    91,    89,    74,    72,     0,    10,     9,     0,    57,
      56,    58,    63,    62,    61,    60,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,     0,     0,    97,
      99,   100,    94,    93,    92,    71,    69,    70,    68,     0,
     105,     0,   103,     0,     0,   104,   102
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -154,  -154,   -42,   -17,     0,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,   158,  -154,    -3,  -154,  -154,  -154,  -154,  -154,
    -154,  -154,    -8,  -154,   132,   147,  -154,  -154,  -154,  -154,
    -153,  -154,  -154,  -154,    90
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    12,    30,   106,    31,    14,    15,    16,    17,    18,
      19,    20,    65,    21,   109,   110,    22,    23,    24,    25,
      26,    27,    81,    43,    44,    45,    46,   159,   160,   161,
     152,   153,    47,    48,    28
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      13,    42,    79,   127,   187,    32,    34,    36,     1,   108,
     195,     1,   193,   134,     1,    62,    49,   135,   157,    63,
     197,   -95,   -54,   196,   128,   -95,    79,   194,   113,    38,
     199,   114,    64,   198,   158,    86,   168,   180,     1,    80,
      79,   181,    52,    53,    39,    40,    41,     1,    72,   -54,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   -78,   103,    74,   104,   107,    39,    40,
      41,    39,    40,    41,   116,   118,   119,   120,   122,   186,
     125,     1,    39,    40,    41,   132,    29,    66,     1,     2,
       3,    76,   -95,   137,     4,    -3,    78,    67,    37,    62,
      85,    68,     5,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,     6,   138,   166,     7,     8,
     167,    41,     9,   151,   139,     1,    10,    11,   156,    82,
      83,    84,   -95,   180,   107,   169,    39,   107,   170,   126,
     129,   202,   171,   172,   173,   174,   175,   177,   179,    70,
     136,    71,   140,   102,   141,   142,   143,    70,   111,   184,
     145,   147,   149,   182,   163,   183,   134,   185,   105,   188,
       1,     2,     3,   189,   138,   203,     4,    -7,   191,   180,
      88,     0,   213,   214,     5,   201,    69,     0,     0,   205,
     206,   207,   208,    87,     0,     0,   210,     6,   212,     0,
       7,     8,     0,     0,     9,     0,     0,     0,    10,    11,
       0,     0,     0,   215,   216,     0,   154,   155,     0,   162,
       0,   112,     0,     1,     2,     3,     0,     0,   111,     4,
      -3,   111,     0,     0,     0,     0,     0,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   154,     0,     0,
       6,     0,     0,     7,     8,     0,     0,     9,     0,     0,
       0,    10,    11,   123,     0,     1,     2,     3,     0,     0,
     154,     4,     0,   154,   200,     0,     0,     0,   204,     5,
       0,     0,     0,     0,     0,   209,   130,   211,     1,     2,
       3,     0,     6,     0,     4,     7,     8,     0,     0,     9,
       0,   124,     5,    10,    11,   165,     0,     1,     2,     3,
       0,     0,     0,     4,    -7,     6,     0,     0,     7,     8,
       0,     5,     9,     0,   131,     0,    10,    11,    33,     0,
       1,     2,     3,     0,     6,     0,     4,     7,     8,     0,
       0,     9,     0,     0,     5,    10,    11,    35,     0,     1,
       2,     3,     0,     0,     0,     4,     0,     6,     0,     0,
       7,     8,     0,     5,     9,     0,     0,     0,    10,    11,
     115,     0,     1,     2,     3,     0,     6,     0,     4,     7,
       8,     0,     0,     9,     0,     0,     5,    10,    11,   117,
       0,     1,     2,     3,     0,     0,     0,     4,     0,     6,
       0,     0,     7,     8,     0,     5,     9,     0,     0,     0,
      10,    11,   121,     0,     1,     2,     3,     0,     6,     0,
       4,     7,     8,     0,     0,     9,     0,     0,     5,    10,
      11,   176,     0,     1,     2,     3,     0,     0,     0,     4,
       0,     6,     0,     0,     7,     8,     0,     5,     9,     0,
       0,     0,    10,    11,   178,     0,     1,     2,     3,     0,
       6,     0,     4,     7,     8,     0,     0,     9,     0,     0,
       5,    10,    11,     0,     1,     2,     3,     0,     0,     0,
       4,     0,     0,     6,     0,     0,     7,     8,     5,     0,
       9,     0,     0,     0,    10,    11,     0,     0,     0,     0,
       0,     6,     0,     0,     7,     8,     0,     0,     9,     0,
       0,     0,    10,    11,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   150,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      50,    51,    52,    53,   -96,   -96,   -96,   -96,   -96,   -96,
       0,   164,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,    77,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,     0,     0,     0,     0,     0,
       0,     0,     0,   190,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,     0,    73,     0,     0,
       0,     0,     0,     0,   192,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,   148,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,     0,
       0,     0,     0,   144,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,     0,     0,     0,     0,
     146,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,   133,     0,     0,    75,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59
};

static const yytype_int16 yycheck[] =
{
       0,     9,     1,     7,   157,     5,     6,     7,     3,     1,
       7,     3,     1,     6,     3,    11,     0,    10,    13,    15,
       7,    10,    14,    20,    28,    14,     1,   180,    70,     1,
     183,    73,    28,    20,    29,    43,     1,     6,     3,    38,
       1,    10,    18,    19,    43,    44,    45,     3,    10,    14,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    38,    64,    31,    66,    67,    43,    44,
      45,    43,    44,    45,    74,    75,    76,    77,    78,     1,
      80,     3,    43,    44,    45,    85,     1,    40,     3,     4,
       5,    36,    14,     6,     9,    10,    28,     9,     8,    11,
      38,    13,    17,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    30,     6,   134,    33,    34,
     137,    45,    37,     1,    14,     3,    41,    42,   128,    39,
      40,    41,    10,     6,   134,   138,    43,   137,   141,     9,
      20,    14,   142,   143,   144,   145,   146,   147,   148,     8,
      10,    10,    14,    63,     6,    20,    32,     8,    68,     7,
      32,    35,    39,    10,    39,     6,     6,    28,     1,    40,
       3,     4,     5,    36,     6,    14,     9,    10,    36,     6,
      48,    -1,    20,    20,    17,   185,    28,    -1,    -1,   189,
     190,   191,   192,    46,    -1,    -1,   196,    30,   198,    -1,
      33,    34,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,
      -1,    -1,    -1,   213,   214,    -1,   126,   127,    -1,   129,
      -1,     1,    -1,     3,     4,     5,    -1,    -1,   138,     9,
      10,   141,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,    -1,    -1,
      30,    -1,    -1,    33,    34,    -1,    -1,    37,    -1,    -1,
      -1,    41,    42,     1,    -1,     3,     4,     5,    -1,    -1,
     180,     9,    -1,   183,   184,    -1,    -1,    -1,   188,    17,
      -1,    -1,    -1,    -1,    -1,   195,     1,   197,     3,     4,
       5,    -1,    30,    -1,     9,    33,    34,    -1,    -1,    37,
      -1,    39,    17,    41,    42,     1,    -1,     3,     4,     5,
      -1,    -1,    -1,     9,    10,    30,    -1,    -1,    33,    34,
      -1,    17,    37,    -1,    39,    -1,    41,    42,     1,    -1,
       3,     4,     5,    -1,    30,    -1,     9,    33,    34,    -1,
      -1,    37,    -1,    -1,    17,    41,    42,     1,    -1,     3,
       4,     5,    -1,    -1,    -1,     9,    -1,    30,    -1,    -1,
      33,    34,    -1,    17,    37,    -1,    -1,    -1,    41,    42,
       1,    -1,     3,     4,     5,    -1,    30,    -1,     9,    33,
      34,    -1,    -1,    37,    -1,    -1,    17,    41,    42,     1,
      -1,     3,     4,     5,    -1,    -1,    -1,     9,    -1,    30,
      -1,    -1,    33,    34,    -1,    17,    37,    -1,    -1,    -1,
      41,    42,     1,    -1,     3,     4,     5,    -1,    30,    -1,
       9,    33,    34,    -1,    -1,    37,    -1,    -1,    17,    41,
      42,     1,    -1,     3,     4,     5,    -1,    -1,    -1,     9,
      -1,    30,    -1,    -1,    33,    34,    -1,    17,    37,    -1,
      -1,    -1,    41,    42,     1,    -1,     3,     4,     5,    -1,
      30,    -1,     9,    33,    34,    -1,    -1,    37,    -1,    -1,
      17,    41,    42,    -1,     3,     4,     5,    -1,    -1,    -1,
       9,    -1,    -1,    30,    -1,    -1,    33,    34,    17,    -1,
      37,    -1,    -1,    -1,    41,    42,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,    33,    34,    -1,    -1,    37,    -1,
      -1,    -1,    41,    42,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    39,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      -1,    39,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
      -1,    -1,    -1,    32,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,
      32,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    12,    -1,    -1,    31,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     9,    17,    30,    33,    34,    37,
      41,    42,    48,    51,    52,    53,    54,    55,    56,    57,
      58,    60,    63,    64,    65,    66,    67,    68,    81,     1,
      49,    51,    51,     1,    51,     1,    51,    81,     1,    43,
      44,    45,    69,    70,    71,    72,    73,    79,    80,     0,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    11,    15,    28,    59,    40,     9,    13,    59,
       8,    10,    10,     8,    31,    31,    36,    36,    28,     1,
      38,    69,    81,    81,    81,    38,    69,    72,    71,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    81,    51,    51,     1,    50,    51,     1,    61,
      62,    81,     1,    49,    49,     1,    51,     1,    51,    51,
      51,     1,    51,     1,    39,    51,     9,     7,    28,    20,
       1,    39,    51,    12,     6,    10,    10,     6,     6,    14,
      14,     6,    20,    32,    32,    32,    32,    35,    35,    39,
      39,     1,    77,    78,    81,    81,    51,    13,    29,    74,
      75,    76,    81,    39,    39,     1,    50,    50,     1,    61,
      61,    51,    51,    51,    51,    51,     1,    51,     1,    51,
       6,    10,    10,     6,     7,    28,     1,    77,    40,    36,
      36,    36,    36,     1,    77,     7,    20,     7,    20,    77,
      81,    51,    14,    14,    81,    51,    51,    51,    51,    81,
      51,    81,    51,    20,    20,    51,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    49,    49,    49,    50,    50,    50,
      50,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    52,    52,
      53,    54,    54,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    56,    56,    56,    56,    57,
      58,    59,    60,    60,    61,    61,    61,    61,    62,    63,
      64,    64,    64,    64,    65,    65,    66,    66,    67,    67,
      67,    67,    68,    68,    68,    68,    68,    68,    69,    69,
      69,    70,    70,    70,    71,    71,    72,    72,    73,    74,
      74,    74,    75,    76,    76,    77,    77,    77,    77,    78,
      79,    79,    80,    80,    80,    80,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     3,     3,     0,     1,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     4,     4,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     2,     3,     1,     3,
       2,     3,     4,     4,     0,     1,     3,     3,     3,     3,
       6,     6,     6,     6,     4,     4,     4,     4,     8,     8,
       8,     8,     5,     5,     5,     5,     4,     4,     1,     2,
       2,     1,     1,     1,     1,     2,     1,     2,     4,     1,
       1,     1,     3,     3,     3,     0,     1,     3,     3,     3,
       6,     4,     9,     7,     9,     7,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 142 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {absyn_root = (yyvsp[0].exp);}
#line 1622 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 3:
#line 147 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.explist) = NULL;}
#line 1628 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 4:
#line 150 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.explist) = absyn_new_exp_list ((yyvsp[0].exp), NULL);
    }
#line 1636 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 5:
#line 155 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.explist) = absyn_new_exp_list ((yyvsp[-2].exp), (yyvsp[0].explist));
    }
#line 1644 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 6:
#line 160 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.explist) = absyn_new_exp_list (NULL, (yyvsp[0].explist));
    }
#line 1652 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 7:
#line 168 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.explist) = NULL;}
#line 1658 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 8:
#line 171 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.explist) = absyn_new_exp_list ((yyvsp[0].exp), NULL);
    }
#line 1666 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 9:
#line 176 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.explist) = absyn_new_exp_list ((yyvsp[-2].exp), (yyvsp[0].explist));
    }
#line 1674 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 10:
#line 181 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.explist) = absyn_new_exp_list (NULL, (yyvsp[0].explist));
    }
#line 1682 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 11:
#line 189 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_var_exp (errm_tok_pos, (yyvsp[0].var));}
#line 1688 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 12:
#line 192 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_nil_exp (errm_tok_pos);}
#line 1694 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 13:
#line 195 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_int_exp (errm_tok_pos, (yyvsp[0].ival));}
#line 1700 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 14:
#line 198 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_str_exp (errm_tok_pos, (yyvsp[0].sval));}
#line 1706 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 15:
#line 201 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1712 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 16:
#line 204 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1718 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 17:
#line 207 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1724 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 18:
#line 210 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1730 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 19:
#line 213 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1736 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 20:
#line 216 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1742 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 21:
#line 219 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1748 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 22:
#line 222 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1754 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 23:
#line 225 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1760 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 24:
#line 228 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1766 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 25:
#line 231 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1772 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 26:
#line 234 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_break_exp (errm_tok_pos);}
#line 1778 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 27:
#line 237 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[0].exp);}
#line 1784 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 28:
#line 243 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_seq_exp (errm_tok_pos, (yyvsp[-1].explist));}
#line 1790 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 29:
#line 246 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_seq_exp (errm_tok_pos, NULL);}
#line 1796 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 30:
#line 252 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.exp) = absyn_new_op_exp (errm_tok_pos,
                             ABSYN_MINUS_OP,
                             absyn_new_int_exp (errm_tok_pos, 0),
                             (yyvsp[0].exp));
    }
#line 1807 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 31:
#line 262 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_call_exp (errm_tok_pos, (yyvsp[-3].symbol), (yyvsp[-1].explist));}
#line 1813 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 32:
#line 265 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_call_exp (errm_tok_pos, (yyvsp[-3].symbol), NULL);}
#line 1819 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 33:
#line 271 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_PLUS_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1825 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 34:
#line 274 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_MINUS_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1831 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 35:
#line 277 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_TIMES_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1837 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 36:
#line 280 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_DIVIDE_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1843 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 37:
#line 283 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.exp) = absyn_new_if_exp (errm_tok_pos,
                             (yyvsp[-2].exp),
                             (yyvsp[0].exp),
                             absyn_new_int_exp (errm_tok_pos, 0));
    }
#line 1854 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 38:
#line 291 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.exp) = absyn_new_if_exp (errm_tok_pos,
                             (yyvsp[-2].exp),
                             absyn_new_int_exp (errm_tok_pos, 1),
                             (yyvsp[0].exp));
    }
#line 1865 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 39:
#line 299 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_EQ_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1871 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 40:
#line 302 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_NEQ_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1877 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 41:
#line 305 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_GE_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1883 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 42:
#line 308 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_GT_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1889 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 43:
#line 311 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_LE_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1895 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 44:
#line 314 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_op_exp (errm_tok_pos, ABSYN_LT_OP, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 1901 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 45:
#line 320 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.var) = absyn_new_simple_var (errm_tok_pos, (yyvsp[0].symbol));}
#line 1907 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 46:
#line 323 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.var) = absyn_new_subscript_var (errm_tok_pos, (yyvsp[-1].var), (yyvsp[0].exp));}
#line 1913 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 47:
#line 326 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.var) = absyn_new_field_var (errm_tok_pos, (yyvsp[-2].var), (yyvsp[0].symbol));}
#line 1919 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 48:
#line 329 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.var) = absyn_new_subscript_var (errm_tok_pos,
                                     absyn_new_simple_var (errm_tok_pos,
                                                           (yyvsp[0].typeid_sub)->sym),
                                     (yyvsp[0].typeid_sub)->exp);
    }
#line 1930 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 49:
#line 340 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_array_exp (errm_tok_pos, (yyvsp[-2].typeid_sub)->sym, (yyvsp[-2].typeid_sub)->exp, (yyvsp[0].exp));}
#line 1936 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 50:
#line 345 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.typeid_sub) = new_typeid_subscript ((yyvsp[-1].symbol), (yyvsp[0].exp));}
#line 1942 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 51:
#line 349 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = (yyvsp[-1].exp);}
#line 1948 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 52:
#line 353 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_record_exp (errm_tok_pos, (yyvsp[-3].symbol), (yyvsp[-1].efieldlist));}
#line 1954 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 53:
#line 356 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_record_exp (errm_tok_pos, (yyvsp[-3].symbol), NULL);}
#line 1960 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 54:
#line 362 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.efieldlist) = NULL;
    }
#line 1968 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 55:
#line 367 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.efieldlist) = absyn_new_efield_list ((yyvsp[0].efield), NULL);
    }
#line 1976 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 56:
#line 372 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.efieldlist) = absyn_new_efield_list ((yyvsp[-2].efield), (yyvsp[0].efieldlist));
    }
#line 1984 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 57:
#line 377 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.efieldlist) = absyn_new_efield_list (NULL, (yyvsp[0].efieldlist));
    }
#line 1992 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 58:
#line 385 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.efield) = absyn_new_efield ((yyvsp[-2].symbol), (yyvsp[0].exp));}
#line 1998 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 59:
#line 390 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_assign_exp (errm_tok_pos, (yyvsp[-2].var), (yyvsp[0].exp));}
#line 2004 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 60:
#line 395 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_if_exp (errm_tok_pos, (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2010 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 61:
#line 398 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_if_exp (errm_tok_pos, (yyvsp[-4].exp), NULL, (yyvsp[0].exp));}
#line 2016 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 62:
#line 401 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_if_exp (errm_tok_pos, NULL, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2022 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 63:
#line 404 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_if_exp (errm_tok_pos, NULL, NULL, (yyvsp[0].exp));}
#line 2028 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 64:
#line 410 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_if_exp (errm_tok_pos, (yyvsp[-2].exp), (yyvsp[0].exp), NULL);}
#line 2034 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 65:
#line 413 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_if_exp (errm_tok_pos, NULL, (yyvsp[0].exp), NULL);}
#line 2040 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 66:
#line 419 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_while_exp (errm_tok_pos, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2046 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 67:
#line 422 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_while_exp (errm_tok_pos, NULL, (yyvsp[0].exp));}
#line 2052 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 68:
#line 428 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_for_exp (errm_tok_pos, (yyvsp[-6].symbol), (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2058 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 69:
#line 431 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_for_exp (errm_tok_pos, (yyvsp[-6].symbol), NULL, (yyvsp[-2].exp), (yyvsp[0].exp));}
#line 2064 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 70:
#line 434 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_for_exp (errm_tok_pos, (yyvsp[-6].symbol), (yyvsp[-4].exp), NULL, (yyvsp[0].exp));}
#line 2070 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 71:
#line 437 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_for_exp (errm_tok_pos, (yyvsp[-6].symbol), NULL, NULL, (yyvsp[0].exp));}
#line 2076 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 72:
#line 443 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_let_exp (errm_tok_pos, (yyvsp[-3].declist), (yyvsp[-1].exp));}
#line 2082 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 73:
#line 446 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_let_exp (errm_tok_pos, NULL, (yyvsp[-1].exp));}
#line 2088 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 74:
#line 449 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_let_exp (errm_tok_pos, (yyvsp[-3].declist), NULL);}
#line 2094 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 75:
#line 452 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_let_exp (errm_tok_pos, NULL, NULL);}
#line 2100 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 76:
#line 455 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_let_exp (errm_tok_pos, (yyvsp[-2].declist), NULL);}
#line 2106 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 77:
#line 458 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.exp) = absyn_new_let_exp (errm_tok_pos, NULL, NULL);}
#line 2112 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 78:
#line 464 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.declist) = absyn_new_dec_list ((yyvsp[0].dec), NULL);
    }
#line 2120 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 79:
#line 469 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.declist) = absyn_new_dec_list ((yyvsp[-1].dec), (yyvsp[0].declist));
    }
#line 2128 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 80:
#line 474 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
       (yyval.declist) = absyn_new_dec_list (NULL, (yyvsp[0].declist));
    }
#line 2136 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 81:
#line 482 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.dec) = absyn_new_type_dec (errm_tok_pos, (yyvsp[0].nametylist));}
#line 2142 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 82:
#line 485 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.dec) = (yyvsp[0].dec);}
#line 2148 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 83:
#line 488 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.dec) = absyn_new_function_dec (errm_tok_pos, (yyvsp[0].fundeclist));}
#line 2154 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 84:
#line 494 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
       (yyval.fundeclist) = absyn_new_fundec_list ((yyvsp[0].fundec), NULL);
    }
#line 2162 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 85:
#line 499 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
       (yyval.fundeclist) = absyn_new_fundec_list ((yyvsp[-1].fundec), (yyvsp[0].fundeclist));
    }
#line 2170 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 86:
#line 506 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.nametylist) = absyn_new_name_ty_list ((yyvsp[0].namety), NULL);
    }
#line 2178 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 87:
#line 511 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.nametylist) = absyn_new_name_ty_list ((yyvsp[-1].namety), (yyvsp[0].nametylist));
    }
#line 2186 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 88:
#line 519 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.namety) = absyn_new_namety ((yyvsp[-2].symbol), (yyvsp[0].ty));}
#line 2192 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 89:
#line 524 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.ty) = absyn_new_name_ty (errm_tok_pos, (yyvsp[0].symbol));}
#line 2198 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 90:
#line 527 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.ty) = (yyvsp[0].ty);}
#line 2204 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 91:
#line 530 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.ty) = (yyvsp[0].ty);}
#line 2210 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 92:
#line 536 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.ty) = absyn_new_array_ty (errm_tok_pos, (yyvsp[0].symbol));}
#line 2216 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 93:
#line 541 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.ty) = absyn_new_record_ty (errm_tok_pos, (yyvsp[-1].fieldlist));}
#line 2222 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 94:
#line 544 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.ty) = absyn_new_record_ty (errm_tok_pos, NULL);}
#line 2228 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 95:
#line 550 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.fieldlist) = NULL;}
#line 2234 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 96:
#line 553 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.fieldlist) = absyn_new_field_list ((yyvsp[0].field), NULL);
    }
#line 2242 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 97:
#line 558 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.fieldlist) = absyn_new_field_list ((yyvsp[-2].field), (yyvsp[0].fieldlist));
    }
#line 2250 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 98:
#line 563 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {
      (yyval.fieldlist) = absyn_new_field_list (NULL, (yyvsp[0].fieldlist));
    }
#line 2258 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 99:
#line 571 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.field) = absyn_new_field (errm_tok_pos, (yyvsp[-2].symbol), (yyvsp[0].symbol));}
#line 2264 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 100:
#line 576 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.dec) = absyn_new_var_dec (errm_tok_pos, (yyvsp[-4].symbol), (yyvsp[-2].symbol), (yyvsp[0].exp));}
#line 2270 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 101:
#line 579 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.dec) = absyn_new_var_dec (errm_tok_pos, (yyvsp[-2].symbol), NULL, (yyvsp[0].exp));}
#line 2276 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 102:
#line 585 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.fundec) = absyn_new_fundec (errm_tok_pos, (yyvsp[-7].symbol), (yyvsp[-5].fieldlist), (yyvsp[-2].symbol), (yyvsp[0].exp));}
#line 2282 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 103:
#line 588 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.fundec) = absyn_new_fundec (errm_tok_pos, (yyvsp[-5].symbol), (yyvsp[-3].fieldlist), NULL, (yyvsp[0].exp));}
#line 2288 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 104:
#line 591 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.fundec) = absyn_new_fundec (errm_tok_pos, (yyvsp[-7].symbol), NULL, (yyvsp[-2].symbol), (yyvsp[0].exp));}
#line 2294 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 105:
#line 594 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.fundec) = absyn_new_fundec (errm_tok_pos, (yyvsp[-5].symbol), NULL, NULL, (yyvsp[0].exp));}
#line 2300 "tiger_grm.c" /* yacc.c:1646  */
    break;

  case 106:
#line 600 "../../src/tiger_grm.y" /* yacc.c:1646  */
    {(yyval.symbol) = sym_new_symbol((yyvsp[0].sval));}
#line 2306 "tiger_grm.c" /* yacc.c:1646  */
    break;


#line 2310 "tiger_grm.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
