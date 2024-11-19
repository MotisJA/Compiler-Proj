#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <sstream>

int main() {
    std::string input;
    std::cout << "Enter input: " << std::endl;
    std::getline(std::cin, input);

    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "Lexer Output:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
    }

    Parser parser(tokens);
    std::cout << "Parser Output:" << std::endl;
    try {
        parser.parse();
        std::cout << "Parsing completed successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed - " << e.what() << std::endl;
    }

    return 0;
}
