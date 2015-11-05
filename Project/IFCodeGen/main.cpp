/*
 * Yang Ho
 * CSC 512
 * Scanner Project
 * 9/28/2015
 */
#include "compiler.h"
#include "parser.h"
#include "token.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <exception>

using namespace::std;

int main(int argc, char* argv[]) {
    try {
        // Check command line input 
        if (argc < 2) {
            cout<<"Error: no file provided!\n";
            return -1;
        }

        cout<<"Input file: "<<argv[1]<<"\n";
        // Initialize scanner and prepare output file
        Scanner scanner(argv[1]);

        bool error = false;
        // Scan for token
        while (scanner.HasMoreTokens()) {
            Token t = scanner.GetNextToken();
            if (t.type == ERROR) {
                cout<<"Scanner Error in "<<argv[1]<<": "<<t.value<<" is an invalid token!\n";
                error = true;
                break;
            } else if (t.type != META){
                tokens.push_back(t);
            }
        }
        // Parse program
        if (!error) {
            if (Program()) {
                if (tokens[tIndex].value == tokens.back().value) {
                    cout<<"Pass variable "<<num_variables<<" functions "<<num_functions<<" statement "<<num_statements<<endl;
                } else {
                    cout<<"Failed to parse: "<<tokens[tIndex].value<<"\n";
                }
            } else {
                cout<<"Failed to parse\n";
            }
        }

        return 0;
    } catch(exception &) {
        return 1;
    }
}
