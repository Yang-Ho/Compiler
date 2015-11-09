#ifndef TOKEN_H
#define TOKEN_H 
#include <string>

using namespace::std;

// Token types
typedef enum{
    TOKEN_ID = 1,
    TOKEN_NUMBER,
    TOKEN_RESERVED,
    TOKEN_SYMBOL,
    TOKEN_STRING,
    TOKEN_META,
    TOKEN_ERROR,
    TOKEN_WHITESPACE
} TokenType;

// Token
class Token {
    private:
        string value;
        int id;
    public:
        Token();
        Token(string new_value, int new_id);
        string GetTokenValue();
        int GetTokenType();
        void SetTokenId(int new_id);
        void SetValueString(string new_value);
};
#endif /* TOKEN_H */
