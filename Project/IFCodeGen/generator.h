#ifndef GENERATOR_H
#define GENERATOR_H 
#include <string>
#include <fstream>

using namespace::std;

class Generator {
    private:
        string output_file;
        ofstream output;
    public:
        Generator(string fileName);
        ~Generator();

        void Emit(string code_line);
};
#endif /* GENERATOR_H */
