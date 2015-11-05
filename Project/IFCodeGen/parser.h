#ifndef PARSER_H
#define PARSER_H 

#include "token.h"
#include "compiler.h"

#include <string>
#include <vector>

using namespace::std;

extern Token currToken;
extern Compiler compiler;
extern int num_variables;
extern int num_functions;
extern int num_statements;

bool Parse(string);

void Consume(Token);

bool Program();

bool Program_Prime();

bool Func_List();

bool Func();

bool Func_Tail();

bool Func_Decl();

static const string TYPE_NAMES[4] = {"int","void","binary","decimal"};
bool Type_Name();

bool Parameter_List();

bool Non_Empty_List();

bool Non_Empty_List_Prime();

bool Data_Decls();

bool Id_List();

bool Id_List_Prime();

bool Id();

bool Id_Tail();

bool Block_Statements();

bool Statements();

bool Statement();

bool Statement_Tail();

bool Assignment();

bool Assignment_Tail();

bool Func_Call();

bool Expr_List();

bool Non_Empty_Expr_List();

bool Non_Empty_Expr_List_Prime();

bool If_Statement();

bool Condition_Expression();

bool Condition_Expression_Tail();
       
bool Condition_Op();

bool Condition();

bool Comparison_Op();

bool While_Statement();

bool Return_Statement();

bool Break_Statement();

bool Continue_Statement();

bool Expression();

bool Expression_Prime();

bool Addop();

bool Term();

bool Term_Prime();

bool Mulop();

bool Factor();

bool Factor_Tail();

#endif /* PARSER_H */
