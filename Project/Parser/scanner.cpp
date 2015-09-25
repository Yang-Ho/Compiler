#include "scanner.h"
#include "token.h"

using namespace::std;

// Function to check if a char is of a specific type
bool IsOfType( char c, string toSearch ) {
    size_t location = toSearch.find(c);
    return (location != string::npos);
}

Scanner::Scanner(string file) {
    input.open(file.c_str());
};

Scanner::~Scanner() {
    input.close();
}

// Scanner
Token Scanner::GetNextToken() {
    // Initialize variables
    string value = "";

    char next;
    int state = 0;
    int previousState = 0;
    bool nextWhiteSpace = false;
    bool valid = true;

    while (state > -1) {
        previousState = state;
        // Check if input is at eof
        if (!input.eof()) {
            next = input.peek();
            //input.get(next);
            if (next == EOF) {
                break;
            }
        } else {
            break;
        }

        // Bool that handles if input is a white space
        nextWhiteSpace = false;

        // DFA iteration
        switch(state) {
            case 0: // Start state
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
                    //input.get(next);
                    nextWhiteSpace = true;
                } else {
                    valid = false;
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
                    valid = false;
                    state = -2;
                } else {
                    state = 11;
                }
                break;
            case 6: // SYMBOL token &&
                if (next != '&') {
                    valid = false;
                    state = -2;
                } else {
                    state = 11;
                }
                break;
            case 7: // SYMBOL token ||
                if (next != '|') {
                    valid = false;
                    state = -2;
                } else {
                    state = 11;
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
                    state = 11;
                }
                break;
            case 10: // STRING token
                if (next == '"') {
                    state = 11;
                } else if (next == '\n' || next == EOF) {
                    valid = false;
                    state = -2;
                }
                break;
            case 11:
                state = -1;
                break;
        }
        
        // For some tokens, you need to consume input or ignore it
        if (!nextWhiteSpace) {
            if (valid && state > -1) {
                input.get(next);
                value += next;
            }
        } else {
            input.get(next);
        }

        // Handle error state
        if (state == -2) {
            Token error;
            error.type = ERROR;
            error.value = value + next;
            return error;
        } else if (state == 11) {
            state = -1;
        }
    }

    // Determine token type and return token
    Token token;
    switch(previousState) {
        case 1: // Check if ID matches a reserved word
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

// Check if input has more input
bool Scanner::HasMoreTokens() {
    if (input.eof()) {
        return false;
    } else {
        return true;
    }
}
