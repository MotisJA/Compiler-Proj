# 本地寄存器分配器 (Local Register Allocator)

本项目是 2024年秋 **编译技术课程设计**的第三次提交内容，这个项目实现了两种本地寄存器分配算法:自上而下(Top-down)和自下而上(Bottom-up)分配策略。

## 项目结构

```
exp03/
├── include/                    # 头文件目录
│   ├── ILOCParser.h           # ILOC指令解析器
│   ├── ILOCTypes.h           # ILOC类型定义
│   ├── RegisterAllocator.h    # 寄存器分配器接口与实现
│   └── ILOCFormatter.h        # ILOC指令格式化工具
├── src/                       # 源代码目录
│   ├── main.cpp              # 主程序
│   ├── ILOCParser.cpp        # ILOC解析器实现
│   ├── TopDownAllocator.cpp  # 自上而下分配器实现
│   ├── BottomUpAllocator.cpp # 自下而上分配器实现
│   └── ILOCFormatter.cpp     # 格式化工具实现
├── test/                     # 测试目录
│   ├── test_main.cpp        # 测试主程序
│   └── TestILOCParser.cpp   # 解析器测试
├── makefile                  # 编译配置文件
└── README.md                # 项目说明文档
```

## 编译与运行

### 编译

使用make命令编译项目:

```bash
make
```

如果需要清理编译产物:

```bash
make clean
```

### 运行

程序接受三个命令行参数:

```bash
./412alloc <k> <t|b> <input_file>
```

参数说明:
- k: 可用寄存器数量(正整数，至少为3)
- t/b: 寄存器分配策略(t=top-down, b=bottom-up)
- input_file: 输入文件路径

示例:
```bash
./412alloc 5 t input
```

**注意**：_无论是自顶向下还是自底向上算法的实现，溢出位置都是从内存地址10000开始。如果你需要在ILOC程序块中操作内存地址，请确保不会大于10000_

## 输出文件

程序会打印ILOC程序块分配后的结果，并会生成两个输出文件:
- <input_file>.out: 带有输入ILOC程序块行号的输出
- <input_file>.console.out: 不带行号的输出(同时也会打印到控制台)

## 实现细节

### 自上而下分配器 (TopDownAllocator)

- 基于贪心策略,优先分配使用频率最高的虚拟寄存器
- 当寄存器不足时使用溢出机制
- 保留固定数量的寄存器(2个)用于处理溢出

### 自下而上分配器 (BottomUpAllocator)

- 基于活跃区间分析
- 动态分配和释放寄存器
- 使用最远未使用(Furthest-Use)策略进行寄存器溢出
- 保留寄存器r(k)用于处理溢出，主要是存放溢出地址

## 错误处理

程序会对以下情况进行检查并给出相应错误提示:

- 命令行参数数量不正确
- k值不是正整数或者小于3
- 分配策略参数不是't'或'b'
- 输入文件不存在或无法打开
- ILOC指令格式错误

## 支持的ILOC指令

支持以下ILOC指令子集:

- load
- loadI
- store
- add
- sub
- mult 
- lshift
- rshift
- output

## 示例输出

项目的5个测试用例存放在test目录下，你可以依次拷贝到input文件中运行。对于这5个测试用例，可用寄存器不小于3时，程序给出的ILOC程序块输出在仅使用寄存器1~k的情况下能和源程序保持一致。

### 对test/test_1: 可分配 3 个寄存器

![示例一](images\1_test1.png){ width=75% }
![示例一](images\2_test1.png){ width=75% }
![示例一](images\3_test1.png){ width=75% }

### 对test/test_2：可分配 4 个寄存器

![示例二](images\1_test2.png){ width=75% }
![示例二](images\2_test2.png){ width=75% }
![示例二](images\3_test2.png){ width=75% }

### 对test/test_5：可分配 3 个寄存器

![示例三](images\1_test5.png){ width=75% }
![示例三](images\2_test5.png){ width=75% }
![示例三](images\3_test5.png){ width=75% }