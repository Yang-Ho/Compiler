#include "ASTnode.h"

#include <map>
#include <string>

using namespace::std;

int ASTNode::label_count = 0;

ASTNode::ASTNode() {
    attributes = map<string, int>();
}

ASTNode::~ASTNode() {
}

void ASTNode::UpdateAttribute(string attribute, int value) {
    attributes[attribute] = value;
}

void ASTNode::UpdateAttribute(string attribute) {
    if (attribute == "label") {
        attributes[attribute] = ++label_count;
    }
}

int ASTNode::GetAttribute(string attribute) {
    return attributes[attribute];
}
