#ifndef SCANNER_H
#define SCANNER_H 

#include "token.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace::std;

// Symbols recognized by scanner
static const string DIGITS("0123456789");
static const string LETTERS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
static const string SINGLESYMBOL("){(}[],;+-*"); // might break this up into operators, and others
static const string COMPARISON("<>=");
static const string WHITESPACE(" \n\t");
static const string RESERVED_WORDS[12] = {"int","void","if","while","return","read","write","print","continue","break","binary","decimal"};
// Other symbols: !, &, |, /, #, "

// Function to check if a char is of a specific type
bool IsOfType( char c, string toSearch );

// Scanner
class Scanner {
    private:
        ifstream input; // input stream 
        string fileName; // input file name
    public:
        Scanner(string file);
        ~Scanner();
        bool HasMoreTokens();
        Token GetNextToken();
};

#endif /* SCANNER_H */
