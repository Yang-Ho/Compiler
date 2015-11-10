#ifndef ASTNODE_H
#define ASTNODE_H 

#include <string>

using namespace::std;

typedef enum{
    AST_UNDEFINED = 1,
} ASTNodeType;

class ASTNode {
    private:
        static int label_count; // Label to use for next goto, if, etc.
        ASTNode * left;         // Left child node 
        ASTNode * right;        // Right child node 
        string value;           // The value of the node
        ASTNodeType type;       // The type of node
    public:
        ASTNode(ASTNodeType t, string value);
        ASTNode(ASTNodeType t, ASTNode* l, ASTNode* r);
        ~ASTNode();

        string GetValue();
        ASTNodeType GetType();
};
#endif /* ASTNODE_H */
