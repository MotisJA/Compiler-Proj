#include "RegisterAllocator.h"
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>

std::vector<ILOCInstruction> BottomUpAllocator::allocate(
    const std::vector<ILOCInstruction>& instructions,
    int k) {
    
    // 初始化数据结构
    std::map<std::string, int> nextUse;  // 下一次使用的位置
    std::map<std::string, std::string> regMapping;  // 虚拟寄存器到物理寄存器的映射
    std::set<std::string> freeRegs;  // 可用的物理寄存器
    std::map<std::string, int> memoryLoc;  // 溢出位置
    int memOffset = 1000;
    
    // 初始化可用物理寄存器
    for (int i = 0; i < k - 1; ++i) { // 保留一个寄存器用于临时操作
        freeRegs.insert("r" + std::to_string(i));
    }
    const std::string spillReg = "r" + std::to_string(k - 1);
    
    // 计算每个变量的下一次使用位置
    for (int i = instructions.size() - 1; i >= 0; --i) {
        const auto& inst = instructions[i];
        
        if (!inst.dest.empty()) {
            if (nextUse.find(inst.dest) == nextUse.end()) {
                nextUse[inst.dest] = i;
            }
        }
        if (!inst.src2.empty()) {
            nextUse[inst.src2] = i;
        }
        if (!inst.src1.empty()) {
            nextUse[inst.src1] = i;
        }
    }
    
    std::vector<ILOCInstruction> result;
    
    // 处理每条指令
    for (size_t i = 0; i < instructions.size(); ++i) {
        auto inst = instructions[i];
        
        // 处理源操作数
        if (!inst.src1.empty()) {
            if (regMapping.find(inst.src1) == regMapping.end()) {
                // 需要分配寄存器
                std::string allocReg;
                if (!freeRegs.empty()) {
                    allocReg = *freeRegs.begin();
                    freeRegs.erase(freeRegs.begin());
                } else {
                    // 寻找最晚使用的寄存器
                    int maxNext = -1;
                    std::string victimReg;
                    for (const auto& mapping : regMapping) {
                        int next = nextUse[mapping.first];
                        if (next > maxNext) {
                            maxNext = next;
                            victimReg = mapping.second;
                        }
                    }
                    allocReg = victimReg;
                    
                    // 溢出到内存
                    for (auto& pair : regMapping) {
                        if (pair.second == allocReg) {
                            if (memoryLoc.find(pair.first) == memoryLoc.end()) {
                                memoryLoc[pair.first] = memOffset++;
                            }
                            // 生成存储指令
                            ILOCInstruction spill;
                            spill.opcode = ILOCOpcode::LOADI;
                            spill.immediate = memoryLoc[pair.first];
                            spill.dest = spillReg;
                            result.push_back(spill);
                            
                            spill.opcode = ILOCOpcode::STORE;
                            spill.src1 = allocReg;
                            spill.dest = spillReg;
                            result.push_back(spill);
                            break;
                        }
                    }
                }
                regMapping[inst.src1] = allocReg;
                
                // 从内存加载
                if (memoryLoc.find(inst.src1) != memoryLoc.end()) {
                    ILOCInstruction load;
                    load.opcode = ILOCOpcode::LOADI;
                    load.immediate = memoryLoc[inst.src1];
                    load.dest = spillReg;
                    result.push_back(load);
                    
                    load.opcode = ILOCOpcode::LOAD;
                    load.src1 = spillReg;
                    load.dest = allocReg;
                    result.push_back(load);
                }
            }
            inst.src1 = regMapping[inst.src1];
        }
        
        // 处理第二个源操作数
        // ... (类似src1的处理逻辑)
        
        // 处理目标操作数
        if (!inst.dest.empty()) {
            if (regMapping.find(inst.dest) == regMapping.end()) {
                std::string allocReg;
                if (!freeRegs.empty()) {
                    allocReg = *freeRegs.begin();
                    freeRegs.erase(freeRegs.begin());
                } else {
                    // 选择牺牲寄存器
                    int maxNext = -1;
                    std::string victimReg;
                    for (const auto& mapping : regMapping) {
                        int next = nextUse[mapping.first];
                        if (next > maxNext) {
                            maxNext = next;
                            victimReg = mapping.second;
                        }
                    }
                    allocReg = victimReg;
                }
                regMapping[inst.dest] = allocReg;
            }
            inst.dest = regMapping[inst.dest];
        }
        
        result.push_back(inst);
        
        // 更新下一次使用信息和释放不再需要的寄存器
        for (auto it = nextUse.begin(); it != nextUse.end(); ) {
            if (it->second == i) {
                std::string reg = regMapping[it->first];
                regMapping.erase(it->first);
                freeRegs.insert(reg);
                it = nextUse.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    return result;
}
