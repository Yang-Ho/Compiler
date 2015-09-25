/*
 * Yang Ho
 * CSC 512
 * Scanner Project
 * 9/8/2015
 */
#include "scanner.h"
#include "parser.h"
#include "token.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

using namespace::std;

int main(int argc, char* argv[]) {
    // Check command line input 
    if (argc < 2) {
        cout<<"Error: no file provided!\n";
        return -1;
    }

    // Initialize scanner and prepare output file
    Scanner scanner(argv[1]);

    bool error = false;
    // Scan for token
    while (scanner.HasMoreTokens()) {
        Token t = scanner.GetNextToken();
        if (t.type == ERROR) {
            cout<<"Error in "<<argv[1]<<": "<<t.value<<" is an invalid token!\n";
            error = true;
            break;
        } else if (t.type != META) {
            tokens.push_back(t);
        }
    }
    if (!error) {
        cout<<"Checking if valid program!\n";
        if (Program()) {
            for (int i = tIndex; i < tokens.size(); i++) {
                cout<<"Leftover: "<<tokens[i].value<<"\n";
            }
            cout<<"Pass variable "<<num_variables<<" functions "<<num_functions<<" statement "<<num_statements<<endl;
        } else {
            cout<<"ERRORRJ:AKDFJ\n";
        }
    }

    return 0;
}
