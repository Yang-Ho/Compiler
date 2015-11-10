/* 
 * My recursive descent parser
 */
#include "parser.h"
#include "token.h"
#include "symbol.h"
#include "ASTnode.h"

#include <vector>
#include <iostream>

using namespace::std;

Parser::Parser(Grammar *new_grammar, string fileName) {
    grammar = new_grammar;
    num_variables = 0;
    num_functions = 0;
    num_statements = 0;
    tokens = new vector<Token *>();
    grammar->LoadParser(this);

    output_file = fileName.substr(0, fileName.rfind('.'));
    output_file += "_gen.c";
    output.open(output_file.c_str());
}

Parser::~Parser() {
    for (vector<Token *>::iterator it = tokens->begin(); it != tokens->end(); ++it) {
        delete *it;
    }
    delete tokens;

    output.close();
}

Token* Parser::Consume(TokenType t_type) {
    //cout<<"Consuming: "<<(*tokenIT)->GetTokenValue()<<"\n";
    ++tokenIT;
    while (tokenIT != tokens->end() && (*tokenIT)->GetTokenType() == TOKEN_META) {
        if ((*tokenIT)->GetTokenType() == TOKEN_META) {
            output<<(*tokenIT)->GetTokenValue();
        }
        ++tokenIT;
    }

    if (tokenIT == tokens->end()) {
        return NULL;
    }
    return *tokenIT;
}

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

bool Parser::Parse() {
    tokenIT = tokens->begin();
    while (tokenIT != tokens->end() && (*tokenIT)->GetTokenType() == TOKEN_META) {
        if ((*tokenIT)->GetTokenType() == TOKEN_META) {
            output<<(*tokenIT)->GetTokenValue();
        }
        ++tokenIT;
    }
    if (tokenIT != tokens->end()) {
        return grammar->Program();
    }
    return true;
}

void Parser::Emit(ASTNode *node) {
    switch (node->GetType()) {
        default:
            break;
    }
}

void Parser::InitializeScope() {
    symbols.push_back(new SymbolTable());
}

void Parser::FinalizeScope() {
    delete symbols.back();
    symbols.pop_back();
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

string Parser::GetOutputFileName() {
    return output_file;
}
