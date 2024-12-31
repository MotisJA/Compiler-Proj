#include "RegisterAllocator.h"
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <stdexcept>

std::vector<ILOCInstruction> TopDownAllocator::allocate(
    const std::vector<ILOCInstruction>& instructions,
    int k) {
    
    if (k <= 0) {
        throw std::invalid_argument("Number of registers must be positive");
    }

    if (instructions.empty()) {
        return instructions;
    }

    // 统计虚拟寄存器使用频率
    std::map<std::string, int> registerFrequency;
    std::set<std::string> virtualRegisters;
    
    try {
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
                registerFrequency[inst.dest] += 2;
                virtualRegisters.insert(inst.dest);
            }
        }

        // 安全检查：如果没有需要分配的寄存器，直接返回
        if (virtualRegisters.empty()) {
            return instructions;
        }

        // 如果虚拟寄存器数量小于k，直接返回原始指令
        if (virtualRegisters.size() <= static_cast<size_t>(k)) {
            return instructions;
        }

        // 将寄存器使用频率信息转换为vector并排序
        std::vector<std::pair<std::string, int>> sortedRegs(
            registerFrequency.begin(), 
            registerFrequency.end()
        );
        
        std::sort(sortedRegs.begin(), sortedRegs.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

        // 保留寄存器计算
        const int minReserved = 1;
        const int maxReserved = std::min(3, k - 1); // 确保不会超过可用寄存器数
        int F = std::min(maxReserved, 
                        std::max(minReserved, 
                                static_cast<int>(virtualRegisters.size() * 0.1)));
        
        if (k <= F) {
            F = 1;
        }

        // 创建寄存器映射
        std::map<std::string, std::string> registerMapping;
        for (size_t i = 0; i < sortedRegs.size() && i < static_cast<size_t>(k - F); ++i) {
            std::string physReg = "r" + std::to_string(i + 1);
            registerMapping[sortedRegs[i].first] = physReg;
        }

        // 初始化溢出寄存器列表
        std::vector<std::string> spillRegs;
        for (int i = 0; i < F; ++i) {
            spillRegs.push_back("r" + std::to_string(k - i));
        }

        // 重写指令序列
        std::vector<ILOCInstruction> result;
        result.reserve(instructions.size() * 3); // 预分配足够空间
        
        int memoryOffset = 1000;
        std::map<std::string, int> spillLocations;
        std::map<std::string, std::string> currentSpillReg;

        // 处理每条指令
        for (const auto& inst : instructions) {
            ILOCInstruction newInst = inst;
            bool needStore = false;

            // 处理源操作数
            auto handleOperand = [&](const std::string& vreg) -> std::string {
                if (vreg.empty()) return "";
                
                if (registerMapping.count(vreg)) {
                    return registerMapping[vreg];
                }

                // 选择溢出寄存器
                std::string spillReg = spillRegs[0];
                if (!result.empty() && result.back().dest == spillReg) {
                    spillReg = spillRegs[spillRegs.size() > 1 ? 1 : 0];
                }

                // 分配内存位置
                if (!spillLocations.count(vreg)) {
                    spillLocations[vreg] = memoryOffset++;
                }

                // 生成加载指令
                ILOCInstruction loadAddr;
                loadAddr.opcode = ILOCOpcode::LOADI;
                loadAddr.immediate = spillLocations[vreg];
                loadAddr.dest = spillReg;
                result.push_back(loadAddr);

                ILOCInstruction loadVal;
                loadVal.opcode = ILOCOpcode::LOAD;
                loadVal.src1 = spillReg;
                loadVal.dest = spillReg;
                result.push_back(loadVal);

                currentSpillReg[vreg] = spillReg;
                return spillReg;
            };

            // 处理操作数
            if (!inst.src1.empty()) newInst.src1 = handleOperand(inst.src1);
            if (!inst.src2.empty()) newInst.src2 = handleOperand(inst.src2);
            
            if (!inst.dest.empty()) {
                if (registerMapping.count(inst.dest)) {
                    newInst.dest = registerMapping[inst.dest];
                } else {
                    newInst.dest = handleOperand(inst.dest);
                    needStore = true;
                }
            }

            result.push_back(newInst);

            // 处理存储操作
            if (needStore) {
                std::string spillReg = currentSpillReg[inst.dest];
                
                ILOCInstruction storeAddr;
                storeAddr.opcode = ILOCOpcode::LOADI;
                storeAddr.immediate = spillLocations[inst.dest];
                storeAddr.dest = spillReg;
                result.push_back(storeAddr);

                ILOCInstruction storeVal;
                storeVal.opcode = ILOCOpcode::STORE;
                storeVal.src1 = newInst.dest;
                storeVal.dest = spillReg;
                result.push_back(storeVal);
            }
        }

        return result;

    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Memory allocation failed during register allocation");
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Register allocation failed: ") + e.what());
    }
}
