#ifndef LABEL_H
#define LABEL_H 
#include <sstream>
#include <string>

using namespace::std;

class Label {
    private:
        static int next;
        int value;
    public:
        Label();
        string str();
};

#endif /* LABEL_H */
