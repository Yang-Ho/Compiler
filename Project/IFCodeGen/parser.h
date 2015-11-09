#ifndef PARSER_H
#define PARSER_H 

#include "token.h"
#include "grammar.h"
#include "generator.h"

#include <vector>

using namespace::std;

class Grammar;
class Generator;

class Parser {
    private:
        vector<Token *> *tokens;
        vector<Token *>::iterator tokenIT;

        int num_variables;
        int num_functions;
        int num_statements;

        Grammar* grammar;
        Generator* generator;
    public:
        Parser(Grammar *new_grammar, Generator* new_generator);
        ~Parser();

        Token* Consume(TokenType t_type);
        Token* GetCurrToken();
        string GetCurrTokenValue();
        int GetCurrTokenType();

        void AddToken(Token *new_token);

        bool Parse();

        void Emit(string code_line);

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
