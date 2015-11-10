#ifndef PARSER_H
#define PARSER_H 

#include "token.h"
#include "grammar.h"
#include "symbol.h"
#include "ASTnode.h"

#include <vector>
#include <fstream>

using namespace::std;

class Grammar;
class Generator;

class Parser {
    private:
        vector<Token *> *tokens;            // Scanned tokens
        vector<Token *>::iterator tokenIT;  // The current token
        
        Grammar* grammar;                   // The grammar used

        string output_file;                 // Output file name
        ofstream output;                    // The output file stream

        vector<SymbolTable *> symbols;      // Symbol table

        int num_variables;
        int num_functions;
        int num_statements;
    public:
        // Constructor and deconstructor
        Parser(Grammar *new_grammar, string fileName);
        ~Parser();

        // Token related
        Token* Consume(TokenType t_type);
        Token* GetCurrToken();
        string GetCurrTokenValue();
        int GetCurrTokenType();
        void AddToken(Token *new_token);

        // Parsing related 
        bool Parse();

        // Code generation related
        void Emit(ASTNode *node);
        void InitializeScope();
        void FinalizeScope();

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

        // Output related
        string GetOutputFileName();
};

#endif /* PARSER_H */
