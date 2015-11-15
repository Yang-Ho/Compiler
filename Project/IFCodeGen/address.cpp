#include "address.h"

#include <string>
#include <sstream>

using namespace::std;

VarAddress::VarAddress(string n, VarType t)
    : name(n), type(t) {}

string VarAddress::str() {
    if (type == VAR_PARAM) {
        return name;
    } else {
        return name;
    }
}

int TempAddress::next = -1;

TempAddress::TempAddress(bool r)
    : value(next++){
    if (r) {
        next = 0;
    }
}

string TempAddress::str() {
    stringstream ss;
    ss<<"local["<<value<<"]";
    return ss.str();
}
