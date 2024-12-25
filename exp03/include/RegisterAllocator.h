#pragma once
#include "ILOCTypes.h"
#include <vector>

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
public:
    std::vector<ILOCInstruction> allocate(
        const std::vector<ILOCInstruction>& instructions,
        int k) override;
};
