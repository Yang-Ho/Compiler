#include "ASTnode.h"
#include "label.h"
#include "symbol.h"

#include <cstdlib>
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

string ComparisonExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: ComparisonExpr\n";
    string resultl = left->genCode(st);
    string resultr = right->genCode(st);

    string result = st->GetAddress();
    output<<result<<"="<<resultl<<op<<resultr<<";\n";
    return result;
}

// @ConditionExpr
ConditionExpr::ConditionExpr(string o, ExprNode*l, ExprNode*r)
    :op(o), left(l), right(r) {}

ConditionExpr::~ConditionExpr() {
    delete left;
    delete right;
}

string ConditionExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: ConditionExpr\n";
    string resultl = left->genCode(st);
    string resultr = right->genCode(st);

    string result = st->GetAddress();
    output<<result<<"="<<resultl<<op<<resultr<<";\n";
    return result;
}

// @BinaryExpr
BinaryExpr::BinaryExpr(string o, ExprNode *l, ExprNode *r)
    :op(o), left(l), right(r) {}

BinaryExpr::~BinaryExpr() {
    delete left;
    delete right;
}

string BinaryExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: BinaryExpr\n";
    string resultl = left->genCode(st);
    string resultr = right->genCode(st);

    string result = st->GetAddress();
    output<<result<<"="<<resultl<<op<<resultr<<";\n";
    return result;
}

// @UnaryExpr
UnaryExpr::UnaryExpr(string o, ExprNode *c)
    :op(o), child(c) {}

UnaryExpr::~UnaryExpr() {
    delete child;
}

string UnaryExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: UnaryExpr\n";
    string resultc = child->genCode(st);
    string result = st->GetAddress();

    output<<result<<"="<<op<<resultc<<";\n";
    return result;
}

// @ParenExpr
ParenExpr::ParenExpr(ExprNode *c)
    :child(c) {}

ParenExpr::~ParenExpr() {
    delete child;
}

string ParenExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: ParenExpr\n";
    string resultc = child->genCode(st);
    string result = st->GetAddress();

    output<<result<<"="<<"("<<resultc<<");\n";
    return result;
}

// @VariableExpr
VariableExpr::VariableExpr(string n)
    :name(n) {}

string VariableExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: VariableExpr\n";
    if (st->LookUp(name)) {
        string result = st->GetAddress(name);
        return result;
    } else {
        Symbol* sym = new Symbol(name, SYMBOL_LOCAL);
        st->Insert(sym);
        return "";
    }
}

// @NumberExpr
NumberExpr::NumberExpr(string v)
    :val(v) {}

string NumberExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: NumberExpr\n";
    //string result = st->GetAddress();

    //output<<result<<"="<<val<<";\n";
    string result = val;
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

string FuncCallExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: FuncCallExpr\n";
    string result = st->GetAddress();

    if (args) {
        vector<ExprNode*>::iterator it;
        vector<string> results;
        for (it = args->begin(); it != args->end(); ++it) {
            if (*it) {
                string r = (*it)->genCode(st);
                results.push_back(r);
            }
        }
        output<<result<<"="<<name<<"(";
        if (results.size() != 0) {
            output<<results[0];
            for (int i = 1; i < results.size(); i++) {
                output<<", "<<results[i];
            }
        }
        output<<");\n";
    } else {
        output<<result<<"="<<name<<"();\n";
    }
    return result;
}

// @ArrayRefExpr
ArrayRefExpr::ArrayRefExpr(string n, ExprNode *i)
    :name(n), index(i) {}

ArrayRefExpr::~ArrayRefExpr() {
    delete index;
}

string ArrayRefExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: ArrayRefExpr\n";
    string resulti = index->genCode(st);
    if (st->LookUp(name)) {
        string result = st->GetAddress(name, resulti);
        //output<<result;
        return result;
    } else {
        int offset = atoi(resulti.c_str());
        Symbol* sym = new Symbol(name, SYMBOL_LOCAL, offset);
        st->Insert(sym);
        return "";
    }
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

string ParamListExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: ParamListExpr\n";
    vector<ExprNode*>::iterator pit;
    for (pit = params->begin(); pit != params->end(); pit++) {
        (*pit)->genCode(st);
    }
    return "";
}

// @ParamExpr
ParamExpr::ParamExpr(string t, string n)
    :type(t), name(n) {}

string ParamExpr::genCode(SymbolTable* st) {
    cout<<"Generating code: ParamExpr\n";
    string result = "";
    if (name != "") {
        Symbol * sym = new Symbol(name, SYMBOL_PARAM);
        st->Insert(sym);
        result = st->GetAddress(name);
        output<<type<<" "<<name;
    } else {
        output<<type;
    }
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

void ProgramStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: ProgramStmt\n";
    vector<StmtNode*>::iterator ddit;
    vector<StmtNode*>::iterator fdit;
    st->EnterScope();
    if (data_decls) {
        for (ddit = data_decls->begin(); ddit != data_decls->end(); ++ddit) {
            (*ddit)->genCode(next, st);
        }
        string data_decls_type = "int";
        output<<data_decls_type<<" global["<<st->GetGlobalCount()<<"];\n";
    }
    for (fdit = func_decls->begin(); fdit != func_decls->end(); ++fdit) {
        (*fdit)->genCode(next, st);
    }
    st->ExitScope();
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

void FuncDeclStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: FuncDeclStmt\n";
    output<<type<<" "<<name<<"(";
    if (params) {
        params->genCode(st);
    }
    output<<")";
    if (data_decls) {
        st->ResetTemp();
        st->EnterScope();
        output<<"{\n";
        long pos = output.tellp();
        output<<"                              \n";
        vector<StmtNode*>::iterator ddit;
        for (ddit = data_decls->begin(); ddit != data_decls->end(); ddit++) {
            if (*ddit)
                (*ddit)->genCode(next, st);
        }
        if (body) {
            vector<StmtNode*>::iterator bit;
            for (bit = body->begin(); bit != body->end(); bit++) {
                if (*bit)
                    (*bit)->genCode(next, st);
            }
        }
        string data_decls_type = "int";
        long endpos = output.tellp();
        output.seekp(pos);
        output<<data_decls_type<<" local["<<st->GetTempCount()<<"];";
        output.seekp(endpos);
        output<<"}";
        st->ExitScope();
    } else {
        output<<";";
    }
    output<<"\n";
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

void DataDeclsStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: DataDeclsStmt\n";
    if (ids) {// To do, figure out how many local vars are in a function
        vector<ExprNode*>::iterator it;
        for (it = ids->begin(); it != ids->end(); it++) {
            if (*it) {
                (*it)->genCode(st);
            }
        }
        //output<<type<<" ["<<ids->size()<<"];\n";
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

void BlockStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: BlockStmt\n";
    if (statements) {
        vector<StmtNode*>::iterator it;
        for (it = statements->begin(); it != statements->end(); ++it) {
            (*it)->genCode(next, st);
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

void FuncCallStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: FuncCallStmt\n";
    if (args) {
        vector<ExprNode*>::iterator it;
        vector<string> results;
        for (it = args->begin(); it != args->end(); ++it) {
            if ((*it)) {
                string r = (*it)->genCode(st);
                results.push_back(r);
            }
        }
        output<<name<<"(";
        if (results.size() != 0) {
            output<<results[0];
            for (int i = 1; i < results.size(); i++) {
                output<<", "<<results[i];
            }
        }
        output<<");\n";
    } else {
        output<<name<<"();\n";
    }
}
// @AssignmentStmt
AssignmentStmt::AssignmentStmt(ExprNode* l, ExprNode* r)
    :lhs(l), rhs(r) {}

AssignmentStmt::~AssignmentStmt() {
    delete lhs;
    delete rhs;
}

void AssignmentStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: AssignmentStmt\n";
    string resultl = lhs->genCode(st);
    string resultr = rhs->genCode(st);

    output<<resultl<<"="<<resultr<<";\n";
}
// @IfStmt
IfStmt::IfStmt(ExprNode *c, StmtNode *b)
    :condition(c), body(b) {}

IfStmt::~IfStmt() {
    delete condition;
    delete body;
}

void IfStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: IfStmt\n";
    Label trueLabel;
    Label falseLabel;
    string result = condition->genCode(st);
    output<<"if ("<<result<<") goto "<<trueLabel.str()<<";\n";  
    output<<"goto "<<falseLabel.str()<<";\n";
    output<<trueLabel.str()<<":;\n";
    body->genCode(falseLabel, st);
    output<<falseLabel.str()<<":;\n";
}
// @WhileStmt
WhileStmt::WhileStmt(ExprNode *c, StmtNode *b)
    :condition(c), body(b) {}

WhileStmt::~WhileStmt() {
    delete condition;
    delete body;
}

void WhileStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: WhileStmt\n";
    Label trueLabel;
    Label falseLabel;
    st->EnterWhile(next, falseLabel);
    string result = condition->genCode(st);
    output<<next.str()<<":;\n";
    output<<"if ("<<result<<") goto "<<trueLabel.str()<<";\n";
    output<<"goto "<<falseLabel.str()<<";\n";
    output<<trueLabel.str()<<":;\n";
    //body->genCode(trueLabel, falseLabel);
    body->genCode(falseLabel, st);
    output<<"goto "<<next.str()<<";\n";
    output<<falseLabel.str()<<":;\n";
    st->ExitWhile();
}

// @ReturnStmt
ReturnStmt::ReturnStmt(ExprNode* e)
    :expr(e) {}

ReturnStmt::~ReturnStmt() {
    delete expr;
}

void ReturnStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: ReturnStmt\n";
    if (expr) {
        string result = expr->genCode(st);
        output<<"return "<<result<<";\n";
    } else {
        output<<"return ;\n";
    }
}

// @BreakStmt
void BreakStmt::genCode(Label& no, SymbolTable* st) {
    cout<<"Generating code: BreakStmt\n";
    output<<"goto "<<st->GetFalseLabel().str()<<";\n";
}

// @ContinueStmt
void ContinueStmt::genCode(Label& yes, SymbolTable* st) {
    cout<<"Generating code: ContinueStmt\n";
    output<<"goto "<<st->GetTrueLabel().str()<<";\n";
}

// @ReadStmt
ReadStmt::ReadStmt(string n)
    :name(n) {}

void ReadStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: ReadStmt\n";
    if (st->LookUp(name)) {
    } else {
        Symbol* sym = new Symbol(name, SYMBOL_LOCAL);
        st->Insert(sym);
    }
    string result = st->GetAddress(name);
    output<<"read("<<result<<");\n";
}

// @WriteStmt
WriteStmt::WriteStmt(ExprNode* e)
    :expr(e) {}

WriteStmt::~WriteStmt() {
    delete expr;
}

void WriteStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: WriteStmt\n";
    string result = expr->genCode(st);
    output<<"write("<<result<<");\n";
}

// @PrintStmt
PrintStmt::PrintStmt(string v)
    : val(v) {}

void PrintStmt::genCode(Label& next, SymbolTable* st) {
    cout<<"Generating code: PrintStmt\n";
    output<<"print("<<val<<");\n";
}

