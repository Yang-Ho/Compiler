/*
 * Yang Ho
 * CSC 512
 * Code Generation Project
 */
#include "parser.h"
#include "grammar.h"
#include "scanner.h"
#include "token.h"
#include "label.h"
#include "ASTnode.h"
#include "symbol.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <stdio.h>

using namespace::std;

ofstream output;
int main(int argc, char* argv[]) {
    // Check command line input 
    if (argc < 2) {
        cout<<"Error: no file provided!\n";
        return -1;
    }

    //cout<<"Input file: "<<argv[1]<<" ";

    // Initialize scanner and parser 
    Scanner scanner(argv[1]);
    Grammar *grammar = new Grammar();
    Parser parser(grammar);

    // Scan file and produce tokens
    while (scanner.HasMoreTokens()) {
        Token* t = scanner.GetNextToken();
        if (t->GetTokenType() == TOKEN_ERROR) {
            cout<<"Scanning error: Unidentified token "<<t->GetTokenValue()<<"\n";
            delete t;
            return 1;
        } 
        parser.AddToken(t);
    }

    // Prepare output file
    string fileName = argv[1];
    string output_file = fileName.substr(0, fileName.rfind('.'));
    output_file += "_gen.c";
    output.open(output_file.c_str());

    // Parse program and produce AST
    StmtNode * ast_tree = parser.Parse();
    if (ast_tree) {
        // Generate code
        //cout<<"Successfully parsed the program!\n";
        //cout<<"Generating code...\n";
        SymbolTable* symtab = new SymbolTable(); 
        ast_tree->genCode(symtab);
        cout<<"Done generating code\n";
        cout<<"Output file: "<<output_file<<"\n";
        output.close(); 
    } else {
        // Error parsing code
        output.close(); 
        cout<<"Falied to parse\n";
        remove(output_file.c_str());
    }
   
    return 0;
}
