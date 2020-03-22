(* Ocamllex scanner for MicroC, to be adapted into scanner for *C *)

{ open Parser }

let digit = ['0'-'9']
let letter = ['a'-'z' 'A'-'Z']

rule token = parse
  [' ' '\t' '\r' '\n'] { token lexbuf } (* Whitespace *)
| "(:"     { comment lexbuf }
| '('      { LPAREN }
| ')'      { RPAREN }
| '{'      { LBRACE }
| '}'      { RBRACE }
| ';'      { SEMI }
| ':'      { COLON }
| "\""     { QUOTE }
| '['      { LBRACKET }
| ']'      { RBRACKET }
| '*'      { DEREFERENCE }
| '&'      { POINTER }
| ','      { COMMA }
| "!"      { NOT }
| '+'      { PLUS }
| '-'      { MINUS }
| '='      { ASSIGN }
| '%'      { MOD }
| "=="     { EQ }
| "!="     { NEQ }
| '<'      { LT }
| "&&"     { AND }
| "||"     { OR }
| "if"     { IF }
| "else"   { ELSE }
| "while"  { WHILE }
| "return" { RETURN }
| "int"    { INT }
| "bool"   { BOOL }
| "float"  { FLOAT }
| "void"   { VOID }
| "heap"   { HEAP }
| "charseq"{ CHARSEQ }
| "true"   { BLIT(true)  }
| "false"  { BLIT(false) }
| digit+ as lem  { LITERAL(int_of_string lem) }
| digit+ (('.')(digit*))? | digit* (('.')(digit+)) {FLOAT(int_of_string lem) } (* floats don't need digits after decimal so this has to be kleene star not the + operator, since + REQUIRES digits.. *)
| letter (digit | letter | '_')* as lem { ID(lem) }
| eof { EOF }
| _ as char { raise (Failure("illegal character " ^ Char.escaped char)) }

(* Our version of comments: *)
and comment = parse
  ":)" { token lexbuf }
| _    { comment lexbuf }


