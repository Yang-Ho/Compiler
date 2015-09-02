#include "scanner.h"
//#include "token.h"

#include <iostream>

using namespace::std;

int main(int argc, char* argv[]) {
    cout<<"Initializing Scanner\n";
    Scanner scanner(argv[1]);
    cout<<"Initialized Scanner\n";
    while (scanner.HasMoreTokens()) {
        Token t = scanner.GetNextToken();
        if (t.type == NUMBER && t.value != "main") {
        } else {
        }
    }
    return 0;
}
