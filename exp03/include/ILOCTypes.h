#pragma once
#include <string>
#include <vector>
#include <set>
#include <stdexcept>

// ILOC操作码枚举
enum class ILOCOpcode {
    LOAD,
    LOADI,
    STORE,
    ADD,
    SUB,
    MULT,
    LSHIFT,
    RSHIFT,
    OUTPUT
};

// ILOC指令结构
struct ILOCInstruction {
    ILOCOpcode opcode;
    std::string src1, src2, dest;
    int immediate;
    int lineNumber;  // 用于错误报告

    ILOCInstruction() : opcode(ILOCOpcode::LOAD), immediate(0), lineNumber(0) {}
};

// ILOC解析异常
class ILOCParseException : public std::runtime_error {
public:
    ILOCParseException(const std::string& msg, int line) 
        : std::runtime_error("Line " + std::to_string(line) + ": " + msg) {}
};

struct LivenessInfo {
    std::set<std::string> liveIn;
    std::set<std::string> liveOut;
    std::set<std::string> def;
    std::set<std::string> use;
};
