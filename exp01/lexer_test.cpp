#include <iostream>
#include <fstream>
#include "lexer.h"

void runTest(const std::string& input) {
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();
    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
    }
}

int main() {
    std::ifstream testFile("lexer_test.txt");
    if (!testFile) {
        std::cerr << "Failed to open lexer_test.txt" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(testFile, line)) {
        std::cout << "Input:"<< std::endl << line << std::endl;
        std::cout << "Output:" << std::endl;
        runTest(line);
        std::cout << "------------------------------------------" << std::endl;
    }

    return 0;
}