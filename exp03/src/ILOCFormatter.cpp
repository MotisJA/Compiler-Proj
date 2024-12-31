#include "ILOCFormatter.h"

std::string ILOCFormatter::format(const ILOCInstruction& inst) {
    // // 验证所有寄存器编号是否合法
    // auto validateReg = [](const std::string& reg) {
    //     if (reg == "r0") {
    //         throw std::runtime_error("Invalid register r0 in output");
    //     }
    // };

    // if (!inst.src1.empty()) validateReg(inst.src1);
    // if (!inst.src2.empty()) validateReg(inst.src2);
    // if (!inst.dest.empty()) validateReg(inst.dest);

    std::string result;
    switch (inst.opcode) {
        case ILOCOpcode::LOAD:
            result = "load " + inst.src1 + " => " + inst.dest;
            break;
        case ILOCOpcode::LOADI:
            result = "loadI " + std::to_string(inst.immediate) + " => " + inst.dest;
            break;
        case ILOCOpcode::STORE:
            result = "store " + inst.src1 + " => " + inst.dest;
            break;
        case ILOCOpcode::ADD:
            result = "add " + inst.src1 + ", " + inst.src2 + " => " + inst.dest;
            break;
        case ILOCOpcode::SUB:
            result = "sub " + inst.src1 + ", " + inst.src2 + " => " + inst.dest;
            break;
        case ILOCOpcode::MULT:
            result = "mult " + inst.src1 + ", " + inst.src2 + " => " + inst.dest;
            break;
        case ILOCOpcode::LSHIFT:
            result = "lshift " + inst.src1 + ", " + inst.src2 + " => " + inst.dest;
            break;
        case ILOCOpcode::RSHIFT:
            result = "rshift " + inst.src1 + ", " + inst.src2 + " => " + inst.dest;
            break;
        case ILOCOpcode::OUTPUT:
            result = "output " + std::to_string(inst.immediate);
            break;
    }
    return result;
}
