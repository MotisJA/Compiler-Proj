CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I./include

TARGET = 412alloc
TEST_TARGET = test_412alloc

# 主程序源文件
SRCS = src/main.cpp \
       src/ILOCParser.cpp \
       src/TopDownAllocator.cpp \
       src/BottomUpAllocator.cpp \
       src/ILOCFormatter.cpp

# ILOC指令分析器测试程序源文件
TEST_SRCS = test/test_main.cpp \
            test/TestILOCParser.cpp \
            src/ILOCParser.cpp

.PHONY: all clean test

# 测试目标优先
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET):
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o $@

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

clean:
	rm -f $(TARGET) $(TEST_TARGET)
