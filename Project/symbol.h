/*
 * Yang Ho
 * CSC 512
 * symbol.cpp
 *
 * Header file contain my implementation of a symbol table and symbols
 */
#ifndef SYMBOL_H
#define SYMBOL_H

#include "label.h"

#include <vector>
#include <map>
#include <string>

using namespace::std;

typedef enum{
    SYMBOL_LOCAL = 1,   // local variable
    SYMBOL_GLOBAL,      // global variable
    SYMBOL_PARAM,       // func parameter
    SYMBOL_FUNC,        // func
} SymbolType;

class Symbol {
    private:
        string name;    // ID of symbol
        SymbolType type;
        int loc;        // Address of symbol
        int offset;     // Offset to of additional memory needed (for arrays)
    public:
        Symbol(string n, SymbolType t, int o=0);
        
        string GetName();
        SymbolType GetType();
        int GetLoc();
        int GetOffset();
        void SetLoc(int l);
        void SetType(SymbolType t);
};

class SymbolTable {
    private:
        vector<map<string, Symbol*> > symbols;      // Symbol table, stack of hash tables(scopes)

        vector<pair<Label, Label> > while_scopes;   // stack of while scopes to handle breaks and continues

        int next_global;                            // the next global variable address
        static int next_temp;                       // The next local variable address
    public:
        SymbolTable();
        ~SymbolTable();

        Symbol* LookUp(string name);                // Search for name
        void Insert(Symbol *sym);                   // Insert symbol

        void ResetTemp();                           // Reset temp count (when entering new function scope

        string GetAddress();                            // Return next temp address
        string GetAddress(string name, string off="");  // return address of variable/func/etc

        int GetTempCount();                             // Return number of local variables in the current scope
        int GetGlobalCount();                           // Return number of global variables

        void EnterScope();                          // Enter a new scope
        void ExitScope();                           // Exit the last scope

        void EnterWhile(Label& t, Label&f);         // Enter a new while scope
        void ExitWhile();                           // Exit a while scope

        Label& GetTrueLabel();                      // Get the current while scope's truelabel
        Label& GetFalseLabel();                     // Get the current while scope's falselabel
};

#endif /* SYMBOL_H */
