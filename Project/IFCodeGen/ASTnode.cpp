#include "ASTnode.h"

#include <string>
#include <vector>

using namespace::std;

// @ComparisonExpr
ComparisonExpr::ComparisonExpr(string o, ExprNode *l, ExprNode *r)
    :op(o), left(l), right(r) {}

ComparisonExpr::~ComparisonExpr() {
    delete left;
    delete right;
}

// @ConditionExpr
ConditionExpr::ConditionExpr(string o, ExprNode*l, ExprNode*r)
    :op(o), left(l), right(r) {}

ConditionExpr::~ConditionExpr() {
    delete left;
    delete right;
}

// @BinaryExpr
BinaryExpr::BinaryExpr(string o, ExprNode *l, ExprNode *r)
    :op(o), left(l), right(r) {}

BinaryExpr::~BinaryExpr() {
    delete left;
    delete right;
}

// @UnaryExpr
UnaryExpr::UnaryExpr(string o, ExprNode *c)
    :op(o), child(c) {}

UnaryExpr::~UnaryExpr() {
    delete child;
}

// @ParenExpr
ParenExpr::ParenExpr(ExprNode *c)
    :child(c) {}

// @VariableExpr
VariableExpr::VariableExpr(string n)
    :name(n) {}

// @NumberExpr
NumberExpr::NumberExpr(string v)
    :val(v) {}

// @FuncCallExpr
FuncCallExpr::FuncCallExpr(string n, vector<ExprNode*> *a)
    :name(n), args(a) {}

FuncCallExpr::~FuncCallExpr() {
    vector<ExprNode*>::iterator it;
    for (it = args->begin(); it != args->end(); ++it) {
        delete (*it);
    }
    delete args;
}

// @ArrayRefExpr
ArrayRefExpr::ArrayRefExpr(string n, ExprNode *i)
    :name(n), index(i) {}

ArrayRefExpr::~ArrayRefExpr() {
    delete index;
}

// @ParamListExpr
ParamListExpr::ParamListExpr(vector<ExprNode*>*p)
    :params(p) {}

ParamListExpr::~ParamListExpr() {
    vector<ExprNode*>::iterator it;
    for (it = params->begin(); it != params->end(); ++it) {
        delete (*it);
    }
    delete params;
}

// @ParamExpr
ParamExpr::ParamExpr(string t, string n)
    :type(t), name(n) {}

// @ProgramStmt
ProgramStmt::ProgramStmt(vector<StmtNode*>*d, vector<StmtNode*>*f)
    :data_decls(d), func_decls(f) {}

ProgramStmt::~ProgramStmt() {
    vector<StmtNode*>::iterator it;
    for (it = data_decls->begin(); it != data_decls->end(); ++it) {
        delete (*it);
    }
    delete data_decls;
    vector<StmtNode*>::iterator it2;
    for (it2 = func_decls->begin(); it2 != func_decls->end(); ++it2) {
        delete (*it2);
    }
    delete func_decls;
}

// @FuncDeclStmt
FuncDeclStmt::FuncDeclStmt(string t, string n, ExprNode*p, vector<StmtNode*>* dd, vector<StmtNode*>* b)
    :type(t), name(n), params(p), data_decls(dd), body(b) {}

FuncDeclStmt::~FuncDeclStmt() {
    delete params;
    vector<StmtNode*>::iterator it;
    for (it = data_decls->begin(); it != data_decls->end(); ++it) {
        delete (*it);
    }
    delete data_decls;
    vector<StmtNode*>::iterator it2;
    for (it2 = body->begin(); it2 != body->end(); ++it2) {
        delete (*it2);
    }
    delete body;
}
// @DataDeclsStmt
DataDeclsStmt::DataDeclsStmt(string t, vector<ExprNode*>*i)
    :type(t), ids(i) {}

DataDeclsStmt::~DataDeclsStmt() {
    vector<ExprNode*>::iterator it;
    for (it = ids->begin(); it != ids->end(); ++it) {
        delete (*it);
    }
    delete ids;
}
// @BlockStmt
BlockStmt::BlockStmt(vector<StmtNode*> *s)
    :statements(s) {}

BlockStmt::~BlockStmt() {
    vector<StmtNode*>::iterator it;
    for (it = statements->begin(); it != statements->end(); ++it) {
        delete (*it);
    }
    delete statements;
}

// @FuncCallStmt
FuncCallStmt::FuncCallStmt(string n, vector<ExprNode*> *a)
    :name(n), args(a) {}

FuncCallStmt::~FuncCallStmt() {
    vector<ExprNode*>::iterator it;
    for (it = args->begin(); it != args->end(); ++it) {
        delete (*it);
    }
    delete args;
}

// @AssignmentStmt
AssignmentStmt::AssignmentStmt(ExprNode* l, ExprNode* r)
    :lhs(l), rhs(r) {}

AssignmentStmt::~AssignmentStmt() {
    delete lhs;
    delete rhs;
}

// @IfStmt
IfStmt::IfStmt(ExprNode *c, StmtNode *b)
    :condition(c), body(b) {}

IfStmt::~IfStmt() {
    delete condition;
    delete body;
}

// @WhileStmt
WhileStmt::WhileStmt(ExprNode *c, StmtNode *b)
    :condition(c), body(b) {}

WhileStmt::~WhileStmt() {
    delete condition;
    delete body;
}

// @ReturnStmt
ReturnStmt::ReturnStmt(ExprNode* e)
    :expr(e) {}

ReturnStmt::~ReturnStmt() {
    delete expr;
}

// @BreakStmt

// @ContinueStmt

// @ReadStmt
ReadStmt::ReadStmt(string n)
    :name(n) {}

// @WriteStmt
WriteStmt::WriteStmt(ExprNode* e)
    :expr(e) {}

WriteStmt::~WriteStmt() {
    delete expr;
}

// @PrintStmt
PrintStmt::PrintStmt(string v)
    : val(v) {}
