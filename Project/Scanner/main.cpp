/*
 * Yang Ho
 * CSC 512
 * Scanner Project
 * 9/8/2015
 */
#include "scanner.h"

#include <iostream>
#include <fstream>
#include <cstdio>

using namespace::std;

int main(int argc, char* argv[]) {
    // Check command line input 
    if (argc < 2) {
        cout<<"Error: no file provided!\n";
        return -1;
    }

    // Initialize scanner and prepare output file
    Scanner scanner(argv[1]);

    string outFileName(argv[1]);
    outFileName.insert(outFileName.length() - 2, "_gen");

    ofstream fout;
    fout.open(outFileName.c_str());

    bool error = false;
    // Scan for token
    while (scanner.HasMoreTokens()) {
        Token t = scanner.GetNextToken();
        if (t.type == ID && t.value != "main") {
            fout<<"cs512"<<t.value<<" ";
        } else if (t.type != ERROR) {
            fout<<t.value<<" ";
        } else {
            cout<<"Error in "<<argv[1]<<": "<<t.value<<" is an invalid token!\n";
            error = true;
            break;
        }
    }

    fout.close();
    // Delete *_gen.c file if there is an error
    if (error) {
        remove(outFileName.c_str());
    }
    return 0;
}
