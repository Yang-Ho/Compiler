/*
 * Yang Ho
 * CSC 512
 * token.cpp
 *
 * Implementation file for token.h 
 */
#include "token.h"

Token::Token(string new_value, int new_id) {
    value = new_value;
    id = new_id;
}

string Token::GetTokenValue() {
    return value;
}

int Token::GetTokenType() {
    return id;
}
