#include "symbol.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

using namespace::std;

Symbol::Symbol(string n, SymbolType t, int l)
    :name(n), type(t), loc(l) {}

string Symbol::GetName() {
    return name;
}

SymbolType Symbol::GetType() {
    return type;
}

int Symbol::GetLoc() {
    return loc;
}

void Symbol::SetLoc(int l) {
    loc = l;
}

void Symbol::SetType(SymbolType t) {
    type = t;
}

SymbolTable::SymbolTable() {
    symbols = vector<map<string, Symbol*> >();
    next_temp = 0;
    curr_temp = next_temp++;
    while_scopes = vector<pair<Label, Label> >();
}

SymbolTable::~SymbolTable() {
    vector<map<string, Symbol*> >::iterator vit;
    for (vit = symbols.begin(); vit != symbols.end(); ++vit) {
        map<string, Symbol*>::iterator mit;
        for (mit = vit->begin(); mit != vit->end(); ++mit) {
            delete mit->second;
        }
    }
}

void SymbolTable::Insert(Symbol *sym) {
    sym->SetLoc(next_temp++);
    curr_temp = next_temp;
    if (symbols.size() == 1) {
        sym->SetType(SYMBOL_GLOBAL);
    }
    symbols.back()[sym->GetName()] = sym;
}

Symbol* SymbolTable::LookUp(string name) {
    vector<map<string, Symbol*> >::reverse_iterator vit;
    for (vit = symbols.rbegin(); vit != symbols.rend(); ++vit) {
        map<string, Symbol*>::iterator mit = vit->find(name);
        if (mit != vit->end()) {
            cout<<"Found it!\n";
            return mit->second;
        }
    }
    return NULL;
}

void SymbolTable::ResetTemp() {
    next_temp = 0;
}

string SymbolTable::GetAddress() {
    stringstream ss;
    ss<<"local["<<curr_temp<<"]";
    curr_temp = next_temp++;
    return ss.str();
}

string SymbolTable::GetAddress(string name) {
    cout<<"Getting address for: "<<name<<endl;
    Symbol* sym = LookUp(name);
    if (sym) {
        if (sym->GetType() == SYMBOL_PARAM) {
            return sym->GetName();
        } else if (sym->GetType() == SYMBOL_FUNC) {
            return sym->GetName();
        } else if (sym->GetType() == SYMBOL_GLOBAL) {
            stringstream ss;
            ss<<"global["<<sym->GetLoc()<<"]";
            return ss.str();
        } else {
            stringstream ss;
            ss<<"local["<<sym->GetLoc()<<"]";
            return ss.str();
        }
    } else {
        return "";
    }
}

void SymbolTable::EnterScope() {
    symbols.push_back(map<string, Symbol*>()); 
}

void SymbolTable::ExitScope() {
    map<string, Symbol*>::iterator mit;
    for (mit = symbols.back().begin(); mit != symbols.back().end(); ++mit) {
        delete mit->second;
    }
    symbols.pop_back();
}

void SymbolTable::EnterWhile(Label& true_label, Label& false_label) {
    while_scopes.push_back(make_pair(true_label, false_label));
}

void SymbolTable::ExitWhile() {
    while_scopes.pop_back();
}

Label& SymbolTable::GetTrueLabel() {
    return while_scopes.back().first;
}

Label& SymbolTable::GetFalseLabel() {
    return while_scopes.back().second;
}
