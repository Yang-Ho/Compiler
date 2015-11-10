#ifndef ASTNODE_H
#define ASTNODE_H 

#include <map>
#include <string>

using namespace::std;

class ASTNode {
    private:
        map<string, int> attributes;
        static int label_count; //maybe move to ASTnode? or make non static
    public:

        ASTNode();
        ~ASTNode();

        void UpdateAttribute(string attribute, int value);
        void UpdateAttribute(string attribute);
        int GetAttribute(string attribute);
};
#endif /* ASTNODE_H */
