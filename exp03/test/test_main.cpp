#include "TestILOCParser.h"
#include <iostream>

int main() {
    try {
        TestILOCParser::runAllTests();
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
