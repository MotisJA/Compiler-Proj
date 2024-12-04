
#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>
#include <vector>
#include <memory>

class ASTNode {
public:
    enum class NodeType {
        GRAMMAR,
        PRODUCTION,
        ALTERNATIVE,
        SYMBOL
    };

    ASTNode(NodeType type, const std::string& value = "");
    void addChild(std::shared_ptr<ASTNode> child);
    void print(int indent = 0) const;

private:
    NodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;
};

#endif