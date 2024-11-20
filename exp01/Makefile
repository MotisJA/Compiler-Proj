CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = compiler
LEXER_TEST_TARGET = lexer_test
PARSER_TEST_TARGET = parser_test

SRCS = lexer.cpp parser.cpp main.cpp
LEXER_TEST_SRCS = lexer.cpp lexer_test.cpp
PARSER_TEST_SRCS = lexer.cpp parser.cpp parser_test.cpp

all: $(TARGET) $(LEXER_TEST_TARGET) $(PARSER_TEST_TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

$(LEXER_TEST_TARGET):
	$(CXX) $(CXXFLAGS) -o $(LEXER_TEST_TARGET) $(LEXER_TEST_SRCS)

$(PARSER_TEST_TARGET):
	$(CXX) $(CXXFLAGS) -o $(PARSER_TEST_TARGET) $(PARSER_TEST_SRCS)

debug: $(PARSER_TEST_TARGET)
	gdb ./$(PARSER_TEST_TARGET)

clean:
	rm -f $(TARGET) $(LEXER_TEST_TARGET) $(PARSER_TEST_TARGET) compiler.exe lexer_test.exe parser_test.exe

.PHONY: all clean