#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>
#include <map>
#include <string>

using namespace::std;

class SymbolTable {
    private:
        vector<map<string, string> > scopes;
    public:
        SymbolTable();
        ~SymbolTable();

        string LookUp(string name);
        void Insert(string name, string record);
        void InitizializeScope();
        void FinalizeScope();
};

#endif /* SYMBOL_H */
