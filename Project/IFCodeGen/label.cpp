#include "label.h"

#include <sstream>
#include <string>

using namespace::std;

int Label::next = 0;

Label::Label()
    : value(next++) {}

string Label::str() {
    stringstream ss;
    ss<<"L"<<value;
    return ss.str();
}
