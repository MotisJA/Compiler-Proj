#include "ILOCParser.h"
#include <fstream>
#include <sstream>
#include <regex>

std::string ILOCParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

bool ILOCParser::isValidRegister(const std::string& reg) {
    // 修改正则表达式以匹配包含前导零的寄存器
    return std::regex_match(reg, std::regex("r0*[0-9]+"));
}

int ILOCParser::parseRegisterNumber(const std::string& reg) {
    if (!isValidRegister(reg)) {
        throw ILOCParseException("Invalid register name: " + reg, 0);
    }
    
    // 去掉r后的数字部分
    std::string numStr = reg.substr(1);
    
    // 只有当长度大于1且全是0时才是无效的
    bool allZeros = true;
    for (char c : numStr) {
        if (c != '0') {
            allZeros = false;
            break;
        }
    }
    
    if (allZeros) {
        throw ILOCParseException("Register r0 is not allowed: " + reg, 0);
    }
    
    // 去掉前导零，但保留最后一个0
    while (numStr.length() > 1 && numStr[0] == '0') {
        numStr = numStr.substr(1);
    }
    
    int num = std::stoi(numStr);
    if (num <= 0) {
        throw ILOCParseException("Register r0 and negative numbered registers are not allowed: " + reg, 0);
    }
    return num;
}

std::string ILOCParser::normalizeRegister(const std::string& reg) {
    return "r" + std::to_string(parseRegisterNumber(reg));
}

std::vector<ILOCInstruction> ILOCParser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<ILOCInstruction> instructions;
    std::string line;
    int lineNumber = 1;

    while (std::getline(file, line)) {
        line = trim(line);
        if (!line.empty()) {
            try {
                instructions.push_back(parseLine(line, lineNumber));
            } catch (const ILOCParseException& e) {
                throw; // 重新抛出异常
            }
        }
        lineNumber++;
    }

    return instructions;
}

ILOCInstruction ILOCParser::parseLine(const std::string& line, int lineNumber) {
    ILOCInstruction inst;
    inst.lineNumber = lineNumber;

    std::string trimmedLine = trim(line);
    if (trimmedLine.empty()) {
        throw ILOCParseException("Empty line", lineNumber);
    }

    // 更新正则表达式模式以匹配规范中定义的格式
    const std::string regPat = "r[0-9]+";
    const std::string immPat = "-?[0-9]+";
    const std::string ws = "\\s*";

    std::vector<std::pair<std::regex, std::function<void(const std::smatch&)>>> patterns = {
        // loadI x => r2
        {std::regex("loadI" + ws + "(" + immPat + ")" + ws + "=>" + ws + "(" + regPat + ")"),
         [&](const std::smatch& m) {
             inst.opcode = ILOCOpcode::LOADI;
             inst.immediate = std::stoi(m[1]);
             inst.dest = normalizeRegister(m[2]);
         }},
        
        // load r1 => r2
        {std::regex("load" + ws + "(" + regPat + ")" + ws + "=>" + ws + "(" + regPat + ")"),
         [&](const std::smatch& m) {
             inst.opcode = ILOCOpcode::LOAD;
             inst.src1 = normalizeRegister(m[1]);
             inst.dest = normalizeRegister(m[2]);
         }},
         
        // store r1 => r2
        {std::regex("store" + ws + "(" + regPat + ")" + ws + "=>" + ws + "(" + regPat + ")"),
         [&](const std::smatch& m) {
             inst.opcode = ILOCOpcode::STORE;
             inst.src1 = normalizeRegister(m[1]);
             inst.dest = normalizeRegister(m[2]);
         }},
         
        // 算术运算: add, sub, mult, lshift, rshift
        {std::regex("(add|sub|mult|lshift|rshift)" + ws + 
                   "(" + regPat + ")" + ws + "," + ws + 
                   "(" + regPat + ")" + ws + "=>" + ws + 
                   "(" + regPat + ")"),
         [&](const std::smatch& m) {
             std::string op = m[1];
             if (op == "add") inst.opcode = ILOCOpcode::ADD;
             else if (op == "sub") inst.opcode = ILOCOpcode::SUB;
             else if (op == "mult") inst.opcode = ILOCOpcode::MULT;
             else if (op == "lshift") inst.opcode = ILOCOpcode::LSHIFT;
             else if (op == "rshift") inst.opcode = ILOCOpcode::RSHIFT;
             inst.src1 = normalizeRegister(m[2]);
             inst.src2 = normalizeRegister(m[3]);
             inst.dest = normalizeRegister(m[4]);
         }},
         
        // output x
        {std::regex("output" + ws + "(" + immPat + ")"),
         [&](const std::smatch& m) {
             inst.opcode = ILOCOpcode::OUTPUT;
             inst.immediate = std::stoi(m[1]);
         }}
    };

    // 尝试匹配每个模式
    for (const auto& pattern_pair : patterns) {
        std::smatch matches;
        if (std::regex_match(trimmedLine, matches, pattern_pair.first)) {
            try {
                pattern_pair.second(matches);
                return inst;
            } catch (const std::exception& e) {
                throw ILOCParseException(
                    "Error processing instruction: " + trimmedLine + "\n" + e.what(), 
                    lineNumber
                );
            }
        }
    }

    throw ILOCParseException("Invalid instruction format: " + trimmedLine, lineNumber);
}
