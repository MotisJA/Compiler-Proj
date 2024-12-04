#ifndef LL1PARSER_H
#define LL1PARSER_H

#include <string>
#include <stack>
#include <vector>
#include "LL1TableGenerator.h"
#include "lexer.h"
#include "ASTNode.h"

class LL1Parser {
public:
    LL1Parser();
    std::shared_ptr<ASTNode> parse(const std::vector<Token>& tokenStream);

private:
    std::stack<std::string> parseStack;
    LL1TableGenerator tableGenerator;
    
    bool isTerminal(const std::string& symbol);
    void reportError(const std::string& message) const;
    
    // 将TokenType转换为分析表中使用的终结符字符串
    std::string tokenTypeToString(TokenType type) const;
    std::shared_ptr<ASTNode> currentNode;
    std::stack<std::shared_ptr<ASTNode>> nodeStack;
};

#endif // LL1PARSER_H