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

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <exception>

using namespace::std;

int main(int argc, char* argv[]) {
    // Check command line input 
    if (argc < 2) {
        cout<<"Error: no file provided!\n";
        return -1;
    }

    cout<<"Input file: "<<argv[1]<<", ";
    // Initialize scanner and prepare output file
    
    Scanner scanner(argv[1]);
    Grammar *grammar = new Grammar();
    Parser parser(grammar);
    while (scanner.HasMoreTokens()) {
        Token* t = scanner.GetNextToken();
        if (t->GetTokenType() == TOKEN_ERROR) {
            //cout<<"Scanning error: Unidentified token "<<t->GetTokenValue()<<"\n";
            delete t;
            cout<<"Error\n";
            return 1;
        } 
        parser.AddToken(t);
    }
    if (parser.Parse()) {
        cout<<"Pass variable "<<parser.GetNumVariables()<<" functions "<<parser.GetNumFunctions()<<" statement "<<parser.GetNumStatements()<<endl;
    } else {
        cout<<"Falied to parse\n";
    }
    return 0;
}
