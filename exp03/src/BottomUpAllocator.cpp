#include "RegisterAllocator.h"
#include <limits>
#include <algorithm>

std::vector<ILOCInstruction> BottomUpAllocator::allocate(
    const std::vector<ILOCInstruction>& instructions,
    int k) {
    // 重新初始化寄存器类
    regClass = RegClass(k);
    spillReg = "r" + std::to_string(k);
    std::vector<ILOCInstruction> result;
    
    // 计算下一次使用位置
    std::map<std::string, int> nextUse;
    for (size_t i = instructions.size(); i-- > 0;) {
        const auto& inst = instructions[i];
        if (!inst.dest.empty()) {
            nextUse[inst.dest] = i;
        }
        if (!inst.src2.empty()) {
            nextUse[inst.src2] = i;
        }
        if (!inst.src1.empty()) {
            nextUse[inst.src1] = i;
        }
    }
    
    // 处理指令
    for (size_t i = 0; i < instructions.size(); ++i) {
        auto inst = instructions[i];
        
        // 处理源操作数
        if (!inst.src1.empty()) {
            inst.src1 = ensure(inst.src1, result);
        }
        if (!inst.src2.empty()) {
            inst.src2 = ensure(inst.src2, result);
        }
        
        // 处理目标操作数
        if (!inst.dest.empty()) {
            inst.dest = allocate(inst.dest, result);
        }
        
        result.push_back(inst);
        
        // 更新下一次使用信息和释放不再需要的寄存器
        for (auto it = nextUse.begin(); it != nextUse.end();) {
            if (static_cast<size_t>(it->second) == i) {
                free(regClass.virtualToPhysical[it->first]);
                it = nextUse.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    return result;
}

std::string BottomUpAllocator::ensure(const std::string& vr, 
                                    std::vector<ILOCInstruction>& result) {
    if (regClass.virtualToPhysical.find(vr) != regClass.virtualToPhysical.end()) {
        return regClass.virtualToPhysical[vr];
    }
    std::string physReg = allocate(vr, result);
    // 如果需要从内存加载
    if (memoryLoc.find(vr) != memoryLoc.end()) {
        ILOCInstruction loadI, load;
        loadI.opcode = ILOCOpcode::LOADI;
        loadI.immediate = memoryLoc[vr];
        loadI.dest = spillReg;
        result.push_back(loadI);

        load.opcode = ILOCOpcode::LOAD;
        load.src1 = spillReg;
        load.dest = physReg;
        result.push_back(load);
    }
    return physReg;
}

std::string BottomUpAllocator::allocate(const std::string& vr, 
                                      std::vector<ILOCInstruction>& result) {
    std::string allocReg;
    if (!regClass.freeRegs.empty()) {
        allocReg = *regClass.freeRegs.begin();
        regClass.freeRegs.erase(regClass.freeRegs.begin());
    } else {
        // 找到最远使用的寄存器
        int maxDist = -1;
        for (const auto& reg : regClass.names) {
            if (regClass.next[reg] > maxDist) {
                maxDist = regClass.next[reg];
                allocReg = reg;
            }
        }
        // 溢出到内存
        for (auto& pair : regClass.virtualToPhysical) {
            if (pair.second == allocReg) {
                spillToMemory(pair.first, allocReg, result);
                regClass.virtualToPhysical.erase(pair.first);
                break;
            }
        }
    }
    regClass.virtualToPhysical[vr] = allocReg;
    regClass.next[allocReg] = -1;
    return allocReg;
}

void BottomUpAllocator::free(const std::string& reg) {
    regClass.freeRegs.insert(reg);
    for (auto it = regClass.virtualToPhysical.begin(); it != regClass.virtualToPhysical.end();) {
        if (it->second == reg) {
            it = regClass.virtualToPhysical.erase(it);
        } else {
            ++it;
        }
    }
}

void BottomUpAllocator::spillToMemory(const std::string& vr, 
                                    const std::string& physReg,
                                    std::vector<ILOCInstruction>& result) {
    if (memoryLoc.find(vr) == memoryLoc.end()) {
        memoryLoc[vr] = memOffset++;
    }
    
    ILOCInstruction loadI, store;
    loadI.opcode = ILOCOpcode::LOADI;
    loadI.immediate = memoryLoc[vr];
    loadI.dest = spillReg;
    result.push_back(loadI);

    store.opcode = ILOCOpcode::STORE;
    store.src1 = physReg;
    store.dest = spillReg;
    result.push_back(store);
}
