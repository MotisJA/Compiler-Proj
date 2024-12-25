#pragma once
#include "ILOCTypes.h"
#include "ILOCParser.h"
#include <string>
#include <vector>
#include <functional>

class TestILOCParser {
public:
    static void runAllTests();

private:
    static void testLoadI();
    static void testLoad();
    static void testStore();
    static void testArithmetic();
    static void testOutput();
    static void testInvalidInstructions();
    
    static void assertInstruction(const ILOCInstruction& actual, 
                                ILOCOpcode expectedOp,
                                const std::string& expectedSrc1 = "",
                                const std::string& expectedSrc2 = "",
                                const std::string& expectedDest = "",
                                int expectedImm = 0);
    
    static void runTest(const std::string& testName, 
                       std::function<void()> testFunc);
};
