#include "address.h"

#include <string>
#include <sstream>

using namespace::std;

VarAddress::VarAddress(string n)
    : name(n) {}

string VarAddress::str() {
    return name;
}

int TempAddress::next = 0;

TempAddress::TempAddress()
    :value(next++) {}

string TempAddress::str() {
    stringstream ss;
    ss<<"local["<<value<<"]";
    return ss.str();
}
