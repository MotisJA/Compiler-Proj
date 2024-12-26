#include "ILOCFormatter.h"

std::string ILOCFormatter::format(const ILOCInstruction& inst) {
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
