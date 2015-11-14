#ifndef ASTNODE_H
#define ASTNODE_H 

#include <string>
#include <vector>

using namespace::std;

class ASTNode {
};

class ExprNode : public ASTNode{
};

class ComparisonExpr : public ExprNode {
    private:
        string op;
        ExprNode * left;
        ExprNode * right;
    public:
        ComparisonExpr(string o, ExprNode *l, ExprNode *r);
        ~ComparisonExpr();
};

class ConditionExpr : public ExprNode {
    private:
        string op;
        ExprNode* left;
        ExprNode* right;
    public:
        ConditionExpr(string o, ExprNode* l, ExprNode*r);
        ~ConditionExpr();
};

class BinaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * left;
        ExprNode * right;
    public:
        BinaryExpr(string o, ExprNode* l, ExprNode* r);
        ~BinaryExpr();
};

class UnaryExpr : public ExprNode {
    private:
        string op;
        ExprNode * child;
    public:
        UnaryExpr(string o, ExprNode* c);
        ~UnaryExpr();
};

class ParenExpr : public ExprNode {
    private:
        ExprNode * child;
    public:
        ParenExpr(ExprNode* c);
        ~ParenExpr();
};

class VariableExpr : public ExprNode {
    private:
        string name;
    public:
        VariableExpr(string n);
};

class NumberExpr : public ExprNode {
    private:
        string val;
    public:
        NumberExpr(string v);
};

class FuncCallExpr : public ExprNode {
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallExpr(string n, vector<ExprNode*> *a);
        ~FuncCallExpr();
};

class ArrayRefExpr : public ExprNode {
    private:
        string name;
        ExprNode * index;
    public:
        ArrayRefExpr(string n, ExprNode *i);
        ~ArrayRefExpr();
};

class ParamListExpr : public ExprNode {
    private:
        vector<ExprNode*> * params;
    public:
        ParamListExpr(vector<ExprNode*> *p);
        ~ParamListExpr();
};

class ParamExpr : public ExprNode {
    private:
        string type;
        string name;
    public:
        ParamExpr(string t, string n);
};

class StmtNode : ASTNode{
};

class ProgramStmt: public StmtNode {
    private:
        vector<StmtNode*>* data_decls;
        vector<StmtNode*>* func_decls;
    public:
        ProgramStmt(vector<StmtNode*>* d, vector<StmtNode*>* f);
        ~ProgramStmt();
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
        ~FuncDeclStmt();
};

class DataDeclsStmt: public StmtNode {
    private:
        string type;
        vector<ExprNode*> *ids;
    public:
        DataDeclsStmt(string t, vector<ExprNode*>*i);
        ~DataDeclsStmt();
};

class BlockStmt: public StmtNode {
    private:
        vector<StmtNode*> *statements;
    public:
        BlockStmt(vector<StmtNode*>*s);
        ~BlockStmt();
};

class FuncCallStmt: public StmtNode{
    private:
        string name;
        vector<ExprNode*> *args;
    public:
        FuncCallStmt(string n, vector<ExprNode*> *a);
        ~FuncCallStmt();
};

class AssignmentStmt: public StmtNode {
    private:
        ExprNode * lhs;
        ExprNode * rhs;
    public:
        AssignmentStmt(ExprNode *l, ExprNode *r);
        ~AssignmentStmt();
};

class IfStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        IfStmt(ExprNode *c, StmtNode *b);
        ~IfStmt();
};

class WhileStmt : public StmtNode {
    private:
        ExprNode * condition;
        StmtNode * body;
    public:
        WhileStmt(ExprNode *c, StmtNode *b);
        ~WhileStmt();
};

class ReturnStmt : public StmtNode {
    private:
        ExprNode * expr;
    public:
        ReturnStmt(ExprNode* e);
        ~ReturnStmt();
};

class BreakStmt : public StmtNode {
    private:
    public:
};

class ContinueStmt : public StmtNode {
    private:
    public:
};

class ReadStmt : public StmtNode {
    private:
        string name;
    public:
        ReadStmt(string n);
};

class WriteStmt : public StmtNode {
    private:
        ExprNode *expr;
    public:
        WriteStmt(ExprNode* e);
        ~WriteStmt();
};

class PrintStmt : public StmtNode {
    private:
        string val;
    public:
        PrintStmt(string v);
};
#endif /* ASTNODE_H */
