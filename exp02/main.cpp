#include <iostream>
#include <fstream>
#include <string>
#include "LL1TableGenerator.h"
#include "LL1Parser.h"
#include "lexer.h"

void printUsage() {
    std::cout << "Usage: \n"
              << "  ./parser           - Enter grammar input mode\n"
              << "  ./parser [options] - Show analysis tables\n"
              << "\nOptions:\n"
              << "  -p  Print productions\n"
              << "  -f  Print FIRST sets\n"
              << "  -g  Print FOLLOW sets\n"
              << "  -h  Print FIRST+ sets\n"
              << "  -t  Print LL(1) parsing table\n";
}

void processFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }

    Lexer lexer(content);
    std::vector<Token> tokens = lexer.tokenize();

    LL1Parser parser;
    auto ast = parser.parse(tokens);
    if (ast) {
        std::cout << "\nGenerated Abstract Syntax Tree:\n";
        ast->print();
    }
}

void processInput() {
    std::cout << "Enter grammar (end with an empty line):\n";
    std::string content;
    std::string line;
    
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        content += line + "\n";
    }

    if (!content.empty()) {
        Lexer lexer(content);
        std::vector<Token> tokens = lexer.tokenize();

        LL1Parser parser;
        auto ast = parser.parse(tokens);
        if (ast) {
            std::cout << "\nGenerated Abstract Syntax Tree:\n";
            ast->print();
        }
    }
}

int main(int argc, char* argv[]) {
    LL1TableGenerator generator;

    if (argc == 1) {
        // 无参数时直接进入输入模式
        processInput();
    } else if (argc == 2) {
        // 有参数时检查是否为选项
        std::string option = argv[1];
        if (option == "-p") {
            generator.printProductions();
        } else if (option == "-f") {
            generator.printFirstSets();
        } else if (option == "-g") {
            generator.printFollowSets();
        } else if (option == "-h") {
            generator.printFirstPlusSets();
        } else if (option == "-t") {
            generator.printParsingTable();
        } else {
            std::cerr << "Invalid option: " << option << "\n";
            printUsage();
            return 1;
        }
    } else {
        std::cerr << "Too many arguments\n";
        printUsage();
        return 1;
    }

    return 0;
}