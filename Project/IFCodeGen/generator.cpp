#include "generator.h"

#include <fstream>
#include <iostream>

using namespace::std;

Generator::Generator(string fileName) {
    label_count = 0;

    output_file = fileName.substr(0, fileName.rfind('.'));
    output_file += "_gen.c";
    output.open(output_file.c_str());
}

Generator::~Generator() {
    output.close();
}

void Generator::WriteToOutput(string line) {
    output<<line;
}

void Generator::Evaluate(ASTNode *node) {
}

int Generator::GetCurrLabel() {
    return label_count;
}

int Generator::GetWhileLabel() {
    return label_count - 2;
}

void Generator::IncLabel() {
    label_count += 1;
}

string Generator::GetOutputFileName() {
    return output_file;
}
