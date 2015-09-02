#include "scanner.h"

#include <iostream>
#include <fstream>

using namespace::std;

int main(int argc, char* argv[]) {
    Scanner scanner(argv[1]);
    ofstream fout;
    string outFileName(argv[1]);
    outFileName.insert(outFileName.length() - 2, "_gen");
    fout.open(outFileName.c_str());
    while (scanner.HasMoreTokens()) {
        Token t = scanner.GetNextToken();
        if (t.type == ID && t.value != "main") {
            fout<<"cs512"<<t.value<<" ";
        } else if (t.type != ERROR) {
            fout<<t.value<<" ";
        } else {
            cout<<"Error: "<<t.value<<" is not a valid token!\n";
            break;
        }
    }
    fout.close();
    return 0;
}
