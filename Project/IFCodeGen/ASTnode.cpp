#include "ASTnode.h"

#include <map>
#include <string>

using namespace::std;

ASTNode::ASTNode() {
    var_count = 0;
    type = AST_UNDEFINED;
    left = NULL;
    right = NULL;
    child = NULL;
    value = "";
}

ASTNode::ASTNode(ASTNodeType t, string v, ASTNode* c, ASTNode* l, ASTNode* r) {
    var_count = 0;
    type = t;
    left = l;
    right = r;
    child = c;
    value = v;
}

ASTNode::~ASTNode() {
    delete left;
    delete right;
}

void ASTNode::SetValue(string v) {
    value = v;
}

void ASTNode::SetVar(int v) {
    var_count = v;
}

void ASTNode::SetType(ASTNodeType t) {
    type = t;
}

void ASTNode::SetLeft(ASTNode* l) {
    left = l;
}

void ASTNode::SetRight(ASTNode* r) {
    right = r;
}

void ASTNode::SetChild(ASTNode* c) {
    child = c;
}

string ASTNode::GetValue() {
    return value;
}

int ASTNode::GetVar() {
    return var_count;
}

ASTNodeType ASTNode::GetType() {
    return type;
}

ASTNode* ASTNode::GetLeft() {
    return left;
}

ASTNode* ASTNode::GetRight() {
    return right;
}

ASTNode* ASTNode::GetChild() {
    return child;
}
