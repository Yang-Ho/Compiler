/*
 * Yang Ho
 * CSC 512
 * label.cpp
 *
 * Implementation file for label.h 
 */
#include "label.h"

#include <sstream>
#include <string>
#include <iostream>

using namespace::std;

int Label::next = 0;

Label::Label()
    : value(next++) {}

string Label::str() {
    stringstream ss;
    ss<<"L"<<value;
    return ss.str();
}
