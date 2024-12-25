#pragma once
#include "ILOCTypes.h"
#include <string>

class ILOCFormatter {
public:
    static std::string format(const ILOCInstruction& inst);
};
