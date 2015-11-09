#include "symbol.h"

#include <vector>
#include <map>
#include <string>

using namespace::std;

SymbolTable::SymbolTable() {
    scopes = vector<map<string, string> >();
    scopes.push_back(map<string, string>());
}

SymbolTable::~SymbolTable() {
    //delete scopes;
}

void SymbolTable::Insert(string name, string record) {
    scopes.back()[name] = record;
}

string SymbolTable::LookUp(string name) {
    if (scopes.back().find(name) != scopes.back().end()) {
        return scopes.back()[name];
    } else {
        return NULL;
    }
}

void SymbolTable::InitizializeScope() {
    scopes.push_back(map<string, string>());
}

void SymbolTable::FinalizeScope() {
    scopes.pop_back();
}
