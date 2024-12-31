#pragma once
#include <string>
#include <vector>
#include <set>
#include <stdexcept>

// ILOC操作码枚举
enum class ILOCOpcode {
    LOAD,   // load r1 => r2
    LOADI,  // loadI x => r2
    STORE,  // store r1 => r2
    ADD,    // add r1,r2 => r3
    SUB,    // sub r1,r2 => r3
    MULT,   // mult r1,r2 => r3
    LSHIFT, // lshift r1,r2 => r3
    RSHIFT, // rshift r1,r2 => r3
    OUTPUT  // output x
};

// 获取操作码的字符串表示
inline std::string getOpcodeStr(ILOCOpcode op) {
    switch (op) {
        case ILOCOpcode::LOAD: return "load";
        case ILOCOpcode::LOADI: return "loadI";
        case ILOCOpcode::STORE: return "store";
        case ILOCOpcode::ADD: return "add";
        case ILOCOpcode::SUB: return "sub";
        case ILOCOpcode::MULT: return "mult";
        case ILOCOpcode::LSHIFT: return "lshift";
        case ILOCOpcode::RSHIFT: return "rshift";
        case ILOCOpcode::OUTPUT: return "output";
        default: return "unknown";
    }
}

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
private:
    int lineNumber;

public:
    ILOCParseException(const std::string& message, int line) 
        : std::runtime_error(message), lineNumber(line) {}

    int getLine() const { return lineNumber; }
};

struct LivenessInfo {
    std::set<std::string> liveIn;
    std::set<std::string> liveOut;
    std::set<std::string> def;
    std::set<std::string> use;
};
