
#include "ASTNode.h"
#include <iostream>

ASTNode::ASTNode(NodeType type, const std::string& value)
    : type(type), value(value) {}

void ASTNode::addChild(std::shared_ptr<ASTNode> child) {
    children.push_back(child);
}

void ASTNode::print(int indent) const {
    std::string indentStr(indent * 2, ' ');
    
    switch(type) {
        case NodeType::GRAMMAR:
            std::cout << indentStr << "Grammar" << std::endl;
            break;
        case NodeType::PRODUCTION:
            std::cout << indentStr << "Production: " << value << std::endl;
            break;
        case NodeType::ALTERNATIVE:
            std::cout << indentStr << "Alternative" << std::endl;
            break;
        case NodeType::SYMBOL:
            std::cout << indentStr << "Symbol: " << value << std::endl;
            break;
    }

    for (const auto& child : children) {
        child->print(indent + 1);
    }
}