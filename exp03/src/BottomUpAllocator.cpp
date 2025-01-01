#include "RegisterAllocator.h"
#include <limits>
#include <algorithm>
#include <stdexcept>

// RegClass实现
RegClass::RegClass(int k) {
    regs.resize(k);
    // 初始化物理寄存器
    for (int i = 0; i < k; i++) {
        regs[i].name = "r" + std::to_string(i + 1);
        freeRegs.push(i);  // 直接压入索引
    }
}

int RegClass::getFreeReg() {
    if (freeRegs.empty()) return -1;
    int reg = freeRegs.top();
    freeRegs.pop();
    regs[reg].isFree = false;
    return reg;
}

void RegClass::freeReg(int regIdx) {
    if (regIdx >= 0 && regIdx < static_cast<int>(regs.size())) {
        regs[regIdx].isFree = true;
        regs[regIdx].nextUse = std::numeric_limits<int>::max();
        freeRegs.push(regIdx);
    }
}

void RegClass::updateNextUse(int regIdx, int next) {
    if (regIdx >= 0 && regIdx < static_cast<int>(regs.size())) {
        regs[regIdx].nextUse = next;
    }
}

int RegClass::getFurthestReg() const {
    int maxDist = -1;
    int result = -1;
    for (size_t i = 0; i < regs.size(); i++) {
        if (!regs[i].isFree && regs[i].nextUse > maxDist) {
            maxDist = regs[i].nextUse;
            result = i;
        }
    }
    return result;
}

void BottomUpAllocator::validateK(int k) {
    if (k <= 2) { // 至少需要2个寄存器:1个用于计算,1个用于溢出
        throw std::invalid_argument("Number of registers must be greater than 2");
    }
}

void BottomUpAllocator::computeVRUsages(const std::vector<ILOCInstruction>& instructions) {
    vrUsages.clear();
    for (size_t i = 0; i < instructions.size(); ++i) {
        const auto& inst = instructions[i];
        if (!inst.src1.empty()) vrUsages[inst.src1].usePositions.push_back(i);
        if (!inst.src2.empty()) vrUsages[inst.src2].usePositions.push_back(i);
        if (!inst.dest.empty()) vrUsages[inst.dest].usePositions.push_back(i);
    }
    
    for (auto it = vrUsages.begin(); it != vrUsages.end(); ++it) {
        std::sort(it->second.usePositions.begin(), it->second.usePositions.end());
    }
}

int BottomUpAllocator::getNextUseDistance(const std::string& vr, size_t currentPos) {
    auto it = vrUsages.find(vr);
    if (it == vrUsages.end()) return std::numeric_limits<int>::max();
    
    auto& usage = it->second;
    auto pos = std::upper_bound(usage.usePositions.begin(), 
                              usage.usePositions.end(), 
                              currentPos);
    
    return pos == usage.usePositions.end() ? 
           std::numeric_limits<int>::max() : 
           static_cast<int>(*pos - currentPos);
}

std::string BottomUpAllocator::ensure(const std::string& vr, 
                                    std::vector<ILOCInstruction>& result,
                                    size_t currentPos) {
    // 如果已经在物理寄存器中,直接返回
    if (regClass.virtualToPhysical.find(vr) != regClass.virtualToPhysical.end()) {
        return regClass.virtualToPhysical[vr];
    }

    // 分配物理寄存器并可能需要从内存加载
    std::string physReg = allocate(vr, result, currentPos);
    
    // 如果需要从内存加载
    if (memoryLoc.find(vr) != memoryLoc.end()) {
        int currentLine = result.empty() ? 1 : result.back().lineNumber + 1;
        
        // 生成加载地址指令
        ILOCInstruction loadAddr;
        loadAddr.opcode = ILOCOpcode::LOADI;
        loadAddr.immediate = memoryLoc[vr];
        loadAddr.dest = physReg;
        loadAddr.lineNumber = currentLine;
        result.push_back(loadAddr);

        // 生成加载值指令
        ILOCInstruction loadVal;
        loadVal.opcode = ILOCOpcode::LOAD;
        loadVal.src1 = physReg;
        loadVal.dest = physReg;
        loadVal.lineNumber = currentLine;
        result.push_back(loadVal);
    }

    return physReg;
}

std::string BottomUpAllocator::allocate(const std::string& vr, 
                                      std::vector<ILOCInstruction>& result,
                                      size_t currentPos) {
    std::string allocReg;
    
    // 如果有空闲寄存器,直接分配
    if (!regClass.freeRegs.empty()) {
        int regIdx = regClass.freeRegs.top();
        // int regIdx = regClass.getFreeReg();
        allocReg = "r" + std::to_string(regIdx + 1);
        regClass.freeRegs.pop();
    } else {
        // 找到最远使用的寄存器
        int maxDist = -1;
        std::string victimVR;
        
        for (auto it = regClass.virtualToPhysical.begin(); 
             it != regClass.virtualToPhysical.end(); ++it) {
            const std::string& v = it->first;
            const std::string& p = it->second;
            int dist = getNextUseDistance(v, currentPos);
            if (dist > maxDist) {
                maxDist = dist;
                allocReg = p;
                victimVR = v;
            }
        }
        
        // 溢出到内存
        if (!victimVR.empty()) {
            spillToMemory(victimVR, allocReg, result, 
                         result.empty() ? 1 : result.back().lineNumber);
            regClass.virtualToPhysical.erase(victimVR);
        }
    }

    // 更新映射
    regClass.virtualToPhysical[vr] = allocReg;
    
    return allocReg;
}

void BottomUpAllocator::free(const std::string& reg) {
    int regIdx = std::stoi(reg.substr(1)) - 1;  // 从"r1"提取索引
    regClass.freeReg(regIdx);
    
    // 删除所有映射到该物理寄存器的虚拟寄存器
    auto it = regClass.virtualToPhysical.begin();
    while (it != regClass.virtualToPhysical.end()) {
        if (it->second == reg) {
            it = regClass.virtualToPhysical.erase(it);
        } else {
            ++it;
        }
    }
}

void BottomUpAllocator::spillToMemory(const std::string& vr, 
                                    const std::string& physReg,
                                    std::vector<ILOCInstruction>& result,
                                    size_t lineNum) {
    // 分配内存位置(如果还没有)
    if (memoryLoc.find(vr) == memoryLoc.end()) {
        memoryLoc[vr] = memOffset;
        memOffset += 4; // 假设每个变量占用4字节
    }
    
    // 生成存储地址加载指令
    ILOCInstruction loadAddr;
    loadAddr.opcode = ILOCOpcode::LOADI;
    loadAddr.immediate = memoryLoc[vr];
    loadAddr.dest = spillReg;
    loadAddr.lineNumber = lineNum;
    result.push_back(loadAddr);

    // 生成存储值指令
    ILOCInstruction store;
    store.opcode = ILOCOpcode::STORE;
    store.src1 = physReg;
    store.src2 = spillReg;
    store.lineNumber = lineNum;
    result.push_back(store);
}

std::vector<ILOCInstruction> BottomUpAllocator::allocate(
    const std::vector<ILOCInstruction>& instructions,
    int k) {
    validateK(k);
    
    regClass = RegClass(k - 1);  // 预留一个寄存器用于溢出
    spillReg = "r" + std::to_string(k);
    memOffset = 1000;  
    memoryLoc.clear();
    
    computeVRUsages(instructions);
    
    std::vector<ILOCInstruction> result;
    result.reserve(instructions.size() * 2);
    
    for (size_t i = 0; i < instructions.size(); ++i) {
        auto inst = instructions[i];
        
        if (!inst.src1.empty()) {
            inst.src1 = ensure(inst.src1, result, i);
        }
        
        if (!inst.src2.empty()) {
            inst.src2 = ensure(inst.src2, result, i);
        }
        
        if (!inst.dest.empty()) {
            // inst.dest = allocate(inst.dest, result, i);
            inst.dest = ensure(inst.dest, result, i);
        }
        
        result.push_back(inst);
        
        for (const auto& pair : regClass.virtualToPhysical) {
            const std::string& vr = pair.first;
            const std::string& pr = pair.second;
            int nextUse = getNextUseDistance(vr, i);
            int regIdx = std::stoi(pr.substr(1)) - 1;
            regClass.updateNextUse(regIdx, nextUse);
            
            if (nextUse == std::numeric_limits<int>::max()) {
                free(pr);
            }
        }
    }
    
    return result;
}
