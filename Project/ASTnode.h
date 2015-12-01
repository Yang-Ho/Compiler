/*
 * Yang Ho
 * CSC 512
 * ASTnode.h
 *
 * Header file for my abstract syntax tree implementation
 */
#ifndef ASTNODE_H
#define ASTNODE_H 
#include "label.h"
#include "symbol.h"

#include <string>
#include <vector>
#include <fstream>

using namespace::std;

extern ofstream output; // Output file

/*
 * Base expression node for AST
 * Expression is anything that doesn't end with a ; or has {}
 */
class ExprNode{
    public:
        virtual~ExprNode() {};
        virtual string  genCode(SymbolTable* st) = 0; // Generates code
};

/*
 * Node for comparisons
 *      left : left operand 
 *      right : right operand 
 *      op : ==, !=, >, >=, <, <= 
 */
class ComparisonExpr : public ExprNode {
    private:
        string op;
        ExprNode * left;
        ExprNode * right;
    public:
        ComparisonExpr(string o, ExprNode *l, ExprNode *r);
        virtual~ComparisonExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for conditions
 *      left : left operand 
 *      right : right operand 
 *      op : &&, ||
 */
class ConditionExpr : public ExprNode {
    private:
        string op;
        ExprNode* left;
        ExprNode* right;
    public:
        ConditionExpr(string o, ExprNode* l, ExprNode*r);
        virtual~ConditionExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for binary operations 
 *      left : left operand 
 *      right : right operand 
 *      op : +, -, *, /
 */
class BinaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * left;
        ExprNode * right;
    public:
        BinaryExpr(string o, ExprNode* l, ExprNode* r);
        virtual~BinaryExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for unary operations 
 *      child: operand 
 *      op : don't know, our grammar doesn't really need this (i store negative numbers as negative numbers, not - and a number
 */
class UnaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * child;
    public:
        UnaryExpr(string o, ExprNode* c);
        virtual~UnaryExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for expressions in parens 
 *      child: The expression
 */
class ParenExpr : public ExprNode {
    private:
        ExprNode * child;
    public:
        ParenExpr(ExprNode* c);
        virtual~ParenExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for variables (ID's of variables, funcs, etc)
 *      name : ID 
 */
class VariableExpr : public ExprNode {
    private:
        string name;
    public:
        VariableExpr(string n);
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for numbers
 *      val : number value in string format 
 */
class NumberExpr : public ExprNode {
    private:
        string val;
    public:
        NumberExpr(string v);
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for a func call expression 
 *      name : ID
 *      args : list of arguments 
 */
class FuncCallExpr : public ExprNode {
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallExpr(string n, vector<ExprNode*> *a);
        virtual~FuncCallExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for a array reference expression
 *      name : ID
 *      index : the index being referenced 
 */
class ArrayRefExpr : public ExprNode {
    private:
        string name;
        ExprNode * index;
    public:
        ArrayRefExpr(string n, ExprNode *i);
        virtual~ArrayRefExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for parameter list in func decl
 *      params : list of parameters 
 */
class ParamListExpr : public ExprNode {
    private:
        vector<ExprNode*> * params;
    public:
        ParamListExpr(vector<ExprNode*> *p);
        virtual~ParamListExpr();
        virtual string  genCode(SymbolTable* st);
};

/*
 * Node for specific paramaters
 *      type : the type of the param 
 *      name : the ID of the param 
 */
class ParamExpr : public ExprNode {
    private:
        string type;
        string name;
    public:
        ParamExpr(string t, string n);
        virtual string  genCode(SymbolTable* st);
};

/*
 * Base class for stmt nodes of AST
 * Statements are anything that end with ; or have {}
 */
class StmtNode{
    public:
        virtual~StmtNode() {}
        virtual void genCode(SymbolTable* st) = 0;
};

/* 
 * Base Program statement
 *      data_decls : list of global variables 
 *      func_decls : list of func decls
 */
class ProgramStmt: public StmtNode {
    private:
        vector<StmtNode*>* data_decls;
        vector<StmtNode*>* func_decls;
    public:
        ProgramStmt(vector<StmtNode*>* d, vector<StmtNode*>* f);
        virtual~ProgramStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Func decl stmt
 *      type : return type
 *      name : ID
 *      params : list of parameters
 *      data_decls : declartion of local variables
 *      body : main body of the program
 */
class FuncDeclStmt: public StmtNode {
    private:
        string type;
        string name;
        ExprNode *params;
        vector<StmtNode*>* data_decls;
        vector<StmtNode*>* body;
    public:
        FuncDeclStmt(string t, string n, ExprNode *p, vector<StmtNode*>* dd, vector<StmtNode*>* b);
        virtual~FuncDeclStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Data declaration statements
 *      type : variable types
 *      ids : list of IDs
 */
class DataDeclsStmt: public StmtNode {
    private:
        string type;
        vector<ExprNode*> *ids;
    public:
        DataDeclsStmt(string t, vector<ExprNode*>*i);
        virtual~DataDeclsStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Block statements (statments in {})
 *      statements : list of statements in the body
 */
class BlockStmt: public StmtNode {
    private:
        vector<StmtNode*> *statements;
    public:
        BlockStmt(vector<StmtNode*>*s);
        virtual~BlockStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Func call statement
 *      See FuncCallExpr
 */
class FuncCallStmt: public StmtNode{
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallStmt(string n, vector<ExprNode*> *a);
        virtual~FuncCallStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Assignment statment
 *      lhs : variable you're changing
 *      rhs : what your storing into the lhs
 */
class AssignmentStmt: public StmtNode {
    private:
        ExprNode * lhs;
        ExprNode * rhs;
    public:
        AssignmentStmt(ExprNode *l, ExprNode *r);
        virtual~AssignmentStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * If statement
 *      condition : the condition being checked
 *      body : what to do if the condition is true
 */
class IfStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        IfStmt(ExprNode *c, StmtNode *b);
        virtual~IfStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * While statement
 *      condition : the condition being checked
 *      body : what to do if the condition is true
 */
class WhileStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        WhileStmt(ExprNode *c, StmtNode *b);
        virtual~WhileStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Return statement
 *      expr : what to return
 */
class ReturnStmt : public StmtNode {
    private:
        ExprNode * expr;
    public:
        ReturnStmt(ExprNode* e);
        virtual~ReturnStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Break statement
 */
class BreakStmt : public StmtNode {
    private:
    public:
        virtual void genCode(SymbolTable* st);
};

/*
 * Continue statement
 */
class ContinueStmt : public StmtNode {
    private:
    public:
        virtual void genCode(SymbolTable* st);
};

/*
 * Read statement
 *      name : what to read in
 */
class ReadStmt : public StmtNode {
    private:
        string name;
    public:
        ReadStmt(string n);
        virtual void genCode(SymbolTable* st);
};

/*
 * Write statement
 *      expr : what to write
 */
class WriteStmt : public StmtNode {
    private:
        ExprNode *expr;
    public:
        WriteStmt(ExprNode* e);
        virtual~WriteStmt();
        virtual void genCode(SymbolTable* st);
};

/*
 * Print statement
 *      val : what to print
 */
class PrintStmt : public StmtNode {
    private:
        string val;
    public:
        PrintStmt(string v);
        virtual void genCode(SymbolTable* st);
};
#endif /* ASTNODE_H */
