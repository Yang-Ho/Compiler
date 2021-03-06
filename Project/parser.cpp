/*
 * Yang Ho
 * CSC 512
 * ASTnode.cpp
 *
 * Implementation file for ASTnode.h 
 */
#include "parser.h"
#include "token.h"
#include "symbol.h"
#include "ASTnode.h"

#include <vector>
#include <iostream>

using namespace::std;

Parser::Parser(Grammar *new_grammar) {
    grammar = new_grammar;
    num_variables = 0;
    num_functions = 0;
    num_statements = 0;
    tokens = new vector<Token *>();
    grammar->LoadParser(this);
}

Parser::~Parser() {
    for (vector<Token *>::iterator it = tokens->begin(); it != tokens->end(); ++it) {
        delete *it;
    }
    delete tokens;

}

// Consume the current token and get the next one
Token* Parser::Consume(TokenType t_type) {
    //cout<<"Consuming: "<<(*tokenIT)->GetTokenValue()<<"\n";
    ++tokenIT;
    while (tokenIT != tokens->end() && (*tokenIT)->GetTokenType() == TOKEN_META) {
        if ((*tokenIT)->GetTokenType() == TOKEN_META) {
            //generator->WriteToOutput((*tokenIT)->GetTokenValue());
            output<<(*tokenIT)->GetTokenValue();
        }
        ++tokenIT;
    }

    if (tokenIT == tokens->end()) {
        return NULL;
    }
    return *tokenIT;
}

// Return the current token
Token* Parser::GetCurrToken() {
    if (tokenIT == tokens->end()) {
        return NULL;
    }

    return *tokenIT;
}

string Parser::GetCurrTokenValue() {
    return (*tokenIT)->GetTokenValue();
}

int Parser::GetCurrTokenType() {
    return (*tokenIT)->GetTokenType();
}

void Parser::AddToken(Token *new_token) {
    tokens->push_back(new_token);
}

// Parse the grammar
StmtNode *Parser::Parse() {
    tokenIT = tokens->begin();
    while (tokenIT != tokens->end() && (*tokenIT)->GetTokenType() == TOKEN_META) {
        if ((*tokenIT)->GetTokenType() == TOKEN_META) {
            //generator->WriteToOutput((*tokenIT)->GetTokenValue());
            output<<(*tokenIT)->GetTokenValue();
        }
        ++tokenIT;
    }
    if (tokenIT != tokens->end()) {
        StmtNode * result = grammar->Program();
        if (tokenIT == tokens->end()-1) {
            return result;
        } else {
            return  NULL;
        }
    }
    return NULL;
}


int Parser::GetNumVariables() {
    return num_variables;
}

int Parser::GetNumFunctions() {
    return num_functions;
}

int Parser::GetNumStatements() {
    return num_statements;
}

void Parser::IncNumVariables() {
    num_variables++;
}

void Parser::IncNumFunctions() {
    num_functions++;
}

void Parser::IncNumStatements() {
    num_statements++;
}

void Parser::DecNumVariables() {
    num_variables--;
}

void Parser::DecNumFunctions() {
    num_functions--;
}

void Parser::DecNumStatements() {
    num_statements--;
}
