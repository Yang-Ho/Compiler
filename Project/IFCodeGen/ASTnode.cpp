#include "ASTnode.h"

#include <string>
#include <vector>

#include <iostream>
#include <fstream>
using namespace::std;

// @ComparisonExpr
ComparisonExpr::ComparisonExpr(string o, ExprNode *l, ExprNode *r)
    :op(o), left(l), right(r) {}

ComparisonExpr::~ComparisonExpr() {
    delete left;
    delete right;
}

Address* ComparisonExpr::genCode() {
    cout<<"Generating code: ComparisonExpr\n";
    Address* resultl = left->genCode();
    Address* resultr = right->genCode();

    Address* result = new TempAddress();
    output<<result->str()<<"="<<resultl->str()<<op<<resultr->str()<<";\n";
    return result;
}

// @ConditionExpr
ConditionExpr::ConditionExpr(string o, ExprNode*l, ExprNode*r)
    :op(o), left(l), right(r) {}

ConditionExpr::~ConditionExpr() {
    delete left;
    delete right;
}

Address* ConditionExpr::genCode() {
    cout<<"Generating code: ConditionExpr\n";
    Address* resultl = left->genCode();
    Address* resultr = right->genCode();

    Address* result = new TempAddress();
    output<<result->str()<<"="<<resultl->str()<<op<<resultr->str()<<";\n";
    return result;
}

// @BinaryExpr
BinaryExpr::BinaryExpr(string o, ExprNode *l, ExprNode *r)
    :op(o), left(l), right(r) {}

BinaryExpr::~BinaryExpr() {
    delete left;
    delete right;
}

Address* BinaryExpr::genCode() {
    cout<<"Generating code: BinaryExpr\n";
    Address* resultl = left->genCode();
    Address* resultr = right->genCode();

    Address* result = new TempAddress();
    output<<result->str()<<"="<<resultl->str()<<op<<resultr->str()<<";\n";
    return result;
}

// @UnaryExpr
UnaryExpr::UnaryExpr(string o, ExprNode *c)
    :op(o), child(c) {}

UnaryExpr::~UnaryExpr() {
    delete child;
}

Address* UnaryExpr::genCode() {
    cout<<"Generating code: UnaryExpr\n";
    Address* resultc = child->genCode();
    Address* result = new TempAddress();

    output<<result->str()<<"="<<op<<resultc->str()<<";\n";
    return result;
}

// @ParenExpr
ParenExpr::ParenExpr(ExprNode *c)
    :child(c) {}

ParenExpr::~ParenExpr() {
    delete child;
}

Address* ParenExpr::genCode() {
    cout<<"Generating code: ParenExpr\n";
    Address* resultc = child->genCode();
    Address* result = new TempAddress();

    output<<result->str()<<"="<<"("<<resultc->str()<<");\n";
    return result;
}

// @VariableExpr
VariableExpr::VariableExpr(string n)
    :name(n) {}

Address* VariableExpr::genCode() {
    cout<<"Generating code: VariableExpr\n";
    Address* result = new VarAddress(name);

    return result;
}

// @NumberExpr
NumberExpr::NumberExpr(string v)
    :val(v) {}

Address* NumberExpr::genCode() {
    cout<<"Generating code: NumberExpr\n";
    Address* result = new TempAddress();

    output<<result->str()<<"="<<val<<";\n";
    return result;
}

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

Address* FuncCallExpr::genCode() {
    cout<<"Generating code: FuncCallExpr\n";
    Address* result = new TempAddress();

    output<<result->str()<<"="<<name<<"(";
    if (args) {
        vector<ExprNode*>::iterator it;
        for (it = args->begin(); it != args->end(); ++it) {
            if (*it) {
                Address* r = (*it)->genCode();
                cout<<r->str();
                if (it + 1 != args->end()) {
                    cout<<", ";
                }
            }
        }
    }
    output<<");\n";
    return result;
}

// @ArrayRefExpr
ArrayRefExpr::ArrayRefExpr(string n, ExprNode *i)
    :name(n), index(i) {}

ArrayRefExpr::~ArrayRefExpr() {
    delete index;
}

Address* ArrayRefExpr::genCode() {
    output<<"Generating code: ArrayRefExpr\n";
    Address* resulti = index->genCode();
    Address* result = new TempAddress();

    output<<result->str()<<"="<<name<<"["<<resulti->str()<<"];\n";

    return result;
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

Address* ParamListExpr::genCode() {
    cout<<"Generating code: ParamListExpr\n";
    vector<ExprNode*>::iterator pit;
    for (pit = params->begin(); pit != params->end(); pit++) {
        (*pit)->genCode();
    }
    return new TempAddress();
}

// @ParamExpr
ParamExpr::ParamExpr(string t, string n)
    :type(t), name(n) {}

Address* ParamExpr::genCode() {
    cout<<"Generating code: ParamExpr\n";
    Address* result = new VarAddress(name, VAR_PARAM);
    output<<type<<" "<<name;
    return result;
}

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

void ProgramStmt::genCode(Label& next) {
    cout<<"Generating code: ProgramStmt\n";
    vector<StmtNode*>::iterator ddit;
    vector<StmtNode*>::iterator fdit;
    if (data_decls) {
        for (ddit = data_decls->begin(); ddit != data_decls->end(); ++ddit) {
            (*ddit)->genCode(next);
        }
    }
    for (fdit = func_decls->begin(); fdit != func_decls->end(); ++fdit) {
        (*fdit)->genCode(next);
    }
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

void FuncDeclStmt::genCode(Label& next) {
    cout<<"Generating code: FuncDeclStmt\n";
    output<<type<<" "<<name<<"(";
    if (params) {
        params->genCode();
    }
    output<<")";
    Address* temp = new TempAddress(true);
    if (data_decls) {
        output<<"{\n";
        vector<StmtNode*>::iterator ddit;
        for (ddit = data_decls->begin(); ddit != data_decls->end(); ddit++) {
            if (*ddit)
                (*ddit)->genCode(next);
        }
        if (body) {
            if (data_decls->empty()) {
                output<<type<<" "<<"local["<<5<<"];\n";
            }
            vector<StmtNode*>::iterator bit;
            for (bit = body->begin(); bit != body->end(); bit++) {
                if (*bit)
                    (*bit)->genCode(next);
            }
        }
        output<<"}";
    }
    output<<";\n";
    delete temp;
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

void DataDeclsStmt::genCode(Label& next) {
    cout<<"Generating code: DataDeclsStmt\n";
    if (ids) {// To do, figure out how many local vars are in a function
        output<<type<<" local["<<ids->size()<<"];\n";
    }
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

void BlockStmt::genCode(Label& next) {
    cout<<"Generating code: BlockStmt\n";
    if (statements) {
        vector<StmtNode*>::iterator it;
        for (it = statements->begin(); it != statements->end(); ++it) {
            (*it)->genCode(next);
        }
    }
}

void BlockStmt::genCode(Label& yes, Label& no) {
    cout<<"Block statment inside while loop\n";
    if (statements) {
        vector<StmtNode*>::iterator it;
        for (it = statements->begin(); it != statements->end(); ++it) {
            (*it)->genCode(yes);
        }
    }
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

void FuncCallStmt::genCode(Label& next) {
    cout<<"Generating code: FuncCallStmt\n";
    output<<name<<"(";
    if (args) {
        vector<ExprNode*>::iterator it;
        for (it = args->begin(); it != args->end(); ++it) {
            if (*it) {
                Address* r = (*it)->genCode();
                cout<<r->str();
                if (it + 1 != args->end()) {
                    cout<<", ";
                }
            }
        }
    }
    output<<");\n";
}
// @AssignmentStmt
AssignmentStmt::AssignmentStmt(ExprNode* l, ExprNode* r)
    :lhs(l), rhs(r) {}

AssignmentStmt::~AssignmentStmt() {
    delete lhs;
    delete rhs;
}

void AssignmentStmt::genCode(Label& next) {
    cout<<"Generating code: AssignmentStmt\n";
    Address* resultl = lhs->genCode();
    Address* resultr = rhs->genCode();

    output<<resultl->str()<<"="<<resultr->str()<<";\n";
}
// @IfStmt
IfStmt::IfStmt(ExprNode *c, StmtNode *b)
    :condition(c), body(b) {}

IfStmt::~IfStmt() {
    delete condition;
    delete body;
}

void IfStmt::genCode(Label& next) {
    cout<<"Generating code: IfStmt\n";
    Label falseLabel;
    Address* result = condition->genCode();
    output<<"if ("<<result->str()<<") goto "<<next.str()<<";\n";  
    output<<"goto "<<falseLabel.str()<<";\n";
    output<<next.str()<<":\n";
    body->genCode(falseLabel);
    output<<falseLabel.str()<<":\n";
}
// @WhileStmt
WhileStmt::WhileStmt(ExprNode *c, StmtNode *b)
    :condition(c), body(b) {}

WhileStmt::~WhileStmt() {
    delete condition;
    delete body;
}

void WhileStmt::genCode(Label& next) {
    cout<<"Generating code: WhileStmt\n";
    Label trueLabel;
    Label falseLabel;
    Address* result = condition->genCode();
    output<<next.str()<<":\n";
    output<<"if ("<<result->str()<<") goto "<<trueLabel.str()<<";\n";
    output<<"goto "<<falseLabel.str()<<";\n";
    output<<trueLabel.str()<<":\n";
    //body->genCode(trueLabel, falseLabel);
    body->genCode(trueLabel);
    output<<"goto "<<next.str()<<";\n";
    output<<falseLabel.str()<<":\n";
}

// @ReturnStmt
ReturnStmt::ReturnStmt(ExprNode* e)
    :expr(e) {}

ReturnStmt::~ReturnStmt() {
    delete expr;
}

void ReturnStmt::genCode(Label& next) {
    cout<<"Generating code: ReturnStmt\n";
    if (expr) {
        Address * result = expr->genCode();
        output<<"return "<<result->str()<<";\n";
    } else {
        output<<"return ;\n";
    }
}

// @BreakStmt
void BreakStmt::genCode(Label& no) {
    cout<<"Generating code: BreakStmt\n";
    output<<"goto "<<no.str()<<";\n";
}

// @ContinueStmt
void ContinueStmt::genCode(Label& yes) {
    cout<<"Generating code: ContinueStmt\n";
    output<<"goto "<<yes.str()<<";\n";
}

// @ReadStmt
ReadStmt::ReadStmt(string n)
    :name(n) {}

void ReadStmt::genCode(Label& next) {
    cout<<"Generating code: ReadStmt\n";
    output<<"read("<<name<<");\n";
}

// @WriteStmt
WriteStmt::WriteStmt(ExprNode* e)
    :expr(e) {}

WriteStmt::~WriteStmt() {
    delete expr;
}

void WriteStmt::genCode(Label& next) {
    cout<<"Generating code: WriteStmt\n";
    Address* result = expr->genCode();
    output<<"write("<<result->str()<<");\n";
}

// @PrintStmt
PrintStmt::PrintStmt(string v)
    : val(v) {}

void PrintStmt::genCode(Label& next) {
    cout<<"Generating code: PrintStmt\n";
    output<<"print("<<val<<");\n";
}

