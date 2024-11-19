#include "paser.h"
#include <iostream>

// 构造函数，初始化tokens列表和位置
Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {} 

void Parser::parse() {
    Grammar();
}

// Grammer → ProductionList
void Parser::Grammar() {
    ProductionList();
}

// ProductionList →  ProductionSet SEMICOLON ProductionList'
void Parser::ProductionList() {
    ProductionSet();
    if (match(SEMICOLON)) {
        get();
        ProductionListPrime();
    } else {
        std::cerr << "Error: Expected ';' after ProductionSet, but got '" << peek().value << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// ProductionList'→  ProductionSet SEMICOLON ProductionList' | ε
void Parser::ProductionListPrime() {
    if (match(SYMBOL)) {
        ProductionSet();
        if (match(SEMICOLON)) {
            get();
            ProductionListPrime();
        } else {
            std::cerr << "Error: Expected ';' after ProductionSet, but got '" << peek().value << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
    } else if (!match(EOF_TOKEN)) {
        std::cout << pos << std::endl;
        std::cout << tokens.size() << std::endl;
        // 递归终点：当不再匹配SYMBOL时，递归终止
        std::cerr << "Error: Unexpected token '" << peek().value << "' after ProductionList" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// ProductionSet  →  SYMBOL DERIVES RightHandSide ProductionSet'
void Parser::ProductionSet() {
    if (match(SYMBOL)) {
        std::string lhs = get().value; // 获取产生式左侧符号
        if (match(DERIVES)) {
            get();
            RightHandSide();
            ProductionSetPrime();
        } else {
            std::cerr << "Error: Expected ':' after SYMBOL, but got '" << peek().value << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "Error: Expected SYMBOL at the beginning of ProductionSet, but got '" << peek().value << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// ProductionSet' →  ALSODERIVES RightHandSide ProductionSet' | ε
void Parser::ProductionSetPrime() {
    if (match(ALSODERIVES)) {
        get();
        RightHandSide();
        ProductionSetPrime();
    }
    // ε (epsilon) case: do nothing
}

// RightHandSide  →  SymbolList | EPSILON
void Parser::RightHandSide() {
    if (match(EPSILON)) {
        get();
    } else {
        SymbolList();
    }
}

// SymbolList  →  SYMBOL SymbolList'
void Parser::SymbolList() {
    if (match(SYMBOL)) {
        get();
        SymbolListPrime();
    } else {
        std::cerr << "Error: Expected SYMBOL in SymbolList, but got '" << peek().value << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
}

// SymbolList' →  SYMBOL SymbolList' | ε
void Parser::SymbolListPrime() {
    if (match(SYMBOL)) {
        get();
        SymbolListPrime();
    }
    // ε (epsilon) case: do nothing
}

bool Parser::match(TokenType type) {
    return !isEOF() && peek().type == type;
}

 // 查看当前token
Token Parser::peek() const {
    return tokens[pos];
}

 // 获取当前token并移动到下一个
Token Parser::get() {
    return tokens[pos++];
}

 // 检查是否到达token列表末尾
bool Parser::isEOF() const {
    return pos >= tokens.size();
}

void Parser::addProduction(const std::string& lhs, const std::vector<std::string>& rhs) {
    productions.emplace_back(lhs, rhs);
}
