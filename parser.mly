/* Ocamlyacc parser for MicroC */

%{
open Ast
%}

%token SEMI LPAREN RPAREN LBRACE RBRACE 
%token PLUS MINUS ASSIGN MOD
%token COLON LBRACKET RBRACKET QUOTE
%token EQ NEQ LT AND OR NOT
%token IF ELSE WHILE INT BOOL VOID HEAP CHARSEQ
%token DEREFERENCE POINTER
/* return, COMMA token */
%token RETURN COMMA
%token <int> LITERAL
%token <bool> BLIT
%token <string> ID
%token <float> FLOAT
%token EOF

%start program
%type <Ast.program> program
%left DEREFERENCE POINTER VOID
%left SEMI 
%left LPAREN RPAREN LBRACE RBRACE

%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT
%left PLUS MINUS



%%

/* add function declarations*/
program:
  decls EOF { $1}

decls:
   /* nothing */ { ([], [])               }
 | vdecl SEMI decls { (($1 :: fst $3), snd $3) }
 | fdecl decls { (fst $2, ($1 :: snd $2)) }

vdecl_list:
  /*nothing*/ { [] }
  | vdecl SEMI vdecl_list  {  $1 :: $3 }

/* int x */
vdecl:
  typ ID { ($1, $2) }

typ:
    INT   { Int   }
  | BOOL  { Bool  }
  | CHARSEQ { Charseq }

/* fdecl */
fdecl:
  vdecl LPAREN formals_opt RPAREN LBRACE vdecl_list stmt_list RBRACE
  {
    {
      rtyp=fst $1;
      fname=snd $1;
      formals=$3;
      locals=$6;
      body=$7
    }
  }

/* formals_opt */
formals_opt:
  /*nothing*/ { [] }
  | formals_list { $1 }

formals_list:
  vdecl { [$1] }
  | vdecl COMMA formals_list { $1::$3 }

stmt_list:
  /* nothing */ { [] }
  | stmt stmt_list  { $1::$2 }

stmt:
    expr SEMI                               { Expr $1      }
  | LBRACE stmt_list RBRACE                 { Block $2 }
  /* if (condition) { block1} else {block2} */
  /* if (condition) stmt else stmt */
  | IF LPAREN expr RPAREN stmt ELSE stmt    { If($3, $5, $7) }
  | WHILE LPAREN expr RPAREN stmt           { While ($3, $5)  }
  /* return */
  | RETURN expr SEMI                        { Return $2      }

expr:
    LITERAL          { Literal($1)            }
  | BLIT             { BoolLit($1)            }
  | HEAP ID          { Malloc ($2)            }
  | ID               { Id($1)                 }
  | FLOAT            { Float($1)              }
  | expr PLUS   expr { Binop($1, Add,   $3)   }
  | expr MINUS  expr { Binop($1, Sub,   $3)   }
  | expr EQ     expr { Binop($1, Equal, $3)   }
  | expr NEQ    expr { Binop($1, Neq, $3)     }
  | expr LT     expr { Binop($1, Less,  $3)   }
  | expr AND    expr { Binop($1, And,   $3)   }
  | expr OR     expr { Binop($1, Or,    $3)   }
  | DEREFERENCE expr { Deref ($2)             } 
  | POINTER expr     { AddrOf ($2)            }
  | VOID expr        { Void ($2)              } 
  | ID RBRACKET expr COLON expr LBRACKET { ArrayRange ($1 , $3, $5) }
  | ID RBRACKET expr COLON LBRACKET { ArrayToEnd ($1, $3) }
  | ID RBRACKET COLON expr LBRACKET { ArrayFromStart ($1, $4) }
  | ID RBRACKET COLON LBRACKET { ArrayFull ($1) }
  | ID ASSIGN expr   { Assign($1, $3)         }
  | LPAREN expr RPAREN { $2                   }
  /* call */
  | ID LPAREN args_opt RPAREN { Call ($1, $3)  }
/* args_opt*/
args_opt:
  /*nothing*/ { [] }
  | args { $1 }

args:
  expr  { [$1] }
  | expr COMMA args { $1::$3 }
