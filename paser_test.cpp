#include "lexer.h"
#include "paser.h"
#include <iostream>
#include <fstream>
#include <sstream>

void runTest(const std::string& input) {
    std::cout << "Input:" << std::endl << input << std::endl;

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
        std::cerr << "Test failed - " << e.what() << std::endl;
    }

    std::cout << "------------------------------------------" << std::endl;
}

int main() {
    std::ifstream testFile("paser_test.txt");
    if (!testFile) {
        std::cerr << "Error: Cannot open file paser_test.txt" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << testFile.rdbuf();
    std::string input = buffer.str();

    runTest(input);

    return 0;
}