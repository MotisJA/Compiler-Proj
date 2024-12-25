#pragma once
#include "ILOCTypes.h"
#include <vector>

class LivenessAnalyzer {
public:
    static std::vector<LivenessInfo> analyze(const std::vector<ILOCInstruction>& instructions);
};
