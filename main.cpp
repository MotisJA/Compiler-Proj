#include "lexer.h"
#include "paser.h"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string input = buffer.str();

    Lexer lexer(input);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parse();

    std::cout << "Parsing completed successfully." << std::endl;
    return 0;
}
