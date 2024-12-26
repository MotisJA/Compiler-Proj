#include "RegisterAllocator.h"
#include <map>
#include <vector>
#include <algorithm>
#include <set>

std::vector<ILOCInstruction> TopDownAllocator::allocate(
    const std::vector<ILOCInstruction>& instructions,
    int k) {
    
    // 统计虚拟寄存器使用频率
    std::map<std::string, int> registerFrequency;
    std::set<std::string> virtualRegisters;
    
    for (const auto& inst : instructions) {
        if (!inst.src1.empty()) {
            registerFrequency[inst.src1]++;
            virtualRegisters.insert(inst.src1);
        }
        if (!inst.src2.empty()) {
            registerFrequency[inst.src2]++;
            virtualRegisters.insert(inst.src2);
        }
        if (!inst.dest.empty()) {
            registerFrequency[inst.dest]++;
            virtualRegisters.insert(inst.dest);
        }
    }

    // 如果虚拟寄存器数量小于k，直接使用原始寄存器
    if (virtualRegisters.size() <= static_cast<size_t>(k)) {
        return instructions;
    }

    // 将虚拟寄存器按使用频率排序
    std::vector<std::pair<std::string, int>> sortedRegs;
    for (const auto& reg : registerFrequency) {
        sortedRegs.push_back(reg);
    }
    std::sort(sortedRegs.begin(), sortedRegs.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    // 分配物理寄存器
    const int F = 1; // 保留一个寄存器用于溢出
    std::map<std::string, std::string> registerMapping;
    for (size_t i = 0; i < sortedRegs.size() && i < static_cast<size_t>(k - F); ++i) {
        registerMapping[sortedRegs[i].first] = "r" + std::to_string(i);
    }

    // 重写代码
    std::vector<ILOCInstruction> result;
    const std::string spillReg = "r" + std::to_string(k - 1); // 用于溢出的寄存器
    int memoryOffset = 1000; // 内存起始位置
    std::map<std::string, int> spillLocations;

    for (const auto& inst : instructions) {
        ILOCInstruction newInst = inst;

        // 处理源操作数1
        if (!inst.src1.empty()) {
            if (registerMapping.count(inst.src1)) {
                newInst.src1 = registerMapping[inst.src1];
            } else {
                // 需要从内存加载
                if (!spillLocations.count(inst.src1)) {
                    spillLocations[inst.src1] = memoryOffset++;
                }
                ILOCInstruction loadInst;
                loadInst.opcode = ILOCOpcode::LOADI;
                loadInst.immediate = spillLocations[inst.src1];
                loadInst.dest = spillReg;
                result.push_back(loadInst);

                loadInst.opcode = ILOCOpcode::LOAD;
                loadInst.src1 = spillReg;
                loadInst.dest = spillReg;
                result.push_back(loadInst);

                newInst.src1 = spillReg;
            }
        }

        // 处理源操作数2
        if (!inst.src2.empty()) {
            if (registerMapping.count(inst.src2)) {
                newInst.src2 = registerMapping[inst.src2];
            } else {
                // 需要从内存加载
                if (!spillLocations.count(inst.src2)) {
                    spillLocations[inst.src2] = memoryOffset++;
                }
                ILOCInstruction loadInst;
                loadInst.opcode = ILOCOpcode::LOADI;
                loadInst.immediate = spillLocations[inst.src2];
                loadInst.dest = spillReg;
                result.push_back(loadInst);

                loadInst.opcode = ILOCOpcode::LOAD;
                loadInst.src1 = spillReg;
                loadInst.dest = spillReg;
                result.push_back(loadInst);

                newInst.src2 = spillReg;
            }
        }

        // 处理目标操作数
        if (!inst.dest.empty()) {
            if (registerMapping.count(inst.dest)) {
                newInst.dest = registerMapping[inst.dest];
            } else {
                // 将结果存储到内存
                if (!spillLocations.count(inst.dest)) {
                    spillLocations[inst.dest] = memoryOffset++;
                }
                newInst.dest = spillReg;
                result.push_back(newInst);

                ILOCInstruction storeInst;
                storeInst.opcode = ILOCOpcode::LOADI;
                storeInst.immediate = spillLocations[inst.dest];
                storeInst.dest = spillReg;
                result.push_back(storeInst);

                storeInst.opcode = ILOCOpcode::STORE;
                storeInst.src1 = newInst.dest;
                storeInst.dest = spillReg;
                result.push_back(storeInst);
                continue;
            }
        }

        result.push_back(newInst);
    }

    return result;
}
