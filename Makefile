CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = compiler
LEXER_TEST_TARGET = lexer_test
PARSER_TEST_TARGET = paser_test

SRCS = lexer.cpp paser.cpp main.cpp
LEXER_TEST_SRCS = lexer.cpp lexer_test.cpp
PARSER_TEST_SRCS = lexer.cpp paser.cpp paser_test.cpp
OBJS = $(SRCS:.cpp=.o)
LEXER_TEST_OBJS = $(LEXER_TEST_SRCS:.cpp=.o)
PARSER_TEST_OBJS = $(PARSER_TEST_SRCS:.cpp=.o)

all: $(TARGET) $(LEXER_TEST_TARGET) $(PARSER_TEST_TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(LEXER_TEST_TARGET): $(LEXER_TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(LEXER_TEST_TARGET) $(LEXER_TEST_OBJS)

$(PARSER_TEST_TARGET): $(PARSER_TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(PARSER_TEST_TARGET) $(PARSER_TEST_OBJS)

debug: $(PARSER_TEST_TARGET)
	gdb ./$(PARSER_TEST_TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LEXER_TEST_OBJS) $(PARSER_TEST_OBJS) $(TARGET) $(LEXER_TEST_TARGET) $(PARSER_TEST_TARGET) compiler.exe lexer_test.exe paser_test.exe

.PHONY: all clean