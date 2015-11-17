/*
 * Yang Ho
 * CSC 512
 * grammar.h
 *
 * Recursive descent parser for the grammar
 * Original implementation is a mix between my own parser for project 2 and the sample on provided
 *
 * I used the overall structure, production rules, and organization of the sample
 * But used my own implementation of the sample parser 
 */
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
        StmtNode* Program();

        StmtNode* Program_Prime(string type, string id);

        StmtNode* Data_Decls_Prime(string type, string id);

        vector<StmtNode*>* Func_List_Prime(string type, string id);

        vector<StmtNode*>* Func_List();

        StmtNode* Func();

        ExprNode* Parameter_List();

        vector<StmtNode*>* Data_Decls();

        vector<ExprNode*>* Id_List();

        ExprNode* Id();

        StmtNode * Block_Statements();

        vector<StmtNode*>* Statements();

        StmtNode * Statement();

        vector<ExprNode*> * Expr_List();

        vector<ExprNode*> * Non_Empty_Expr_List();

        StmtNode * If_Statement();

        ExprNode* Condition_Expression();

        ExprNode * Condition();

        StmtNode * While_Statement();

        StmtNode * Return_Statement();
        
        StmtNode * Return_Statement_Tail();

        StmtNode * Break_Statement();

        StmtNode * Continue_Statement();

        ExprNode* Expression();

        ExprNode* Term();

        ExprNode * Factor();
};
#endif /* GRAMMAR_H */
