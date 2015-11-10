#ifndef GRAMMAR_H
#define GRAMMAR_H 
#include "parser.h"
#include "ASTnode.h"
#include "symbol.h"

using namespace::std;

class Parser;

class Grammar {
    private:
        Parser *parser;                     // The parser object
    public:
        // Loads parser
        void LoadParser(Parser *newparser);

        // Grammar productions
        bool Program();

        bool Program_Prime();

        bool Data_Decls_Prime();

        bool Data_Decls_Prime_Tail();

        bool Func_List_Prime();

        bool Func_List();

        bool Func();

        bool Func_Tail();

        bool Func_Decl();

        bool Func_Decl_Prime();

        bool Type_Name();

        bool Type_Name_Prime();

        bool Parameter_List();

        bool Parameter_List_Tail();

        bool Non_Empty_List();

        bool Non_Empty_List_Prime();

        bool Data_Decls();

        bool Data_Func_Decl();

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

        bool Func_Call_Tail();

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
        
        bool Return_Statement_Tail();

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
};
#endif /* GRAMMAR_H */
