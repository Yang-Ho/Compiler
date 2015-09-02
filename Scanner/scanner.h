#ifndef SCANNER_H
#define SCANNER_H 

//#include "token.h"

#include <fstream>
#include <string>
#include <iostream>

using namespace::std;

static const string DIGITS("0123456789");
static const string LETTERS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
static const string SINGLESYMBOL("(){}[],;+-/*");
static const string BOOLLOGIC("&|=!><");

bool IsOfType( char c, string toSearch ) {
    size_t location = toSearch.find(c);
    return (location != string::npos);
}

typedef enum{
    ID = 1,
    NUMBER,
    RESERVED,
    SYMBOL,
    STRING,
    META 
} TokenType;

typedef struct {
    TokenType type;
    string value;
} Token;

class Scanner {
    private:
        ifstream input;
        string fileName;
    public:
        Scanner(string file) { input.open(file.c_str()); };
        bool HasMoreTokens();
        Token GetNextToken();
};

Token Scanner::GetNextToken() {
    string value;

    char next;
    int state = 0;
    int previousState = 0;

    while (state > -1) {
        previousState = state;
        if (!input.eof()) {
            next = input.peek();
            if (next == EOF) {
                break;
            }
        } else {
            break;
        }

        switch(state) {
            case 0:
                if (IsOfType(next, DIGITS)) {
                    state = 2;
                }
                break;
            case 1: // ID token 
                break;
            case 2: // NUMBER token
                if (!IsOfType(next, DIGITS)) {
                    state = -1;
                }
                break;
            case 3: // RESERVED token
                break;
            case 4: // SYMBOL token
                break;
            case 5: // STRING token
                break;
            case 6: // META token
                break;
        }
        if (state > -1) {
            value += next;
        }
        input.get(next);
    }

    Token token;
    switch(previousState) {
        case 1:
            token.type = ID;
            token.value = value;
            break;
        case 2:
            token.type = NUMBER;
            token.value = value;
            break;
        case 3:
            token.type = RESERVED;
            token.value = value;
            break;
        case 4:
            token.type = SYMBOL;
            token.value = value;
            break;
        case 5:
            token.type = STRING;
            token.value = value;
            break;
        case 6:
            token.type = META;
            token.value = value;
            break;
    }
    return token;
}

bool Scanner::HasMoreTokens() {
    if (input.eof()) {
        return false;
    } else {
        return true;
    }
}
#endif /* SCANNER_H */
