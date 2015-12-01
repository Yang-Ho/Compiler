/*
 * Yang Ho
 * CSC 512
 * label.h
 *
 * Header file for goto labels
 */
#ifndef LABEL_H
#define LABEL_H 
#include <sstream>
#include <string>

using namespace::std;

class Label {
    private:
        static int next;        // Keeps track of the next label to create
        int value;              // The label associated with a instance of a label object
    public:
        Label();
        string str();
};

#endif /* LABEL_H */
