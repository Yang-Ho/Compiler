#ifndef TOKEN_H
#define TOKEN_H 
#include <string>

using namespace::std;
// Token types
typedef enum{
    ID = 1,
    NUMBER,
    RESERVED,
    SYMBOL,
    STRING,
    META,
    ERROR,
} TokenType;

// Token
typedef struct {
    TokenType type;
    string value;
} Token;

#endif /* TOKEN_H */
