#include "RegisterAllocator.h"
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <stdexcept>
#include <string>

std::vector<ILOCInstruction> TopDownAllocator::allocate(
    const std::vector<ILOCInstruction>& instructions,
    int k) {
    
    if (k <= 1) { // 至少需要1个物理寄存器和1个溢出寄存器
        throw std::invalid_argument("Number of registers must be greater than 1");
    }

    if (instructions.empty()) {
        return instructions;
    }

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

    // 安全检查：如果没有需要分配的寄存器，直接返回
    if (virtualRegisters.empty()) {
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

    // 保留寄存器固定为2个
    const int F = 2;
    std::map<std::string, std::string> registerMapping;
    std::vector<std::string> spillRegs;

    if (virtualRegisters.size() < static_cast<size_t>(k)) {
        // 虚拟寄存器数量小于k时，直接一对一映射
        size_t regIndex = 1;
        for (const auto& vReg : virtualRegisters) {
            registerMapping[vReg] = "r" + std::to_string(regIndex++);
        }
    } else {
        // 需要使用溢出策略，将使用频率最高的k-F个虚拟寄存器分配到物理寄存器
        for (size_t i = 0; i < sortedRegs.size() && i < static_cast<size_t>(k - F); ++i) {
            registerMapping[sortedRegs[i].first] = "r" + std::to_string(i + 1);
        }
        
        // 初始化溢出寄存器
        for (int i = 0; i < F; ++i) {
            spillRegs.push_back("r" + std::to_string(k - F + i + 1));
        }
    }

    // 重写指令序列
    std::vector<ILOCInstruction> result;
    result.reserve(instructions.size() * 3); // 预分配足够空间
    
    int memoryOffset = 1000;
    std::map<std::string, int> spillLocations;
    std::map<std::string, std::string> currentSpillReg;

    // 选择溢出寄存器的索引循环使用
    int spillRegIndex = 0;

    // 处理每条指令
    for (const auto& inst : instructions) {
        ILOCInstruction newInst;
        newInst.opcode = inst.opcode;  // 直接使用枚举类型
        newInst.immediate = inst.immediate;
        newInst.lineNumber = inst.lineNumber;
        bool needStore = false;

        // 处理源操作数
        auto handleOperand = [&](const std::string& vreg) -> std::string {
            if (vreg.empty()) return "";
            
            if (registerMapping.count(vreg)) {
                return registerMapping[vreg];
            }

            // 分配内存位置
            if (!spillLocations.count(vreg)) {
                spillLocations[vreg] = memoryOffset;
                memoryOffset += 4; // 每个变量占用4字节
            }

            // 选择溢出寄存器
            std::string spillReg = spillRegs[spillRegIndex];
            spillRegIndex = (spillRegIndex + 1) % spillRegs.size();

            // 生成加载指令
            ILOCInstruction loadAddr;
            loadAddr.opcode = ILOCOpcode::LOADI;
            loadAddr.immediate = spillLocations[vreg];
            loadAddr.dest = spillReg;
            loadAddr.src1 = "";
            loadAddr.src2 = "";
            loadAddr.lineNumber = inst.lineNumber;
            result.push_back(loadAddr);

            ILOCInstruction loadVal;
            loadVal.opcode = ILOCOpcode::LOAD;
            loadVal.src1 = spillReg;
            loadVal.dest = spillReg;
            // loadVal.immediate = 0;
            loadVal.src2 = "";
            loadVal.lineNumber = inst.lineNumber;
            result.push_back(loadVal);

            currentSpillReg[vreg] = spillReg;
            return spillReg;
        };

        // 处理 src1
        if (!inst.src1.empty()) {
            if (spillLocations.count(inst.src1)) { 
                // 需要从内存加载
                newInst.src1 = handleOperand(inst.src1);
            } else if (registerMapping.count(inst.src1)) {
                // 直接映射到对应的物理寄存器。
                newInst.src1 = registerMapping[inst.src1];
            }
            // 否则，src1 可能是立即数或常量，不需要替换
        }

        // 处理 src2
        if (!inst.src2.empty()) {
            if (spillLocations.count(inst.src2)) {
                newInst.src2 = handleOperand(inst.src2);
            } else if (registerMapping.count(inst.src2)) {
                newInst.src2 = registerMapping[inst.src2];
            }
            // 否则，src2 可能是立即数或常量，不需要替换
        }
        
        // 处理 dest
        if (!inst.dest.empty()) {
            if (registerMapping.count(inst.dest)) {
                newInst.dest = registerMapping[inst.dest];
            } else {
                // 分配内存位置（如果还没有）
                if (!spillLocations.count(inst.dest)) {
                    spillLocations[inst.dest] = memoryOffset;
                    memoryOffset += 4;
                }
                
                // 分配溢出寄存器
                std::string spillReg = spillRegs[spillRegIndex];
                spillRegIndex = (spillRegIndex + 1) % spillRegs.size();
                newInst.dest = spillReg;
                needStore = true;
                currentSpillReg[inst.dest] = spillReg;
            }
        }

        // 添加修改后的指令
        result.push_back(newInst);

        // 处理存储操作
        if (needStore) {
            std::string valueReg = newInst.dest;
            
            // 选择另一个溢出寄存器用于存储地址
            std::string addrReg = spillRegs[spillRegIndex];
            spillRegIndex = (spillRegIndex + 1) % spillRegs.size();
            
            // 生成存储地址指令
            ILOCInstruction storeAddr;
            storeAddr.opcode = ILOCOpcode::LOADI;
            storeAddr.immediate = spillLocations[inst.dest];
            storeAddr.dest = addrReg;  // 使用不同的溢出寄存器
            storeAddr.src1 = "";
            storeAddr.src2 = "";
            storeAddr.lineNumber = inst.lineNumber;
            result.push_back(storeAddr);

            // 生成 STORE 指令
            ILOCInstruction storeVal;
            storeVal.opcode = ILOCOpcode::STORE;
            storeVal.src1 = valueReg;   // 存储的值
            storeVal.dest = addrReg;    // 使用地址寄存器
            // storeVal.immediate = 0;
            storeVal.src2 = "";
            storeVal.lineNumber = inst.lineNumber;
            result.push_back(storeVal);
        }
    }

    return result;
}