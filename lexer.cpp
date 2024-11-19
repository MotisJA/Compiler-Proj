#include "lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(const std::string &input) : input(input), pos(0) {
    initTokenMap();
}

char Lexer::peek() const {
    return input[pos];
}

char Lexer::get() {
    return input[pos++];
}

bool Lexer::isEOF() const {
    return pos >= input.size();
}

void Lexer::initTokenMap() {
    tokenMap[";"] = SEMICOLON;
    tokenMap[":"] = DERIVES;
    tokenMap["|"] = ALSODERIVES;
    tokenMap["EPSILON"] = TokenType::EPSILON;
    tokenMap["epsilon"] = TokenType::EPSILON;
    tokenMap["Epsilon"] = TokenType::EPSILON;
}

// 格式化 Token 输出
std::string Token::toString() const {
    static const std::unordered_map<TokenType, std::string> tokenTypeToString = {
        {SEMICOLON, "SEMICOLON"},
        {DERIVES, "DERIVES"},
        {ALSODERIVES, "ALSODERIVES"},
        {EPSILON, "EPSILON"},
        {SYMBOL, "SYMBOL"},
        {EOF_TOKEN, "EOF_TOKEN"},
        {UNKNOWN, "UNKNOWN"}
    };

    return "Token(" + tokenTypeToString.at(type) 
                + ", \"" + value + "\", " + std::to_string(type) + ")";
}

// 词法分析，将输入字符串转换为 Token 序列
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!isEOF()) {
        Token token = getNextToken();
        if (token.type == UNKNOWN) {
            std::cerr << "Error: Unknown token \"" << token.value << "\" at position " << pos << std::endl;
            exit(EXIT_FAILURE);
        }
        tokens.push_back(token);
    }
    tokens.push_back({EOF_TOKEN, ""});
    return tokens;
}

Token Lexer::getNextToken() {
    while (!isEOF() && std::isspace(peek())) {
        get();
    }

    if (isEOF()) {
        return {EOF_TOKEN, ""};
    }

    char current = peek();
    if (current == ';' || current == ':' || current == '|') {
        std::string value(1, get());
        return {tokenMap[value], value};
    }

    if (std::isalpha(current) || std::isdigit(current)) {
        return SYMBOL();
    }

    get();
    return {TokenType::UNKNOWN, std::string(1, current)};
}

// [a-z | A-Z | 0-9]+
Token Lexer::SYMBOL() {
    std::string value;
    while (!isEOF() && (std::isalpha(peek()) || std::isdigit(peek()))) {
        value += get();
    }
    if (tokenMap.find(value) != tokenMap.end()) {
        return {tokenMap[value], value};
    }

    return {TokenType::SYMBOL, value};
}
