CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I./include

TARGET = 412alloc
TEST_TARGET = test_412alloc

# Windows和Linux环境下的可执行文件后缀
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
else
    EXE_EXT =
endif

# 最终的可执行文件名
FINAL_TARGET = $(TARGET)$(EXE_EXT)
FINAL_TEST_TARGET = $(TEST_TARGET)$(EXE_EXT)

# 主程序源文件
SRCS = src/main.cpp \
       src/ILOCParser.cpp \
       src/TopDownAllocator.cpp \
       src/BottomUpAllocator.cpp \
       src/ILOCFormatter.cpp

# 测试程序源文件
TEST_SRCS = src/ILOCParser.cpp \
            test/test_main.cpp \
            test/TestILOCParser.cpp

.PHONY: all clean test

# 默认目标改为编译主程序
all: $(FINAL_TARGET)

$(FINAL_TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@

test: $(FINAL_TEST_TARGET)
	./$(FINAL_TEST_TARGET)

$(FINAL_TEST_TARGET): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(TEST_SRCS) -o $@

# 根据操作系统选择删除命令
ifeq ($(OS),Windows_NT)
    RM_CMD = del /Q /F 2>nul || ver>nul
else
    RM_CMD = rm -f
endif

clean:
	-$(RM_CMD) *.exe *.o
