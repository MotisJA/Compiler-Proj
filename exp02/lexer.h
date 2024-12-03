#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include <vector>

enum TokenType {
    SEMICOLON = 0,
    DERIVES,
    ALSODERIVES,
    EPSILON,
    SYMBOL,
    EOF_TOKEN,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;

    std::string toString() const;
};

class Lexer {
public:
    Lexer(const std::string &input);
    std::vector<Token> tokenize();

private:
    std::string input;  // 输入字符串
    size_t pos; // 当前读取位置
    std::unordered_map<std::string, TokenType> tokenMap; // 哈希表，快速查找 Token 类型

    void initTokenMap();
    Token getNextToken();
    Token EPSILON();
    Token SYMBOL();
    char peek() const;
    char get();
    bool isEOF() const;
};

#endif // LEXER_H
