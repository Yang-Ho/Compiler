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
} TokenType;

// Token
typedef struct {
    TokenType type;
    string value;
} Token;

#endif /* TOKEN_H */
