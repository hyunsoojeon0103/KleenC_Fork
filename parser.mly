/* Ocamlyacc parser for KleenC */

%{
open Ast
%}

/* SYNTAX */
%token SEMI COLON QUOTE COMMA LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
/* OPERATOR */
%token PLUS MINUS MULT DIV MOD INCREMENT DECREMENT ASSIGN EQ NEQ LT NOT AND OR DEREFERENCE POINTER
/* TYPE */
%token INT BOOL HEAP VOID CHARSEQ FLOAT
/* CONTROL */
%token RETURN IF ELSE WHILE FOR
/* LITERAL */
%token <int> LITERAL
%token <bool> BLIT
%token <string> ID STRLIT FLIT
%token EOF

%start program
%type <Ast.program> program

%right ASSIGN
%left OR
%left AND
%left EQ NEQ
%left LT
%left PLUS MINUS
%left MULT DIV MOD
%left INCREMENT
%left DECREMENT

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
  | HEAP  { Heap }
  | CHARSEQ { Charseq }
  | VOID  { Void }
  | FLOAT { Float }

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
  | IF LPAREN expr RPAREN stmt ELSE stmt    { If($3, $5, $7) }
  | WHILE LPAREN expr RPAREN stmt           { While ($3, $5)  }
  | FOR LPAREN expr_opt SEMI expr SEMI expr_opt RPAREN stmt
					    { For($3, $5, $7, $9) }
  | RETURN expr SEMI                        { Return $2      }

expr_opt:
		     { Noexpr }
  | expr    	     { $1     }

expr:
    LITERAL          { Literal($1)            }
  | FLIT  	     { FloatLit($1)	      } 
  | BLIT             { BoolLit($1)            }
  | STRLIT	     { StrLit($1)	      }
  | ID               { Id($1)                 }
  | expr PLUS   expr { Binop($1, Add,   $3)   }
  | expr MINUS  expr { Binop($1, Sub,   $3)   }
  | expr MULT   expr { Binop($1, Mult,   $3)  }
  | expr DIV    expr { Binop($1, Div,   $3)   }
  | expr MOD    expr { Binop($1, Mod,   $3)   }
  | expr EQ     expr { Binop($1, Equal, $3)   }
  | expr NEQ    expr { Binop($1, Neq, $3)     }
  | expr LT     expr { Binop($1, Less,  $3)   }
  | expr AND    expr { Binop($1, And,   $3)   }
  | expr OR     expr { Binop($1, Or,    $3)   }
  | expr INCREMENT   { Unop(Inc, $1)	      }
  | expr DECREMENT   { Unop(Dec, $1)	      }
  | ID ASSIGN expr   { Assign($1, $3)         }
  | ID LPAREN args_opt RPAREN { Call ($1, $3) }
  | LPAREN expr RPAREN { $2                   }
  | LBRACKET expr RBRACKET { $2		      }
  


/* args_opt*/
args_opt:
  /*nothing*/ { [] }
  | args { $1 }

args:
  expr  { [$1] }
  | expr COMMA args { $1::$3 }
