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

void LL1Parser::parse(const std::vector<Token>& tokenStream) {
    size_t currentPos = 0;
    
    // 清空并初始化解析栈
    while (!parseStack.empty()) {
        parseStack.pop();
    }
    
    // 压入初始符号
    parseStack.push("EOF_TOKEN");
    parseStack.push("Grammar");  // 起始符号
    
    // 主循环
    while (true) {
        std::string focus = parseStack.empty() ? "EOF_TOKEN" : parseStack.top();
        std::string currentType = tokenTypeToString(tokenStream[currentPos].type);
        
        // 检查是否成功完成解析
        if (focus == "EOF_TOKEN" && currentType == "EOF_TOKEN") {
            std::cout << "Successfully parsed input!\n";
            break;
        }
        
        // 处理终结符
        if (isTerminal(focus)) {
            if (focus == currentType) {
                parseStack.pop();
                currentPos++;
            } else {
                reportError("Unexpected token: " + tokenStream[currentPos].value + 
                          ", expected: " + focus);
                break;
            }
        }
        // 处理非终结符
        else {
            int productionIndex = tableGenerator.getParsingTableEntry(focus, currentType);
            
            if (productionIndex >= 0) {
                parseStack.pop();
                
                // 获取产生式右侧并反向压栈
                auto production = tableGenerator.getProduction(productionIndex);
                for (auto it = production.second.rbegin(); 
                     it != production.second.rend(); ++it) {
                    if (*it != "ε") {
                        parseStack.push(*it);
                    }
                }
            } else {
                reportError("No production rule for " + focus + 
                          " with input " + tokenStream[currentPos].value);
                break;
            }
        }
    }
}