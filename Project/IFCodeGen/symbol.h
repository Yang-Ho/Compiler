#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <map>
#include <string>

using namespace::std;

typedef enum{
    SYMBOL_LOCAL = 1,
    SYMBOL_GLOBAL,
    SYMBOL_PARAM,
    SYMBOL_FUNC
} SymbolType;

class Symbol {
    private:
        string name;
        SymbolType type;
        int loc;
    public:
        Symbol(string n, SymbolType t, int l);
        
        string GetName();
        SymbolType GetType();
        int GetLoc();
};

class SymbolTable {
    private:
        map<string, Symbol*> symbols;
        SymbolTable * parent;
    public:
        SymbolTable(SymbolTable* p = 0);
        ~SymbolTable();

        Symbol* LookUp(string name);
        void Insert(Symbol *sym);
};

#endif /* SYMBOL_H */
