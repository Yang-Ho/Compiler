#include "generator.h"

#include <string>
#include <fstream>

using namespace::std;

Generator::Generator(string fileName) {
    output_file = fileName.substr(0, fileName.rfind('.'));
    output.open(output_file.c_str());
}

Generator::~Generator() {
    output.close();
}

void Generator::Emit(string code_line) {
    output<<code_line;
}
