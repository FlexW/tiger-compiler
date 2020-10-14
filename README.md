# Tiger Compiler
This is my implementation for the Tiger compiler form Andrew W. Appels book, Modern Compiler Implementation in C. With this project I tried to learn how a compiler works.

The Tiger language is a small language with nested functions, record values with implicit pointers, arrays, integer and string variables, and a few simple structured control constructs.


## Language Description

### Lexical Issues

#### Identifiers:
An identifier is a sequence of letters, digits, and underscores, starting with a letter. Uppercase letters are distinguished from lowercase. In this description the symbol id stands for an identifier.

#### Comments:
A comment may appear between any two tokens. Comments start with /* and end with */ and may be nested.

### Declarations
A declaration-sequence is a sequence of type, value, and function declarations; no punctuation separates or terminates individual declarations.
```  
decs → {dec}

dec → tydec
    → vardec
    → fundec
```
In the syntactic notation used here, ε stands for the empty string and {x} stands for a possibly empty sequence of x’s.

### Data Types
The syntax of types and type declarations in Tiger is
```
tydec → type type-id = ty

ty → type-id
   → { tyfields } (these braces stand for themselves)
   → array of type-id

tyfields → ε
         → id : type-id {, id : type-id}
```
**Built-in types:**
Two named types int and string are predefined. Additional
named types may be defined or redefined (including the predefined ones) by
type declarations.


**Records:**
Record types are defined by a listing of their fields enclosed in braces,
with each field described by fieldname : type-id, where type-id is an identifier
defined by a type declaration.


**Arrays:**
An array of any named type may be made by array of type-id. The length of the array is not specified as part of its type;
each array of that type can have a different length, and the length will be decided upon array creation,
at run time.


**Record distinction:**
Each declaration of a record or array type creates a new type,
incompatible with all other record or array types (even if all the fields are similar).


**Mutually recursive types:**
A collection of types may be recursive or mutually recursive.
Mutually recursive types are declared by a consecutive sequence of type declarations without intervening value or
function declarations. Each recursion cycle must pass through a record or array type.

Thus, the type of lists of integers:
```
type intlist = {hd: int, tl: intlist}
type tree = {key: int, children: treelist}
type treelist = {hd: tree, tl: treelist}
```
But the following declaration sequence is illegal:
```
type b = c
type c = b
```
**Field name reusability:**
Different record types may use the same field names (such as the hd field of intlist and treelist in the example above).

### Variables
```
vardec → var id := exp
       → var id : type-id := exp
```
In the short form of variable declaration, the name of the variable is given,
followed by an expression representing the initial value of the variable.
In this case, the type of the variable is determined from the type of the expression.
In the long form, the type of the variable is also given. The expression must have the same type.
If the initializing expression is nil, then the long form must be used.
Each variable declaration creates a new variable, which lasts as long as the scope of the declaration.

### Functions
```
fundec → function id ( tyfields) = exp
       → function id ( tyfields) : type-id = exp
```
The first of these is a procedure declaration; the second is a function declaration.
Procedures do not return result values; functions do, and the type is specified after the colon.
The exp is the body of the procedure or function, and the tyfields specify the names and type of the parameters.
All parameters are passed by value.
Functions may be recursive. Mutually recursive functions and procedures are declared by a sequence of consecutive function
declarations (with no intervening type or variable declarations):
```
function treeLeaves(t : tree) : int =
  if t=nil then 1
  else treelistLeaves(t.children)

function treelistLeaves(L : treelist) : int =
  if L=nil then 0
  else treeLeaves(L.hd) + treelistLeaves(L.tl)
```

### Scope Rules
**Local variables:**
In the expression let · · · vardec · · · in exp end, the scope of the declared variable starts just after its vardec and
lasts until the end.


**Parameters:**
In function id ( · · · id 1 : id 2 · · · ) = exp the scope of the parameter id 1 lasts throughout the function body exp.

**Nested scopes:**
The scope of a variable or parameter includes the bodies of any function definitions in that scope.
That is, access to variables in outer scopes is permitted, as in Pascal and Algol.

**Types:**
In the expression let · · · tydecs · · · in exps end the scope of a type identifier starts at the beginning of the
consecutive sequence of type declarations defining it and lasts until the end.
This includes the headers and bodies of any functions within the scope.

**Functions:**
In the expression let · · · fundecs · · · in exps end the scope of a function identifier starts at the beginning of the
consecutive sequence of function declarations defining it and lasts until the end.
This includes the headers and bodies of any functions within the scope.

**Name spaces:**
There are two different name spaces: one for types, and one for functions and variables.
A type a can be “in scope” at the same time as a variable a or a function a,
but variables and functions of the same name cannot both be in scope simultaneously (one will hide the other).

**Local redeclarations:**
A variable or function declaration may be hidden by the redeclaration of the same name (as a variable or function) in a
smaller scope;
for example, this function prints “6 7 6 8 6” when applied to 5:
```
function f(v: int) =
  let var v := 6
    in print(v);
      let var v := 7 in print (v) end;
      print(v);
      let var v := 8 in print (v) end;
      print(v)
end
```
Functions hide variables of the same name, and vice versa. Similarly, a type declaration may be hidden by the redeclaration
of the same name (as a type) in a smaller scope.
However, no two functions in a sequence of mutually recursive functions may have the same name;
and no two types in a sequence of mutually recursive types may have the same name.

### Variables and Expressions
### L-Values
An l-value is a location whose value may be read or assigned.
Variables, procedure parameters, fields of records, and elements of arrays are all l-values.
```
lvalue → id
       → lvalue . id
       → lvalue [ exp ]
```
**Variable:**
The form id refers to a variable or parameter accessible by scope rules.


**Record field:**
The dot notation allows the selection of the correspondingly named field of a record value.


**Array subscript:**
The bracket notation allows the selection of the correspondingly numbered slot of an array.
Arrays are indexed by consecutive integers starting at zero (up to the size of the array minus one).

### Expressions
**l-value:**
An l-value, when used as an expression, evaluates to the contents of the
corresponding location.


**Valueless expressions:**
Certain expressions produce no value: procedure calls, assignment, if-then, while, break, and sometimes if-then-else.
Therefore the expression (a:=b)+c is syntactically correct but fails to type-check.

**Nil:**
The expression nil (a reserved word) denotes a value nil belonging to every record type.
If a record variable v contains the value nil, it is a checked runtime error to select a field from v.
Nil must be used in a context where its type can be determined, that is:
```
var a : my record := nil                      OK
a := nil                                      OK
if a <> nil then . . .                        OK
if nil <> a then . . .                        OK
if a = nil then . . .                         OK
function f(p: my record) = . . . f(nil)       OK
var a := nil                             ILLEGAL
if nil = nil then . . .                  ILLEGAL
```
**Sequencing:**
A sequence of two or more expressions, surrounded by parentheses and
separated by semicolons (exp;exp; . . . exp) evaluates all the expressions in order.
The result of a sequence is the result (if any) yielded by the last of the expressions.


**No value:**
An open parenthesis followed by a close parenthesis (two separate tokens) is an expression that yields no value.
Similarly, a let expression with nothing between the in and end yields no value.


**Integer literal:**
A sequence of decimal digits is an integer constant that denotes the corresponding integer value.

**String literal:**
A string constant is a sequence, between quotes ( " ), of zero or more printable characters, spaces, or escape sequences.
Each escape sequence is introduced by the escape character \ , and stands for a character sequence.
The allowed escape sequences are as follows (all other uses of \ being illegal):
```
\n        A character interpreted by the system as end-of-line.
\t        Tab.
\^c       The control character c, for any appropriate c.
\ddd      The single character with ASCII code ddd (3 decimal digits).
\"        The double-quote character ( " ).
\\        The backslash character ( \ ).
\f___f    This sequence is ignored, where f f stands for a sequence of one or more formatting characters (a subset of
          the non-printable characters including at least space, tab, newline, formfeed).
          This allows one to write long strings on more than one line, by writing \ at the end of one line
          and at the start of the next.
```
**Negation:**
An integer-valued expression may be prefixed by a minus sign.


**Function call:**
A function application id() or id(exp{, exp}) indicates the application of function id
to a list of actual parameter values obtained by evaluating the expressions left to right. The actual parameters are bound to the corresponding formal parameters of the function definition and the function body is bound using conventional static
scoping rules to obtain a result. If id actually stands for a procedure (a function returning no result),
then the function body must produce no value, and the function application also produces no value.


**Arithmetic:**
Expressions of the form exp op exp, where op is + , - , * , / , require integer arguments and produce an integer result.


**Comparison:**
Expressions of the form exp op exp, where op is = , <> , > , < , >= , <= , compare their operands for equality or inequality
and produce the integer 1 for true, 0 for false.
All these operators can be applied to integer operands. The equals and not-equals operators can also be applied to two
record or array operands of the same type, and compare for “reference” or “pointer” equality (they test whether two records
are the same instance, not whether they have the same contents).


**String comparison:**
The comparison operators may also be applied to strings. Two strings are equal if their contents are equal;
there is no way to distinguish strings whose component characters are the same. Inequality is according to lexicographic
order.


**Boolean operators:**
Expressions of the form exp op exp, where op is & or | , are short-circuit boolean conjunctions and disjunctions:
they do not evaluate the right-hand operand if the result is detemined by the left-hand one.
Any nonzero integer value is considered true, and an integer value of zero is false.
Precedence of operators: Unary minus (negation) has the highest precedence.
Then operators * , / have the next highest (tightest binding) precedence,
followed by + , - , then by = , <> , > , < , >= , <= , then by & , then by | .


**Associativity of operators:**
The operators * , / , + , - are all left-associative.
The comparison operators do not associate, so a=b=c is not a legal expression, although a=(b=c) is legal.


**Record creation:**
The expression type-id { id=exp{,id=exp} } or (for an empty record type) type-id { } creates a new record instance
of type type-id. The field names and types of the record expression must match those of the named type,
in the order given. The braces { } stand for themselves.


**Array creation:**
The expression type-id[exp 1 ] of exp 2 evaluates exp 1 and exp 2 (in that order) to find n,
the number of elements, and v the initial value. The type type-id must be declared as an array type.
The result of the expression is a new array of type type-id,
indexed from 0 to n − 1, in which each slot is initialized to the value v.


**Array and record assignment:**
When an array or record variable a is assigned a value b, then a references the same array or record as b.
Future updates of elements of a will affect b, and vice versa, until a is reassigned.
Parameter passing of arrays and records is similarly by reference, not by copying.


**Extent:**
Records and arrays have infinite extent: each record or array value lasts forever,
even after control exits from the scope in which it was created.


**Assignment:**
The assignment statement lvalue := exp evaluates the lvalue, then evaluates the exp,
then sets the contents of the lvalue to the result of the expression.
Syntactically, := binds weaker than the boolean operators & and | .
The assignment expression produces no value, so that (a:=b)+c is illegal.


**If-then-else:**
The if-expression if exp 1 then exp 2 else exp 3 evaluates the integer expression exp 1 .
If the result is nonzero it yields the result of evaluating exp 2 ; otherwise it yields the result of exp 3 .
The expressions exp 2 and exp 3 must have the same type, which is also the type of the entire if-expression (or both
expressions must produce no value).


**If-then:**
The if-expression if exp 1 then exp 2 evaluates the integer expression exp 1 .
If the result is nonzero, then exp 2 (which must produce no value) is evaluated.
The entire if-expression produces no value.


**While:**
The expression while exp 1 do exp 2 evaluates the integer expression exp 1 .
If the result is nonzero, then exp 2 (which must produce no value) is executed,
and then the entire while-expression is reevaluated.


**For:**
The expression for id := exp 1 to exp 2 do exp 3 iterates exp 3 over each integer value of id between exp 1 and exp 2 .
The variable id is a new variable implicitly declared by the for statement, whose scope covers only exp 3 , and
may not be assigned to. The body exp 3 must produce no value.
The upper and lower bounds are evaluated only once, prior to entering the body of the loop.
If the upper bound is less than the lower, the body is not executed.


**Break:**
The break expression terminates evaluation of the nearest enclosing while-expression or for-expression.
A break in procedure p cannot terminate a loop in procedure q, even if p is nested within q.
A break that is not within a while or for is illegal.


**Let:**
The expression let decs in expseq end evaluates the declarations decs, binding types, variables, and
procedures whose scope then extends over the expseq.
The expseq is a sequence of zero or more expressions, separated by semicolons.
The result (if any) of the last exp in the sequence is then the result of the entire let-expression.


**Parentheses:**
Parentheses around any expression enforce syntactic grouping, as in most programming languages.

## Implementation
The compiler consists of different modules. The different modules are:

- Lexer
- Parser
- Semantic Analysis
- Translation to Intermediate Code
- Instruction selection
- Control Flow Analysis
- Register Allocation
- Code Emission

The compiler will then output a assembly file, which must be passed to a linker to link it with the runtime library.

## Usage
First compile the compiler. You need a GNU/Linux system for that. I have just tested under Ubuntu 17.04. You also need Bison
and Flex.

```
cd ~
git clone https://github.com/FlexW/tiger-compiler
cd tiger-compiler
./bootstrap.sh
mkdir build
cd build
../configure
make
sudo make install
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib
```
Run a simple test `tc ../test/testcases/queens.tig`. If everything goes right, the compiler will produce a assembly file in the same directory as the source file is. In this case `queens.tig.S`.

Now pass this file to a linker and link it with `runtime.c` in `~/tiger-compiler/src/`.
For example (assuming you have gcc installed):
```
cd ~
gcc -Wl,--wrap,getchar -m32 tiger-compiler/test/testcases/queens.tig.S tiger-compiler/src/runtime.c -o queens
```
Now you will have a binary file named `queens`.
Run it with:
`./queens`
