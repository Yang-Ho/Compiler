#ifndef GENERATOR_H
#define GENERATOR_H 

#include "ASTnode.h"
#include "symbol.h"

#include <fstream>

using namespace::std;

class Generator {
    private:
        string output_file;                 // Output file name
        ofstream output;                    // The output file stream

        int label_count;
    public:
        Generator(string filename);
        ~Generator();
        // Code generation related
        void Evaluate(ASTNode *node);

        int GetCurrLabel();
        int GetWhileLabel();

        void IncLabel();

        // Output related
        string GetOutputFileName();
        void WriteToOutput(string line);
};
#endif /* GENERATOR_H */
