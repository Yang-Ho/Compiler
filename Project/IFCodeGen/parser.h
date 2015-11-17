/*
 * Yang Ho
 * CSC 512
 * parser.h
 *
 * Parser that is a mix between the sample solution and my own solution
 * I used the overall structure and design (separating the grammar into a separate file) from the sample solution
 *
 * But used my own implementation of the algorithms and such
 */
#ifndef PARSER_H
#define PARSER_H 

#include "token.h"
#include "grammar.h"
#include "ASTnode.h"

#include <vector>
#include <fstream>

using namespace::std;

class Grammar;

class Parser {
    private:
        vector<Token *> *tokens;            // Scanned tokens
        vector<Token *>::iterator tokenIT;  // The current token
        
        Grammar* grammar;                   // The grammar used

        // Project 2 legacy code
        int num_variables;
        int num_functions;
        int num_statements;
    public:
        // Constructor and deconstructor
        Parser(Grammar *new_grammar);
        ~Parser();

        // Token related
        Token* Consume(TokenType t_type);
        Token* GetCurrToken();
        string GetCurrTokenValue();
        int GetCurrTokenType();
        void AddToken(Token *new_token);

        // Parsing related 
        StmtNode *Parse();


        // Project 2 legacy code
        int GetNumVariables();
        int GetNumFunctions();
        int GetNumStatements();
        void IncNumVariables();
        void IncNumFunctions();
        void IncNumStatements();
        void DecNumVariables();
        void DecNumFunctions();
        void DecNumStatements();
};

#endif /* PARSER_H */
