#include "LL1Parser.h"
#include <iostream>

LL1Parser::LL1Parser() {
    // 初始化解析器
}

std::string LL1Parser::tokenTypeToString(TokenType type) const {
    switch(type) {
        case SEMICOLON: return "SEMICOLON";
        case DERIVES: return "DERIVES";
        case ALSODERIVES: return "ALSODERIVES";
        case EPSILON: return "EPSILON";
        case SYMBOL: return "SYMBOL";
        case EOF_TOKEN: return "EOF_TOKEN";
        default: return "UNKNOWN";
    }
}

bool LL1Parser::isTerminal(const std::string& symbol) {
    return symbol == "SYMBOL" || symbol == "DERIVES" || 
           symbol == "ALSODERIVES" || symbol == "EPSILON" || 
           symbol == "SEMICOLON" || symbol == "EOF_TOKEN";
}

void LL1Parser::reportError(const std::string& message) const {
    std::cerr << "Error: " << message << std::endl;
}

std::shared_ptr<ASTNode> LL1Parser::parse(const std::vector<Token>& tokenStream) {
    size_t currentPos = 0;
    
    // 清空并初始化解析栈
    while (!parseStack.empty()) parseStack.pop();
    while (!nodeStack.empty()) nodeStack.pop();
    
    auto root = std::make_shared<ASTNode>(ASTNode::NodeType::GRAMMAR);
    currentNode = root;
    
    parseStack.push("EOF_TOKEN");
    parseStack.push("Grammar");
    
    std::shared_ptr<ASTNode> currentProduction = nullptr;
    std::shared_ptr<ASTNode> currentAlternative = nullptr;
    std::string leftSideSymbol; // 用于存储产生式左侧的符号
    
    while (true) {
        std::string focus = parseStack.empty() ? "EOF_TOKEN" : parseStack.top();
        std::string currentType = tokenTypeToString(tokenStream[currentPos].type);
        
        if (focus == "EOF_TOKEN" && currentType == "EOF_TOKEN") {
            std::cout << "Successfully parsed input!\n";
            break;
        }
        
        if (isTerminal(focus)) {
            if (focus == currentType) {
                if (focus == "SYMBOL") {
                    if (parseStack.size() > 1) {
                        std::string nextSymbol = tokenStream[currentPos + 1].type == DERIVES ? 
                            tokenStream[currentPos].value : "";
                        
                        if (!nextSymbol.empty()) {
                            // 这是产生式的左侧
                            leftSideSymbol = nextSymbol;
                            currentProduction = std::make_shared<ASTNode>(
                                ASTNode::NodeType::PRODUCTION,
                                nextSymbol
                            );
                            root->addChild(currentProduction);
                        } else {
                            // 这是产生式右侧的符号
                            auto symbolNode = std::make_shared<ASTNode>(
                                ASTNode::NodeType::SYMBOL,
                                tokenStream[currentPos].value
                            );
                            if (currentAlternative) {
                                currentAlternative->addChild(symbolNode);
                            }
                        }
                    }
                } else if (focus == "EPSILON") {
                    auto epsilonNode = std::make_shared<ASTNode>(
                        ASTNode::NodeType::SYMBOL,
                        "EPSILON"
                    );
                    if (currentAlternative) {
                        currentAlternative->addChild(epsilonNode);
                    }
                }
                parseStack.pop();
                currentPos++;
            } else {
                reportError("Unexpected token: " + tokenStream[currentPos].value);
                return nullptr;
            }
        } else {
            int productionIndex = tableGenerator.getParsingTableEntry(focus, currentType);
            
            if (productionIndex >= 0) {
                parseStack.pop();
                auto production = tableGenerator.getProduction(productionIndex);
                
                if (focus == "RightHandSide") {
                    currentAlternative = std::make_shared<ASTNode>(
                        ASTNode::NodeType::ALTERNATIVE
                    );
                    if (currentProduction) {
                        currentProduction->addChild(currentAlternative);
                    }
                }
                
                for (auto it = production.second.rbegin(); 
                     it != production.second.rend(); ++it) {
                    if (*it != "#") {
                        parseStack.push(*it);
                    }
                }
            } else {
                reportError("No production rule for " + focus);
                return nullptr;
            }
        }
    }
    
    return root;
}