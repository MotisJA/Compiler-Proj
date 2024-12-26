#include <iostream>
#include <string>
#include <vector>
#include "ILOCParser.h"
#include "RegisterAllocator.h"
#include "ILOCFormatter.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <k> <t|b> <input_file>" << std::endl;
        return 1;
    }

    // 解析参数
    int k;
    try {
        k = std::stoi(argv[1]);
        if (k <= 0) {
            throw std::invalid_argument("k must be positive");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid value for k. Must be a positive integer." << std::endl;
        return 1;
    }

    char method = argv[2][0];
    if (method != 't' && method != 'b') {
        std::cerr << "Error: Second argument must be 't' (top-down) or 'b' (bottom-up)" << std::endl;
        return 1;
    }

    std::string inputFile = argv[3];

    try {
        // 解析ILOC指令
        ILOCParser parser;
        auto instructions = parser.parseFile(inputFile);

        // 选择并执行寄存器分配策略
        std::vector<ILOCInstruction> result;
        if (method == 't') {
            TopDownAllocator allocator;
            result = allocator.allocate(instructions, k);
        } else {
            BottomUpAllocator allocator;
            result = allocator.allocate(instructions, k);
        }

        // 格式化并输出结果
        ILOCFormatter formatter;
        for (const auto& inst : result) {
            std::cout << formatter.format(inst) << std::endl;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
