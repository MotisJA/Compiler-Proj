#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "ILOCParser.h"
#include "RegisterAllocator.h"
#include "ILOCFormatter.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <k> <t|b> <input_file>" << std::endl;
        std::cerr << "  k: number of available registers (positive integer)" << std::endl;
        std::cerr << "  t: top-down allocation strategy" << std::endl;
        std::cerr << "  b: bottom-up allocation strategy" << std::endl;
        std::cerr << "  input_file: path to ILOC input file" << std::endl;
        return 1;
    }

    // 解析k值参数
    int k;
    try {
        k = std::stoi(argv[1]);
        if (k <= 0) {
            std::cerr << "Error: k must be a positive integer" << std::endl;
            std::cerr << "Provided value: " << argv[1] << std::endl;
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse k value" << std::endl;
        std::cerr << "Provided value: " << argv[1] << std::endl;
        std::cerr << "Details: " << e.what() << std::endl;
        return 1;
    }

    // 检查分配策略参数
    std::string method = argv[2];
    if (method.length() != 1 || (method[0] != 't' && method[0] != 'b')) {
        std::cerr << "Error: Invalid allocation strategy" << std::endl;
        std::cerr << "Provided value: " << method << std::endl;
        std::cerr << "Expected: 't' for top-down or 'b' for bottom-up" << std::endl;
        return 1;
    }

    // 检查输入文件
    std::string inputFile = argv[3];
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open input file: " << inputFile << std::endl;
        std::cerr << "Please check if:" << std::endl;
        std::cerr << "  - The file exists" << std::endl;
        std::cerr << "  - You have read permissions" << std::endl;
        std::cerr << "  - The path is correct" << std::endl;
        return 1;
    }
    file.close();

    try {
        // 解析ILOC指令
        ILOCParser parser;
        auto instructions = parser.parseFile(inputFile);

        // 选择并执行寄存器分配策略
        std::vector<ILOCInstruction> result;
        if (method == "t") {
            TopDownAllocator allocator;
            result = allocator.allocate(instructions, k);
        } else {
            BottomUpAllocator allocator;
            result = allocator.allocate(instructions, k);
        }

        // 创建两个输出文件
        std::string numberedOutputFile = inputFile + ".out";
        std::string consoleOutputFile = inputFile + ".console.out";
        
        std::ofstream numberedFile(numberedOutputFile);
        std::ofstream consoleFile(consoleOutputFile);
        
        if (!numberedFile.is_open() || !consoleFile.is_open()) {
            std::cerr << "Error: Could not create output files" << std::endl;
            return 1;
        }

        // 格式化并输出结果
        ILOCFormatter formatter;
        
        // 输出到控制台并写入console输出文件
        for (const auto& inst : result) {
            std::string formattedInst = formatter.format(inst);
            std::cout << formattedInst << std::endl;
            consoleFile << formattedInst << std::endl;
        }
        
        // 写入带行号的输出文件
        for (const auto& inst : result) {
            std::string formattedInst = formatter.format(inst);
            if (inst.lineNumber > 0) {
                numberedFile << inst.lineNumber << ":\t" << formattedInst << std::endl;
            } else {
                numberedFile << "\t" << formattedInst << std::endl;
            }
        }

        numberedFile.close();
        consoleFile.close();
        std::cout << "\nNumbered output has been written to: " << numberedOutputFile << std::endl;
        std::cout << "Console output has been written to: " << consoleOutputFile << std::endl;

        return 0;
    } catch (const ILOCParseException& e) {
        std::cerr << "Error parsing ILOC instruction at line " << e.getLine() << std::endl;
        std::cerr << "Details: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error occurred" << std::endl;
        std::cerr << "Details: " << e.what() << std::endl;
        return 1;
    }
}
