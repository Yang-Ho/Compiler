#ifndef SYMBOL_H
#define SYMBOL_H

#include "label.h"

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
        void SetLoc(int l);
        void SetType(SymbolType t);
};

class SymbolTable {
    private:
        vector<map<string, Symbol*> > symbols;

        vector<pair<Label, Label> > while_scopes;

        int next_temp;
        int curr_temp;
    public:
        SymbolTable();
        ~SymbolTable();

        Symbol* LookUp(string name);
        void Insert(Symbol *sym);

        void ResetTemp();

        string GetAddress();
        string GetAddress(string name);

        void EnterScope();
        void ExitScope();

        void EnterWhile(Label& t, Label&f);
        void ExitWhile();

        Label& GetTrueLabel();
        Label& GetFalseLabel();
};

#endif /* SYMBOL_H */
