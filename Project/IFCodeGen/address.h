#ifndef ADDRESS_H
#define ADDRESS_H 

#include <string>
#include <sstream>

using namespace::std;

class Address {
    public:
        virtual string str()= 0;
};

class VarAddress: public Address {
    private:
        string name;
    public:
        VarAddress(string n);
        virtual string str();
};

class TempAddress: public Address {
    private:
        static int next;
        const int value;
    public:
        TempAddress();
        virtual string str();
        static void reset();
};
#endif /* ADDRESS_H */
