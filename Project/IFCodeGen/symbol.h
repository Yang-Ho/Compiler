#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <map>
#include <string>

using namespace::std;

class SymbolTable {
    private:
        int count;
        map<string, int> symbols;
    public:
        SymbolTable();
        ~SymbolTable();

        int LookUp(string name);
        void Insert(string name);

        void OpenScope();
        void CloseScope();

        int CurrentScope();
};

#endif /* SYMBOL_H */
