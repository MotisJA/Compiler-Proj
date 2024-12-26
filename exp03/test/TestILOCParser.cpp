#include "TestILOCParser.h"
#include <iostream>
#include <cassert>

void TestILOCParser::runAllTests() {
    runTest("LoadI Test", testLoadI);
    runTest("Load Test", testLoad);
    runTest("Store Test", testStore);
    runTest("Arithmetic Test", testArithmetic);
    runTest("Output Test", testOutput);
    runTest("Invalid Instructions Test", testInvalidInstructions);
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
