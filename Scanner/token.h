#ifndef TOKEN_H
#define TOKEN_H 

#include <string>

using namespace::std;

enum TokenType {
    ID = 1,
    NUMBER,
    RESERVED,
    SYMBOL,
    STRING,
    META 
};

class Token {
    private:
        TokenType type;
        string value;
    public:
        Token();
        Token(TokenType t, string val) {
            type = t;
            value = val;
        }
        TokenType const GetTokenType() { return type; }
        string GetTokenValue() { return value; }
};
#endif /* TOKEN_H */
