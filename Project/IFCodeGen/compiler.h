#ifndef COMPILER_H
#define COMPILER_H 

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

bool IsOfType( char c, string toSearch );

class Compiler{
    private:
        ifstream input;
        ofstream output;
        string inputFile;
        string outputFile;
    public:
        Compiler(string);
        ~Compiler();
        // Scanner section
        bool HasMoreTokens();
        Token GetNextToken();
};

#endif /* COMPILER_H */
