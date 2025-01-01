#pragma once
#include "ILOCTypes.h"
#include <vector>
#include <map>
#include <set>

// 表示寄存器类的结构
class RegClass {
public:
    int size;
    std::vector<std::string> names;     
    std::map<std::string, int> next;    
    std::set<std::string> freeRegs;     
    std::map<std::string, std::string> virtualToPhysical;  
    
    explicit RegClass(int k) : size(k) {
        for (int i = 1; i <= k; i++) {
            std::string reg = "r" + std::to_string(i);
            names.push_back(reg);
            freeRegs.insert(reg);
        }
    }
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

protected:
    std::string ensure(const std::string& vr, std::vector<ILOCInstruction>& result);
    std::string allocate(const std::string& vr, std::vector<ILOCInstruction>& result);
    void free(const std::string& reg);
    void spillToMemory(const std::string& vr, const std::string& physReg, 
                      std::vector<ILOCInstruction>& result);

public:
    BottomUpAllocator() : regClass(0), memOffset(1000), spillReg("r0") {}
    std::vector<ILOCInstruction> allocate(
        const std::vector<ILOCInstruction>& instructions,
        int k) override;
};
