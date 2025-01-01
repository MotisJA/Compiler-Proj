#pragma once
#include "ILOCTypes.h"
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <limits>

// 寄存器信息结构
struct RegInfo {
    std::string name;      // 物理寄存器名称
    int nextUse;          // 下一次使用位置
    bool isFree;          // 是否空闲
    
    RegInfo() : nextUse(std::numeric_limits<int>::max()), isFree(true) {}
};

// 虚拟寄存器使用信息
struct VRUsage {
    std::vector<size_t> usePositions;  // 使用位置列表
    size_t currentUse;                 // 当前使用位置索引
    
    VRUsage() : currentUse(0) {}
};

// 改进的寄存器类
class RegClass {
public:
    std::vector<RegInfo> regs;                    // 物理寄存器信息
    std::stack<int> freeRegs;                     // 改用stack存储空闲寄存器
    std::map<std::string, std::string> virtualToPhysical; // 虚拟到物理寄存器映射
    
    explicit RegClass(int k = 0);
    int getFreeReg();                             // 获取空闲寄存器
    void freeReg(int regIdx);                     // 释放寄存器
    void updateNextUse(int regIdx, int next);     // 更新下次使用位置
    int getFurthestReg() const;                   // 获取最远使用的寄存器
};

class RegisterAllocator {
public:
    virtual std::vector<ILOCInstruction> allocate(
        const std::vector<ILOCInstruction>& instructions,
        int k) = 0;
    virtual ~RegisterAllocator() = default;
};

class TopDownAllocator : public RegisterAllocator {
public:
    std::vector<ILOCInstruction> allocate(
        const std::vector<ILOCInstruction>& instructions,
        int k) override;
};

class BottomUpAllocator : public RegisterAllocator {
private:
    RegClass regClass;
    int memOffset;
    std::map<std::string, int> memoryLoc;
    std::string spillReg;
    std::map<std::string, VRUsage> vrUsages;

protected:
    void validateK(int k);
    void computeVRUsages(const std::vector<ILOCInstruction>& instructions);
    int getNextUseDistance(const std::string& vr, size_t currentPos);
    std::string ensure(const std::string& vr, std::vector<ILOCInstruction>& result, 
                      size_t currentPos);
    std::string ensure(const std::string& vr, 
                      std::vector<ILOCInstruction>& result,
                      size_t currentPos,
                      const ILOCInstruction& inst);
    std::string allocate(const std::string& vr, std::vector<ILOCInstruction>& result, 
                        size_t currentPos, size_t lineNum);
    void free(const std::string& reg);
    void spillToMemory(const std::string& vr, const std::string& physReg, 
                      std::vector<ILOCInstruction>& result, size_t currentPos);

public:
    BottomUpAllocator() : memOffset(1000) {}
    
    std::vector<ILOCInstruction> allocate(
        const std::vector<ILOCInstruction>& instructions,
        int k) override;
};
