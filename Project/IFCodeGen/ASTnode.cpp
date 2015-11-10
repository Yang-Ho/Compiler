#include "ASTnode.h"

#include <map>
#include <string>

using namespace::std;

int ASTNode::label_count = 0;

ASTNode::ASTNode(ASTNodeType t, string value) {
    type = t;
    left = NULL;
    right = NULL;
    value = "";
}

ASTNode::ASTNode(ASTNodeType t, ASTNode* l, ASTNode* r) {
    type = t;
    left = l;
    right = r;
    value = "";
}

ASTNode::~ASTNode() {
    delete left;
    delete right;
}

string ASTNode::GetValue() {
    return value;
}

ASTNodeType ASTNode::GetType() {
    return type;
}
