#pragma once
#include "ILOCTypes.h"
#include <string>
#include <vector>
#include <regex>

class ILOCParser {
public:
    static std::vector<ILOCInstruction> parseFile(const std::string& filename);
    static ILOCInstruction parseLine(const std::string& line, int lineNumber = 0);

private:
    static std::string trim(const std::string& str);
    static bool isValidRegister(const std::string& reg);
    static int parseRegisterNumber(const std::string& reg);
    static std::string normalizeRegister(const std::string& reg);
};
