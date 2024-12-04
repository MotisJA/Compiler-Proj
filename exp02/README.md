# 运行说明

本项目是 2024年秋 **编译技术课程设计**的第二次提交内容，包括一个**LL(1)表格生成器**，以及基于生成的LL1表格查询的**LL(1)语法分析器**。`lexer.cpp` `LL1Parser.cpp`分别是词法分析器和语法分析器，`LL1TableGenerator`是对应的LL1表格生成器，`ASTNode.cpp` 定义了AST的基本结构和输出形式；在`main.cpp`中，定义了我的命令行程序，并提供了相应的命令行选项

## 运行方法

1. 在项目目录下运行 `make`
2. 命令行输入 `./parser` 进入语法分析，接受命令行输入(以空行结尾)，运行LL1语法分析并输出AST表示的分析结果
3. 命令行输入 `./parser -p` 打印 **产生式**
4. 命令行输入 `./parser -f` 打印 **First** 集
5. 命令行输入 `./parser -g` 打印 **Follow** 集
6. 命令行输入 `./parser -h` 打印 **First+** 集
7. 命令行输入 `./parser -t` 打印 **LL1语法分析表**
8. 命令行输入 `./test_parser` 运行语法分析测试程序，该程序读取 `test_cases.txt` 中的测试用例并分别打印词法分析和语法分析结果。

## 结果说明

- 因字符"ε"在控制台输出显示中可能会出现乱码，因此产生式和各类表格的"ε"全部替换为"#"
- 因First集、Follow集和First+集都是哈希表存储，输出顺序可能较乱；但在LL1分析表中做了顺序调整，以更加直观和方便检验

## 示例输出

![示例一](images\example01.png){ width=75% }
![示例二](images\example02.png){ width=75% }
![示例三](images\example03.png){ width=75% }
![示例四](images\example04.png){ width=75% }

