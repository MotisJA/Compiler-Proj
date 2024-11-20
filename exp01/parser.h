
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>
#include <unordered_map>

class Parser {
public:
    Parser(const std::vector<Token>& tokens); // 构造函数，初始化解析器
    void parse(); // 解析函数

private:
    std::vector<Token> tokens; // 词法分析器生成的标记列表
    size_t pos; // 当前解析位置

    class Production {
    public:
        std::string lhs; // 产生式左侧
        std::vector<std::string> rhs; // 产生式右侧

        Production(const std::string& lhs, const std::vector<std::string>& rhs)
            : lhs(lhs), rhs(rhs) {}
    };

    std::vector<Production> productions; // 存储所有的产生式

    void addProduction(const std::string& lhs, const std::vector<std::string>& rhs); // 添加产生式的方法

    void Grammar(); // 解析文法
    void ProductionList();
    void ProductionSet();
    void RightHandSide();
    void SymbolList();
    bool match(TokenType type);
    Token peek() const;
    Token get();
    bool isEOF() const;
    void ProductionListPrime();
    void ProductionSetPrime();
    void SymbolListPrime();
};

#endif // PARSER_H