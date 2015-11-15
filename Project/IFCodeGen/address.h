#ifndef ADDRESS_H
#define ADDRESS_H 

#include <string>
#include <sstream>

using namespace::std;

typedef enum {
    VAR_PARAM = 1,
    VAR_LOCAL,
    VAR_GLOBAL,
    VAR_FUNC
} VarType;

class Address {
    public:
        virtual string str()= 0;
};

class VarAddress: public Address {
    private:
        string name;
        VarType type;
    public:
        VarAddress(string n, VarType t = VAR_GLOBAL);
        virtual string str();
};

class TempAddress: public Address {
    private:
        static int next;
        const int value;
    public:
        TempAddress(bool r=false);
        virtual string str();
        static void reset() {next=-1;};
};
#endif /* ADDRESS_H */
