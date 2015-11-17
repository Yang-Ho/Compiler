/*
 * Yang Ho
 * CSC 512
 * ASTnode.cpp
 *
 * Implementation file for ASTnode.h 
 */
#include "symbol.h"

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

using namespace::std;

// @Symbol
Symbol::Symbol(string n, SymbolType t, int o)
    :name(n), type(t), offset(o) {
    loc = -1;
}

string Symbol::GetName() {
    return name;
}

SymbolType Symbol::GetType() {
    return type;
}

int Symbol::GetLoc() {
    return loc;
}

int Symbol::GetOffset() {
    return offset;
}

void Symbol::SetLoc(int l) {
    loc = l;
}

void Symbol::SetType(SymbolType t) {
    type = t;
}

// @SymbolTable
int SymbolTable::next_temp = 0;

SymbolTable::SymbolTable() {
    next_global = 0;
    symbols = vector<map<string, Symbol*> >();
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
    // Should add a check if symbol already is in the table
    if (symbols.size() == 1 && sym->GetType() == SYMBOL_LOCAL) {  // Check if global variable
        sym->SetType(SYMBOL_GLOBAL);
        sym->SetLoc(next_global);
        next_global++;
        next_global += sym->GetOffset();                //Compute offset next address
    } else if (sym->GetType() == SYMBOL_LOCAL) {
        sym->SetLoc(next_temp);
        next_temp++;
        next_temp += sym->GetOffset();                  // Compute offset next address
    }
    symbols.back()[sym->GetName()] = sym;
}

Symbol* SymbolTable::LookUp(string name) {
    vector<map<string, Symbol*> >::reverse_iterator vit;
    // Search for name starting from the most recent scope
    for (vit = symbols.rbegin(); vit != symbols.rend(); ++vit) {
        map<string, Symbol*>::iterator mit = vit->find(name);
        if (mit != vit->end()) {
            return mit->second;
        }
    }
    return NULL; // name not found
}

void SymbolTable::ResetTemp() {
    next_temp = 0;
}

string SymbolTable::GetAddress() {
    stringstream ss;
    ss<<"local["<<next_temp++<<"]";
    return ss.str();
}

string SymbolTable::GetAddress(string name, string off) {
    Symbol* sym = LookUp(name);
    if (sym) {
        if (sym->GetType() == SYMBOL_PARAM) {
            cout<<"SYMBOL PARAM: "<<name<<endl;
            if (sym->GetLoc() < 0) {
                cout<<"Setting new temp loc for param\n";
                sym->SetLoc(next_temp);
                return "Param";
            } else {
                cout<<"Getting temp loc for param\n";
                stringstream ss;
                if (off == "") {
                    ss<<"local["<<sym->GetLoc()<<"]";
                } else {
                    ss<<"local["<<sym->GetLoc()<<"+"<<off<<"]";
                }
                return ss.str();
            }
        } else if (sym->GetType() == SYMBOL_FUNC) {
            return sym->GetName();
        } else if (sym->GetType() == SYMBOL_GLOBAL) {
            stringstream ss;
            if (off == "") {
                ss<<"global["<<sym->GetLoc()<<"]";
            } else {
                ss<<"global["<<sym->GetLoc()<<"+"<<off<<"]";
            }
            return ss.str();
        } else {
            stringstream ss;
            if (off == "") {
                ss<<"local["<<sym->GetLoc()<<"]";
            } else {
                ss<<"local["<<sym->GetLoc()<<"+"<<off<<"]";
            }
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

int SymbolTable::GetTempCount() {
    return next_temp;
}

int SymbolTable::GetGlobalCount() {
    return next_global;
}

Label& SymbolTable::GetTrueLabel() {
    return while_scopes.back().first;
}

Label& SymbolTable::GetFalseLabel() {
    return while_scopes.back().second;
}
