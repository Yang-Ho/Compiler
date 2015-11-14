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
