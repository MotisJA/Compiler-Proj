CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS =

SRCS = main.cpp LL1TableGenerator.cpp LL1Parser.cpp lexer.cpp ASTNode.cpp
TEST_SRCS = test_parser.cpp LL1TableGenerator.cpp LL1Parser.cpp lexer.cpp ASTNode.cpp
TARGET = parser
TEST_TARGET = test_parser

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

clean:
	del /f $(TARGET).exe $(TEST_TARGET).exe

.PHONY: all clean