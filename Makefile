CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = compiler
TEST_TARGET = lexer_test

SRCS = lexer.cpp paser.cpp main.cpp
TEST_SRCS = lexer.cpp lexer_test.cpp
OBJS = $(SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)

.PHONY: all clean