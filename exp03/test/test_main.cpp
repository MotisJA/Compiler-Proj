#include "TestILOCParser.h"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // 运行基本测试套件
        TestILOCParser::runAllTests();
        
        // 如果提供了输入文件，则测试文件解析
        if (argc > 1) {
            TestILOCParser::testInputFile(argv[1]);
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
