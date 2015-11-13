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
        ASTNode * Program();

        ASTNode * Program_Prime();

        ASTNode * Data_Decls_Prime();

        ASTNode * Data_Decls_Prime_Tail();

        ASTNode * Func_List_Prime();

        ASTNode * Func_List();

        ASTNode * Func();

        ASTNode * Func_Tail();

        ASTNode * Func_Decl();

        ASTNode * Func_Decl_Prime();

        ASTNode * Type_Name();

        ASTNode * Type_Name_Prime();

        ASTNode * Parameter_List();

        ASTNode * Parameter_List_Tail();

        ASTNode * Non_Empty_List();

        ASTNode * Non_Empty_List_Prime();

        ASTNode * Data_Decls();

        ASTNode * Data_Func_Decl();

        ASTNode * Id_List();

        ASTNode * Id_List_Prime();

        ASTNode * Id();

        ASTNode * Id_Tail();

        ASTNode * Block_Statements();

        ASTNode * Statements();

        ASTNode * Statement();

        ASTNode * Statement_Tail();

        ASTNode * Assignment();

        ASTNode * Assignment_Tail();

        ASTNode * Func_Call_Tail();

        ASTNode * Expr_List();

        ASTNode * Non_Empty_Expr_List();

        ASTNode * Non_Empty_Expr_List_Prime();

        ASTNode * If_Statement();

        ASTNode * Condition_Expression();

        ASTNode * Condition_Expression_Tail();
               
        ASTNode * Condition_Op();

        ASTNode * Condition();

        ASTNode * Comparison_Op();

        ASTNode * While_Statement();

        ASTNode * Return_Statement();
        
        ASTNode * Return_Statement_Tail();

        ASTNode * Break_Statement();

        ASTNode * Continue_Statement();

        ASTNode * Expression();

        ASTNode * Expression_Prime();

        ASTNode * Addop();

        ASTNode * Term();

        ASTNode * Term_Prime();

        ASTNode * Mulop();

        ASTNode * Factor();

        ASTNode * Factor_Tail();
};
#endif /* GRAMMAR_H */
