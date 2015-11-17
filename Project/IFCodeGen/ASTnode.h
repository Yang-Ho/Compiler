#ifndef ASTNODE_H
#define ASTNODE_H 
#include "label.h"
#include "symbol.h"

#include <string>
#include <vector>
#include <fstream>

using namespace::std;

extern ofstream output;
class ExprNode{
    public:
        virtual~ExprNode() {};
        virtual string  genCode(SymbolTable* st) = 0;
};

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

class UnaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * child;
    public:
        UnaryExpr(string o, ExprNode* c);
        virtual~UnaryExpr();
        virtual string  genCode(SymbolTable* st);
};

class ParenExpr : public ExprNode {
    private:
        ExprNode * child;
    public:
        ParenExpr(ExprNode* c);
        virtual~ParenExpr();
        virtual string  genCode(SymbolTable* st);
};

class VariableExpr : public ExprNode {
    private:
        string name;
    public:
        VariableExpr(string n);
        virtual string  genCode(SymbolTable* st);
};

class NumberExpr : public ExprNode {
    private:
        string val;
    public:
        NumberExpr(string v);
        virtual string  genCode(SymbolTable* st);
};

class FuncCallExpr : public ExprNode {
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallExpr(string n, vector<ExprNode*> *a);
        virtual~FuncCallExpr();
        virtual string  genCode(SymbolTable* st);
};

class ArrayRefExpr : public ExprNode {
    private:
        string name;
        ExprNode * index;
    public:
        ArrayRefExpr(string n, ExprNode *i);
        virtual~ArrayRefExpr();
        virtual string  genCode(SymbolTable* st);
};

class ParamListExpr : public ExprNode {
    private:
        vector<ExprNode*> * params;
    public:
        ParamListExpr(vector<ExprNode*> *p);
        virtual~ParamListExpr();
        virtual string  genCode(SymbolTable* st);
};

class ParamExpr : public ExprNode {
    private:
        string type;
        string name;
    public:
        ParamExpr(string t, string n);
        virtual string  genCode(SymbolTable* st);
};

class StmtNode{
    public:
        virtual~StmtNode() {}
        virtual void genCode(SymbolTable* st) = 0;
};

class ProgramStmt: public StmtNode {
    private:
        vector<StmtNode*>* data_decls;
        vector<StmtNode*>* func_decls;
    public:
        ProgramStmt(vector<StmtNode*>* d, vector<StmtNode*>* f);
        virtual~ProgramStmt();
        virtual void genCode(SymbolTable* st);
};

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

class DataDeclsStmt: public StmtNode {
    private:
        string type;
        vector<ExprNode*> *ids;
    public:
        DataDeclsStmt(string t, vector<ExprNode*>*i);
        virtual~DataDeclsStmt();
        virtual void genCode(SymbolTable* st);
};

class BlockStmt: public StmtNode {
    private:
        vector<StmtNode*> *statements;
    public:
        BlockStmt(vector<StmtNode*>*s);
        virtual~BlockStmt();
        virtual void genCode(SymbolTable* st);
};

class FuncCallStmt: public StmtNode{
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallStmt(string n, vector<ExprNode*> *a);
        virtual~FuncCallStmt();
        virtual void genCode(SymbolTable* st);
};

class AssignmentStmt: public StmtNode {
    private:
        ExprNode * lhs;
        ExprNode * rhs;
    public:
        AssignmentStmt(ExprNode *l, ExprNode *r);
        virtual~AssignmentStmt();
        virtual void genCode(SymbolTable* st);
};

class IfStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        IfStmt(ExprNode *c, StmtNode *b);
        virtual~IfStmt();
        virtual void genCode(SymbolTable* st);
};

class WhileStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        WhileStmt(ExprNode *c, StmtNode *b);
        virtual~WhileStmt();
        virtual void genCode(SymbolTable* st);
};

class ReturnStmt : public StmtNode {
    private:
        ExprNode * expr;
    public:
        ReturnStmt(ExprNode* e);
        virtual~ReturnStmt();
        virtual void genCode(SymbolTable* st);
};

class BreakStmt : public StmtNode {
    private:
    public:
        virtual void genCode(SymbolTable* st);
};

class ContinueStmt : public StmtNode {
    private:
    public:
        virtual void genCode(SymbolTable* st);
};

class ReadStmt : public StmtNode {
    private:
        string name;
    public:
        ReadStmt(string n);
        virtual void genCode(SymbolTable* st);
};

class WriteStmt : public StmtNode {
    private:
        ExprNode *expr;
    public:
        WriteStmt(ExprNode* e);
        virtual~WriteStmt();
        virtual void genCode(SymbolTable* st);
};

class PrintStmt : public StmtNode {
    private:
        string val;
    public:
        PrintStmt(string v);
        virtual void genCode(SymbolTable* st);
};
#endif /* ASTNODE_H */
