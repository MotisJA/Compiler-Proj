
#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "LL1Parser.h"
#include "LL1TableGenerator.h"

void runTest(const std::string& input, const std::string& testName) {
    std::cout << "\n=== Test Case: " << testName << " ===" << std::endl;
    std::cout << "Input:\n" << input << std::endl;

    // 词法分析
    std::cout << "\nLexical Analysis:" << std::endl;
    Lexer lexer(input);
    std::vector<Token> tokens;
    try {
        tokens = lexer.tokenize();
        for (const auto& token : tokens) {
            std::cout << token.toString() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Lexer Error: " << e.what() << std::endl;
        return;
    }

    // 语法分析
    std::cout << "\nSyntax Analysis:" << std::endl;
    LL1Parser parser;
    try {
        parser.parse(tokens);
    } catch (const std::exception& e) {
        std::cerr << "Parser Error: " << e.what() << std::endl;
        return;
    }

    std::cout << "\nTest completed successfully." << std::endl;
    std::cout << "================================\n" << std::endl;
}

int main() {
    std::ifstream testFile("test_cases.txt");
    if (!testFile) {
        std::cerr << "Error: Cannot open test_cases.txt" << std::endl;
        return 1;
    }

    std::string testCase;
    std::string testName;
    int testCount = 0;

    while (std::getline(testFile, testName)) {
        if (testName.empty() || testName[0] == '#') continue;
        
        std::stringstream caseContent;
        std::string line;
        while (std::getline(testFile, line) && line != "---") {
            caseContent << line << "\n";
        }
        
        if (!caseContent.str().empty()) {
            testCount++;
            runTest(caseContent.str(), testName);
        }
    }

    std::cout << "Completed " << testCount << " test cases." << std::endl;
    return 0;
}