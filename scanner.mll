(* Ocamllex scanner for KleenC *)

{ open Parser }

let digit = ['0'-'9']
let digits = digit+
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
| ':'	   { COLON }	
| ','      { COMMA }
| "'\""	   { QUOTE }
| '&'      { POINTER }
(* OPERATORS *)
| '+'      { PLUS }
| '-'      { MINUS }
| '*'	   { MULT }
| '/'      { DIV }
| '%'	   { MOD }
| '='      { ASSIGN }
| "=="     { EQ }
| "!="     { NEQ }
| '<'      { LT }
| "&&"     { AND }
| "||"     { OR }
| "++"	   { INCREMENT }
| "--"     { DECREMENT }
| '!'	   { NOT }
(* CONTROL *)
| "if"     { IF }
| "else"   { ELSE }
| "while"  { WHILE }
| "for"	   { FOR }
| "return" { RETURN }
(* TYPE *)
| "int"    { INT }
| "bool"   { BOOL }
| "charseq" { CHARSEQ } 
| "heap"   { HEAP }
| "void"   { VOID }
| "float"  { FLOAT }
(* LITERALS *)
| "true"   { BLIT(true)  }
| "false"  { BLIT(false) }
| digit+ as lem  { LITERAL(int_of_string lem) }
| letter (digit | letter | '_')* as lem { ID(lem) }
| '"' ([^ '"']* as lxm) '"'  { STRLIT(lxm) }
| digits '.'  digit* ( ['e' 'E'] ['+' '-']? digits )? as lxm { FLIT(lxm) }
| eof { EOF }
| _ as char { raise (Failure("illegal character " ^ Char.escaped char)) }

and comment = parse
  ":)" { token lexbuf }
| _    { comment lexbuf }
