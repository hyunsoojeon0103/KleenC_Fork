(* Ocamllex scanner for KleenC *)

{ open Parser }

let digit = ['0'-'9']
let letter = ['a'-'z' 'A'-'Z']

rule token = parse
  [' ' '\t' '\r' '\n'] { token lexbuf }
| "(:"     { comment lexbuf }
(* SYNTAX *)
| '('      { LPAREN }
| ')'      { RPAREN }
| '{'      { LBRACE }
| '}'      { RBRACE }
| '['	   { LBRACKET }
| ']'	   { RBRACKET }
| ';'      { SEMI }
| ','      { COMMA }
(* OPERATORS *)
| '+'      { PLUS }
| '-'      { MINUS }
| '='      { ASSIGN }
| "=="     { EQ }
| "!="     { NEQ }
| '<'      { LT }
| "&&"     { AND }
| "||"     { OR }
| "++"	   { INCREMENT }
| "--"     { DECREMENT }
(* CONTROL *)
| "if"     { IF }
| "else"   { ELSE }
| "while"  { WHILE }
| "return" { RETURN }
(* TYPE *)
| "int"    { INT }
| "bool"   { BOOL }
| "charseq" { CHARSEQ } 
| "heap"   { HEAP }
| "void"   { VOID }
(* LITERALS *)
| "true"   { BLIT(true)  }
| "false"  { BLIT(false) }
| digit+ as lem  { LITERAL(int_of_string lem) }
| letter (digit | letter | '_')* as lem { ID(lem) }
| '"' ([^ '"']* as lxm) '"'  { STRLIT(lxm) }
| eof { EOF }
| _ as char { raise (Failure("illegal character " ^ Char.escaped char)) }

and comment = parse
  ":)" { token lexbuf }
| _    { comment lexbuf }
