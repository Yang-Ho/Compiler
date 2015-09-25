#ifndef PARSER_H
#define PARSER_H 

#include <vector>
#include "token.h"

using namespace::std;

extern vector< Token > tokens;
extern int tIndex;
extern int num_variables;
extern int num_functions;
extern int num_statements;

bool Program() ;

bool Func_List() ;

bool Func() ;

bool Func_Decl() ;

static const string TYPE_NAMES[4] = {"int","void","binary","decimal"};
bool Type_Name();

bool Parameter_List() ;

bool Non_Empty_List() ;

bool Non_Empty_List_Prime() ;

bool Data_Decls() ;

bool Id_List() ;

bool Id_List_Prime() ;

bool Id() ;

bool Block_Statements() ;

bool Statements() ;

bool Statement() ;

bool Assignment() ;

bool Func_Call() ;

bool Expr_List() ;

bool Non_Empty_Expr_List() ;

bool Non_Empty_Expr_List_Prime() ;

bool If_Statement() ;

bool Condition_Expression() ;
       
bool Condition_Op() ;

bool Condition() ;

bool Comparison_Op() ;

bool While_Statement() ;

bool Return_Statement() ;

bool Break_Statement() ;

bool Continue_Statement() ;

bool Expression() ;

bool Expression_Prime() ;

bool Addop() ;

bool Term() ;

bool Term_Prime() ;

bool Mulop() ;

bool Factor() ;

#endif /* PARSER_H */
