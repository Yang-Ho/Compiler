#ifndef SCANNER_H
#define SCANNER_H 

//#include "token.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace::std;

static const string DIGITS("0123456789");
static const string LETTERS("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
static const string SINGLESYMBOL("){(}[],;+-*");
static const string COMPARISON("<>=");
static const string WHITESPACE(" \n\t");
static const string RESERVED_WORDS[12] = {"int","void","if","while","return","read","write","print","continue","break","binary","decimal"};

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
    META,
    ERROR
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
    string value = "";

    char next;
    int state = 0;
    int previousState = 0;
    bool nextWhiteSpace = false;

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

        nextWhiteSpace = false;

        switch(state) {
            case 0:
                if (IsOfType(next, LETTERS)) {
                    state = 1;
                }
                else if (IsOfType(next, DIGITS)) {
                    state = 2;
                } 
                else if (IsOfType(next, SINGLESYMBOL)) {
                    state = 3;
                } 
                else if (IsOfType(next, COMPARISON)) {
                    state = 4;
                } 
                else if (next == '!') {
                    state = 5;
                } 
                else if (next == '&') {
                    state = 6;
                } 
                else if (next == '|') {
                    state = 7;
                } 
                else if (next == '/') {
                    state = 8;
                } 
                else if (next == '#') {
                    state = 9;
                } 
                else if (next == '"') {
                    state = 10;
                } 
                else if (IsOfType(next, WHITESPACE)) {
                    input.get(next);
                    nextWhiteSpace = true;
                } else {
                    state = -2;
                }
                break;
            case 1: // ID token OR RESERVED token
                if (!IsOfType(next, LETTERS) && !IsOfType(next, DIGITS)) {
                    state = -1;
                } 
                break;
            case 2: // NUMBER token
                if (!IsOfType(next, DIGITS)) {
                    state = -1;
                }
                break;
            case 3: // SYMBOL token
                state = -1;
                break;
            case 4: // SYMBOL tokens >=, ==, <=, =, >, <
                if (next != '=') {
                    state = -1;
                }
                break;
            case 5: // SYMBOL token !=
                if (next != '=') {
                    state = -2;
                } else {
                    state = -1;
                }
                break;
            case 6: // SYMBOL token &&
                if (next != '&') {
                    state = -2;
                } else {
                    state = -1;
                }
                break;
            case 7: // SYMBOL token ||
                if (next != '|') {
                    state = -2;
                } else {
                    state = -1;
                }
                break;
            case 8: // SYMBOL token /
                if (next == '/') {
                    state = 9;
                } else {
                    state = -1;
                }
                break;
            case 9: // META token
                if (next == '\n') {
                    state = -1;
                }
                break;
            case 10: // STRING token
                if (next == '"') {
                    state = -1;
                } else if (next == '\n' || next == EOF) {
                    state = -2;
                }
                break;
        }
        if (!nextWhiteSpace) {
            if (state > -1) {
                input.get(next);
                value += next;
            } else if (previousState == 5) {
                input.get(next);
                value += next;
            } else if (previousState == 6) {
                input.get(next);
                value += next;
            } else if (previousState == 7) {
                input.get(next);
                value += next;
            } else if (previousState == 9) {
                input.get(next);
                value += next;
            } else if (previousState == 10) {
                input.get(next);
                value += next;
            }
        }

        if (state == -2) {
            Token error;
            error.type = ERROR;
            error.value = value;
            return error;
        }
    }

    Token token;
    switch(previousState) {
        case 1:
            if (find(RESERVED_WORDS, RESERVED_WORDS+12, value) != RESERVED_WORDS+12) {
                token.type = RESERVED;
            } else {
                token.type = ID;
            }
            token.value = value;
            break;
        case 2:
            token.type = NUMBER;
            token.value = value;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            token.type = SYMBOL;
            token.value = value;
            break;
        case 9:
            token.type = META;
            token.value = value;
            break;
        case 10:
            token.type = STRING;
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
