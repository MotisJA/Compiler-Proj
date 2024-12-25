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
    return std::regex_match(reg, std::regex("r[0-9]+"));
}

int ILOCParser::parseRegisterNumber(const std::string& reg) {
    if (!isValidRegister(reg)) {
        throw ILOCParseException("Invalid register name: " + reg, 0);
    }
    return std::stoi(reg.substr(1));
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

    // 定义所有指令的正则表达式
    const std::string regPat = "r[0-9]+";
    const std::string immPat = "[0-9]+";
    
    std::map<std::string, std::regex> patterns = {
        {"loadI", std::regex("loadI\\s+(" + immPat + ")\\s*=>\\s*(" + regPat + ")")},
        {"load", std::regex("load\\s+(" + regPat + ")\\s*=>\\s*(" + regPat + ")")},
        {"store", std::regex("store\\s+(" + regPat + ")\\s*=>\\s*(" + regPat + ")")},
        {"arith", std::regex("(add|sub|mult|lshift|rshift)\\s+(" + regPat + ")\\s*,\\s*(" + regPat + ")\\s*=>\\s*(" + regPat + ")")},
        {"output", std::regex("output\\s+(" + immPat + ")")}
    };

    std::string trimmedLine = trim(line);
    std::smatch matches;

    // 尝试匹配每种指令模式
    for (const auto& pattern : patterns) {
        if (std::regex_match(trimmedLine, matches, pattern.second)) {
            if (pattern.first == "loadI") {
                inst.opcode = ILOCOpcode::LOADI;
                inst.immediate = std::stoi(matches[1]);
                inst.dest = normalizeRegister(matches[2]);
            }
            else if (pattern.first == "load") {
                inst.opcode = ILOCOpcode::LOAD;
                inst.src1 = normalizeRegister(matches[1]);
                inst.dest = normalizeRegister(matches[2]);
            }
            else if (pattern.first == "store") {
                inst.opcode = ILOCOpcode::STORE;
                inst.src1 = normalizeRegister(matches[1]);
                inst.dest = normalizeRegister(matches[2]);
            }
            else if (pattern.first == "arith") {
                std::string op = matches[1];
                if (op == "add") inst.opcode = ILOCOpcode::ADD;
                else if (op == "sub") inst.opcode = ILOCOpcode::SUB;
                else if (op == "mult") inst.opcode = ILOCOpcode::MULT;
                else if (op == "lshift") inst.opcode = ILOCOpcode::LSHIFT;
                else if (op == "rshift") inst.opcode = ILOCOpcode::RSHIFT;

                inst.src1 = normalizeRegister(matches[2]);
                inst.src2 = normalizeRegister(matches[3]);
                inst.dest = normalizeRegister(matches[4]);
            }
            else if (pattern.first == "output") {
                inst.opcode = ILOCOpcode::OUTPUT;
                inst.immediate = std::stoi(matches[1]);
            }
            return inst;
        }
    }

    throw ILOCParseException("Invalid instruction format: " + line, lineNumber);
}
