#ifndef ASTNODE_H
#define ASTNODE_H 
#include "label.h"
#include "address.h"

#include <string>
#include <vector>
#include <fstream>

using namespace::std;

extern ofstream output;
class ExprNode{
    public:
        virtual~ExprNode() {};
        virtual Address* genCode() = 0;
};

class ComparisonExpr : public ExprNode {
    private:
        string op;
        ExprNode * left;
        ExprNode * right;
    public:
        ComparisonExpr(string o, ExprNode *l, ExprNode *r);
        virtual~ComparisonExpr();
        virtual Address* genCode();
};

class ConditionExpr : public ExprNode {
    private:
        string op;
        ExprNode* left;
        ExprNode* right;
    public:
        ConditionExpr(string o, ExprNode* l, ExprNode*r);
        virtual~ConditionExpr();
        virtual Address* genCode();
};

class BinaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * left;
        ExprNode * right;
    public:
        BinaryExpr(string o, ExprNode* l, ExprNode* r);
        virtual~BinaryExpr();
        virtual Address* genCode();
};

class UnaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * child;
    public:
        UnaryExpr(string o, ExprNode* c);
        virtual~UnaryExpr();
        virtual Address* genCode();
};

class ParenExpr : public ExprNode {
    private:
        ExprNode * child;
    public:
        ParenExpr(ExprNode* c);
        virtual~ParenExpr();
        virtual Address* genCode();
};

class VariableExpr : public ExprNode {
    private:
        string name;
    public:
        VariableExpr(string n);
        virtual Address* genCode();
};

class NumberExpr : public ExprNode {
    private:
        string val;
    public:
        NumberExpr(string v);
        virtual Address* genCode();
};

class FuncCallExpr : public ExprNode {
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallExpr(string n, vector<ExprNode*> *a);
        virtual~FuncCallExpr();
        virtual Address* genCode();
};

class ArrayRefExpr : public ExprNode {
    private:
        string name;
        ExprNode * index;
    public:
        ArrayRefExpr(string n, ExprNode *i);
        virtual~ArrayRefExpr();
        virtual Address* genCode();
};

class ParamListExpr : public ExprNode {
    private:
        vector<ExprNode*> * params;
    public:
        ParamListExpr(vector<ExprNode*> *p);
        virtual~ParamListExpr();
        virtual Address* genCode();
};

class ParamExpr : public ExprNode {
    private:
        string type;
        string name;
    public:
        ParamExpr(string t, string n);
        virtual Address* genCode();
};

class StmtNode{
    public:
        virtual~StmtNode() {}
        virtual void genCode(Label& next) = 0;
};

class ProgramStmt: public StmtNode {
    private:
        vector<StmtNode*>* data_decls;
        vector<StmtNode*>* func_decls;
    public:
        ProgramStmt(vector<StmtNode*>* d, vector<StmtNode*>* f);
        virtual~ProgramStmt();
        virtual void genCode(Label& next);
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
        virtual void genCode(Label& next);
};

class DataDeclsStmt: public StmtNode {
    private:
        string type;
        vector<ExprNode*> *ids;
    public:
        DataDeclsStmt(string t, vector<ExprNode*>*i);
        virtual~DataDeclsStmt();
        virtual void genCode(Label& next);
};

class BlockStmt: public StmtNode {
    private:
        vector<StmtNode*> *statements;
    public:
        BlockStmt(vector<StmtNode*>*s);
        virtual~BlockStmt();
        virtual void genCode(Label& next);
        virtual void genCode(Label& yes, Label& no); 
};

class FuncCallStmt: public StmtNode{
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallStmt(string n, vector<ExprNode*> *a);
        virtual~FuncCallStmt();
        virtual void genCode(Label& next);
};

class AssignmentStmt: public StmtNode {
    private:
        ExprNode * lhs;
        ExprNode * rhs;
    public:
        AssignmentStmt(ExprNode *l, ExprNode *r);
        virtual~AssignmentStmt();
        virtual void genCode(Label& next);
};

class IfStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        IfStmt(ExprNode *c, StmtNode *b);
        virtual~IfStmt();
        virtual void genCode(Label& next);
};

class WhileStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        WhileStmt(ExprNode *c, StmtNode *b);
        virtual~WhileStmt();
        virtual void genCode(Label& next);
};

class ReturnStmt : public StmtNode {
    private:
        ExprNode * expr;
    public:
        ReturnStmt(ExprNode* e);
        virtual~ReturnStmt();
        virtual void genCode(Label& next);
};

class BreakStmt : public StmtNode {
    private:
    public:
        virtual void genCode(Label& no);
};

class ContinueStmt : public StmtNode {
    private:
    public:
        virtual void genCode(Label& yes);
};

class ReadStmt : public StmtNode {
    private:
        string name;
    public:
        ReadStmt(string n);
        virtual void genCode(Label& next);
};

class WriteStmt : public StmtNode {
    private:
        ExprNode *expr;
    public:
        WriteStmt(ExprNode* e);
        virtual~WriteStmt();
        virtual void genCode(Label& next);
};

class PrintStmt : public StmtNode {
    private:
        string val;
    public:
        PrintStmt(string v);
        virtual void genCode(Label& next);
};
#endif /* ASTNODE_H */
