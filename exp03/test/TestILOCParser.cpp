#include "TestILOCParser.h"
#include <iostream>
#include <cassert>
#include <iomanip>

void TestILOCParser::runAllTests() {
    runTest("LoadI Test", testLoadI);
    runTest("Load Test", testLoad);
    runTest("Store Test", testStore);
    runTest("Arithmetic Test", testArithmetic);
    runTest("Output Test", testOutput);
    runTest("Invalid Instructions Test", testInvalidInstructions);
    runTest("Program Block Test", testProgramBlock);
    std::cout << "\nAll basic tests completed.\n" << std::endl;
}

void TestILOCParser::runTest(const std::string& testName, std::function<void()> testFunc) {
    try {
        std::cout << "Running " << testName << "... ";
        testFunc();
        std::cout << "PASSED" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "FAILED" << std::endl;
        std::cout << "Error: " << e.what() << std::endl;
        throw;
    }
}

void TestILOCParser::assertInstruction(const ILOCInstruction& actual, 
                                     ILOCOpcode expectedOp,
                                     const std::string& expectedSrc1,
                                     const std::string& expectedSrc2,
                                     const std::string& expectedDest,
                                     int expectedImm) {
    assert(actual.opcode == expectedOp);
    assert(actual.src1 == expectedSrc1);
    assert(actual.src2 == expectedSrc2);
    assert(actual.dest == expectedDest);
    assert(actual.immediate == expectedImm);
}

void TestILOCParser::testLoadI() {
    auto inst = ILOCParser::parseLine("loadI 42 => r1");
    assertInstruction(inst, ILOCOpcode::LOADI, "", "", "r1", 42);
}

void TestILOCParser::testLoad() {
    auto inst = ILOCParser::parseLine("load r1 => r2");
    assertInstruction(inst, ILOCOpcode::LOAD, "r1", "", "r2", 0);
}

void TestILOCParser::testStore() {
    auto inst = ILOCParser::parseLine("store r1 => r2");
    assertInstruction(inst, ILOCOpcode::STORE, "r1", "", "r2", 0);
}

void TestILOCParser::testArithmetic() {
    auto addInst = ILOCParser::parseLine("add r1, r2 => r3");
    assertInstruction(addInst, ILOCOpcode::ADD, "r1", "r2", "r3", 0);

    auto subInst = ILOCParser::parseLine("sub r1, r2 => r3");
    assertInstruction(subInst, ILOCOpcode::SUB, "r1", "r2", "r3", 0);

    auto multInst = ILOCParser::parseLine("mult r1, r2 => r3");
    assertInstruction(multInst, ILOCOpcode::MULT, "r1", "r2", "r3", 0);
}

void TestILOCParser::testOutput() {
    auto inst = ILOCParser::parseLine("output 1024");
    assertInstruction(inst, ILOCOpcode::OUTPUT, "", "", "", 1024);
}

void TestILOCParser::testInvalidInstructions() {
    try {
        ILOCParser::parseLine("invalid r1 => r2");
        assert(false && "Should throw exception");
    } catch (const ILOCParseException&) {
        // 预期的异常
    }

    try {
        ILOCParser::parseLine("loadI abc => r1");
        assert(false && "Should throw exception");
    } catch (const ILOCParseException&) {
        // 预期的异常
    }
}

void TestILOCParser::testProgramBlock() {
    std::vector<std::string> testProgram = {
        "loadI 0 => r1",
        "loadI 1 => r2",
        "add r1,r2 => r3",
        "store r3 => r1",
        "output 0"
    };

    for (const auto& line : testProgram) {
        auto inst = ILOCParser::parseLine(line);
        assert(inst.lineNumber == 0);  // 确保基本解析正确
    }
}

void TestILOCParser::testInputFile(const std::string& filename) {
    try {
        std::cout << "Parsing file: " << filename << "\n\n";
        auto instructions = ILOCParser::parseFile(filename);
        
        std::cout << "Successfully parsed " << instructions.size() << " instructions:\n";
        std::cout << std::string(50, '-') << "\n";
        
        for (const auto& inst : instructions) {
            printInstruction(inst);
        }
        
        std::cout << std::string(50, '-') << "\n";
        std::cout << "Parse completed successfully.\n";
        
    } catch (const ILOCParseException& e) {
        std::cerr << "Parse error at line " << e.getLine() << ": " << e.what() << std::endl;
        throw;
    }
}

void TestILOCParser::printInstruction(const ILOCInstruction& inst) {
    std::cout << std::setw(4) << inst.lineNumber << ": ";
    
    switch (inst.opcode) {
        case ILOCOpcode::LOADI:
            std::cout << "loadI " << inst.immediate << " => " << inst.dest;
            break;
            
        case ILOCOpcode::LOAD:
            std::cout << "load " << inst.src1 << " => " << inst.dest;
            break;
            
        case ILOCOpcode::STORE:
            std::cout << "store " << inst.src1 << " => " << inst.dest;
            break;
            
        case ILOCOpcode::OUTPUT:
            std::cout << "output " << inst.immediate;
            break;
            
        default:  // 算术操作
            std::cout << getOpcodeStr(inst.opcode) << " " 
                     << inst.src1 << "," << inst.src2 
                     << " => " << inst.dest;
    }
    std::cout << std::endl;
}
