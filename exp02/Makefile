CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS =

SRCS = main.cpp LL1TableGenerator.cpp LL1Parser.cpp lexer.cpp
TEST_SRCS = test.cpp LL1TableGenerator.cpp LL1Parser.cpp lexer.cpp
TARGET = parser
TEST_TARGET = test_parser

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean