#include "symbol.h"

#include <vector>
#include <map>
#include <string>

using namespace::std;

SymbolTable::SymbolTable() {
    symbols = map<string, int>();
    count = 0;
}

SymbolTable::~SymbolTable() {
    //delete scopes;
}

void SymbolTable::Insert(string name) {
    symbols[name] = count++;
}

int SymbolTable::LookUp(string name) {
    if (symbols.find(name) != symbols.end()) {
        return symbols[name];
    } else {
        return -1;
    }
}
