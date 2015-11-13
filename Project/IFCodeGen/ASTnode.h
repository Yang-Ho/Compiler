#ifndef ASTNODE_H
#define ASTNODE_H 

#include <string>

using namespace::std;

typedef enum{
    AST_UNDEFINED = 1,
    AST_EMPTY,
    AST_FUNC_CALL,
    AST_ARRAY_ELEMENT,
    AST_FACTOR,
    AST_OP,
    AST_TERM,
    AST_EXPRESSION,
    AST_CONTINUE,
    AST_BREAK,
    AST_RETURN,
    AST_WHILE,
    AST_COMPARISON_OP,
    AST_CONDITION,
    AST_CONDITION_OP,
    AST_CONDITION_EXPR,
    AST_IF,
    AST_NON_EMPTY_EXPR_LIST,
    AST_EXPR_LIST,
    AST_ASSIGNMENT,
    AST_ID_STATEMENT,
    AST_READ,
    AST_WRITE,
    AST_PRINT,
    AST_STATEMENT,
    AST_BLOCK,
    AST_ID,
    AST_ID_LIST,
    AST_DATA_DECLS, 
    AST_NON_EMPTY_LIST,
    AST_NON_EMPTY_LIST_PRIME,
    AST_PARAM_LIST,
    AST_RESERVED,
    AST_FUNC_DECL,
    AST_FUNC_TAIL,
    AST_FUNC,
    AST_FUNC_LIST,
    AST_DATA_DECLS_PRIME_TAIL,
    AST_DATA_DECLS_PRIME,
    AST_PROGRAM
} ASTNodeType;

class ASTNode {
    private:
        ASTNode * left;         // Left child node 
        ASTNode * right;        // Right child node 
        ASTNode * child;        // Direct child

        int var_count;          // Count of the variable in terms of local
        string value;           // The value of the node
        ASTNodeType type;       // The type of node
    public:
        ASTNode();
        ASTNode(ASTNodeType t, string v, ASTNode*c, ASTNode* l, ASTNode* r);
        ~ASTNode();

        void SetValue(string v);
        void SetVar(int v);
        void SetType(ASTNodeType t);
        void SetLeft(ASTNode* l);
        void SetRight(ASTNode* r);
        void SetChild(ASTNode* c);
        string GetValue();
        int GetVar();
        ASTNodeType GetType();
        ASTNode* GetLeft();
        ASTNode* GetRight();
        ASTNode* GetChild();
};
#endif /* ASTNODE_H */
