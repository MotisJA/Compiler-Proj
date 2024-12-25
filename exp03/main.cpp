#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

// ILOC指令结构
struct ILOCInstruction {
    std::string op;
    std::string src1, src2, dest;
    int immediate;
};

// 活跃变量分析结果
struct LivenessInfo {
    std::set<std::string> liveIn;
    std::set<std::string> liveOut;
    std::set<std::string> def;
    std::set<std::string> use;
};

// 解析ILOC指令
ILOCInstruction parseInstruction(const std::string& line) {
    // 实现指令解析逻辑
    // ...
}

// 执行活跃变量分析
std::vector<LivenessInfo> performLivenessAnalysis(const std::vector<ILOCInstruction>& instructions) {
    // 实现活跃变量分析
    // ...
}

// 自顶向下寄存器分配
std::vector<ILOCInstruction> topDownAllocation(const std::vector<ILOCInstruction>& instructions, int k) {
    // 实现自顶向下分配
    // ...
}

// 自底向上寄存器分配
std::vector<ILOCInstruction> bottomUpAllocation(const std::vector<ILOCInstruction>& instructions, int k) {
    // 实现自底向上分配
    // ...
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <k> <t|b> <input_file>" << std::endl;
        return 1;
    }

    // 解析参数
    int k = std::stoi(argv[1]);
    char method = argv[2][0];
    std::string inputFile = argv[3];

    if (k <= 0) {
        std::cerr << "Error: k must be a positive integer" << std::endl;
        return 1;
    }

    if (method != 't' && method != 'b') {
        std::cerr << "Error: second argument must be 't' or 'b'" << std::endl;
        return 1;
    }

    // 读取输入文件
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error: cannot open input file " << inputFile << std::endl;
        return 1;
    }

    std::vector<ILOCInstruction> instructions;
    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            instructions.push_back(parseInstruction(line));
        }
    }

    // 根据选择的方法进行寄存器分配
    std::vector<ILOCInstruction> result;
    if (method == 't') {
        result = topDownAllocation(instructions, k);
    } else {
        result = bottomUpAllocation(instructions, k);
    }

    // 输出结果
    for (const auto& inst : result) {
        // 输出格式化的ILOC指令
        // ...
    }

    return 0;
}
