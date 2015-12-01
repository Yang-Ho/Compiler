# Compiler
For CSC 512

In this project, we were asked to write a compiler for a modfied C language into a assembly like language using C.

Input files and output files should be valid C programs.

___

Input Token Language:
```
<letter> ::= a | b | ... | y | z | A | B | ... | Z | underscore
*<identifier> ::= <letter> (<letter> | <digit>)*

<digit> ::= 0 | 1 | ... | 9
*<number> ::= <digit>+

*<reserved word> ::= int  | void | if | while | return | read | write | print | continue | break | binary | decimal

*<symbol> ::= left_parenthesis | right_parenthesis | left_brace | right_brace | left_bracket | right_bracket | comma | semicolon | plus_sign | minus_sign | star_sign | forward_slash | == | != | > | >= | < | <= | equal_sign | double_and_sign | double_or_sign
(The symbols are: ( ) { } [ ] , ; + - * / == != > >= < <= = && ||)

*<string> ::= any string between (and including) the closest pair of quotation marks.
*<meta statement> ::= any string begins with '#' or '//' and ends with the end of line ('\n'). 
```

Input Grammar:
```
<program> ::= <data decls> <func list>
<func list> ::= empty | <func> <func list>
<func> ::= <func decl> semicolon | <func decl> left_brace <data decls> <statements> right_brace
<func decl> ::= <type name> ID left_parenthesis <parameter list> right_parenthesis
<type name> ::= int | void | binary | decimal
<parameter list> ::= empty | void | <non-empty list>
<non-empty list> ::= <type name> ID | <non-empty list> comma <type name> ID
<data decls> ::= empty | <type name> <id list> semicolon <data decls>
<id list> ::= <id> | <id list> comma <id>
<id> ::= ID | ID left_bracket <expression> right_bracket

<block statements> ::= left_brace <statements> right_brace
<statements> ::= empty | <statement> <statements>
<statement> ::= <assignment> | <func call> | <if statement> | <while statement> | <return statement> | <break statement> | <continue statement> | read left_parenthesis  ID right_parenthesis semicolon | write left_parenthesis <expression> right_parenthesis semicolon | print left_parenthesis  STRING right_parenthesis semicolon
<assignment> ::= <id> equal_sign <expression> semicolon
<func call> ::= ID left_parenthesis <expr list> right_parenthesis semicolon
<expr list> ::= empty | <non-empty expr list>
<non-empty expr list> ::= <expression> | <non-empty expr list> comma <expression>

<if statement> ::= if left_parenthesis <condition expression> right_parenthesis <block statements>
<condition expression> ::=  <condition> | <condition> <condition op> <condition>
<condition op> ::= double_end_sign | double_or_sign
<condition> ::= <expression> <comparison op> <expression>
<comparison op> ::= == | != | > | >= | < | <=

<while statement> ::= while left_parenthesis <condition expression> right_parenthesis <block statements>
<return statement> ::= return <expression> semicolon | return semicolon
<break statement> -::= break semicolon
<continue statement> -::= continue semicolon

<expression> ::= <term> | <expression> <addop> <term>
<addop> ::= plus_sign | minus_sign
<term> ::= <factor> | <term> <mulop> <factor>
<mulop> ::= star_sign | forward_slash
<factor> ::= ID | ID left_bracket <expression> right_bracket | ID left_parenthesis <expr list> right_parenthesis | NUMBER | minus_sign NUMBER | left_parenthesis <expression> right_parenthesis
```
