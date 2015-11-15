/*
 * Yang Ho
 * CSC 512
 * Scanner Project
 * 9/28/2015
 */
#include "parser.h"
#include "grammar.h"
#include "scanner.h"
#include "token.h"
#include "address.h"
#include "label.h"
#include "ASTnode.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

using namespace::std;

ofstream output;
int main(int argc, char* argv[]) {
    // Check command line input 
    if (argc < 2) {
        cout<<"Error: no file provided!\n";
        return -1;
    }

    cout<<"Input file: "<<argv[1]<<" ";
    // Initialize scanner and prepare output file

    Scanner scanner(argv[1]);
    Grammar *grammar = new Grammar();
    Parser parser(grammar);

    string fileName = argv[1];
    string output_file = fileName.substr(0, fileName.rfind('.'));
    output_file += "_gen.c";
    output.open(output_file.c_str());
    while (scanner.HasMoreTokens()) {
        Token* t = scanner.GetNextToken();
        if (t->GetTokenType() == TOKEN_ERROR) {
            cout<<"Scanning error: Unidentified token "<<t->GetTokenValue()<<"\n";
            delete t;
            return 1;
        } 
        parser.AddToken(t);
    }
    StmtNode * ast_tree = parser.Parse();
    if (ast_tree) {
        cout<<"Successfully parsed the program!\n";
        Label l;
        cout<<"Generating code...\n";
        ast_tree->genCode(l);
        cout<<"Done generating code\n";
        cout<<"Output file: "<<output_file<<"\n";
    } else {
        cout<<"Falied to parse\n";
    }
    //delete grammar;
    //delete ast_tree;
   
    output.close(); 
    return 0;
}
