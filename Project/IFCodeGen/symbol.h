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
};

#endif /* SYMBOL_H */
