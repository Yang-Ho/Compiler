#include "symbol.h"

#include <vector>
#include <map>
#include <string>

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

SymbolTable::SymbolTable(SymbolTable * p) {
    symbols = map<string, Symbol*>();
    parent = p;
}

SymbolTable::~SymbolTable() {
    //delete scopes;
}

void SymbolTable::Insert(Symbol *sym) {
    symbols[sym->GetName()] = sym;
}

Symbol* SymbolTable::LookUp(string name) {
    SymbolTable* curr_table = this;
    while(curr_table != 0) {
        map<string, Symbol*>::iterator it = curr_table->symbols.find(name);
        if (it != curr_table->symbols.end()) {
            return it->second;
        }
        curr_table = curr_table->parent;
    }
    return 0;
}
