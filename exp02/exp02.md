# 实验二：基于表格查询的LL(1) 语法分析器

除递归下降语法分析器外，你还将设计开发一个基于表格查询的LL(1) 语法分析器（table-driven LL(1) parser）。这部分内容包含两个任务：（1）完成一个LL(1)表格生成器（table generator）；（2）完成基于表格的语法分析器。

## LL(1) 表格生成器

你所实现的表格生成器需要根据给定的输入，自动计算三个集合：FIRST、FOLLOW、FIRST+集，并依据这三个集合，产生LL(1)语法分析表格。给定的输入是下面的文法：
Grammar        →  ProductionList
ProductionList →  ProductionSet SEMICOLON ProductionList'
ProductionList'→  ProductionSet SEMICOLON ProductionList' | ε
ProductionSet  →  SYMBOL DERIVES RightHandSide ProductionSet'
ProductionSet' →  ALSODERIVES RightHandSide ProductionSet' | ε
RightHandSide  →  SymbolList | EPSILON
SymbolList     →  SYMBOL SymbolList'
SymbolList'    →  SYMBOL SymbolList' | ε

终结符包括

| Terminal Symbol | Regular Expression                  | Token Type |
|-----------------|-------------------------------------|------------|
| SEMICOLON       | ;                                   | 0          |
| DERIVES         | :                                   | 1          |
| ALSODERIVES     | \|                                  | 2          |
| EPSILON         | EPSILON \| epsilon \| Epsilon       | 3          |
| SYMBOL          | [a-z \| A-Z \| 0-9]+                | 4          |
| EOF             | (not applicable)                    | 5          |

为方便调试和给分，你的生成器需要命令行选项，打印产生式（productions），FIRST、FOLLOW、FIRST+集合，以及LL(1)分析表格。你需要实现以下命令行选项。

-p 打印分析器所识别的产生式，打印结果需要是人工易读的
-f 为每个语法符号打印FIRST集，结果人工易读
-g 为每个非终结符打印FOLLOW集，结果人工易读
-h 为每个产生式打印FIRST+集，结果人工易读
-t 打印LL(1)分析表格，包含标题、行与列名字、每个单元格内容等。对于非错误单元格内容，可使用产生式序号（-p结果中显示）作为产生式表示。

此外，对于无效选项，生成器应当给出无效信息，并打印所有支持的选项内容，方便用户查看。

为语法符号及产生式使用一致的编号，结果如下。

| 终结符 (Terminals) | 编号 |
| ------------------ | ---- |
| SYMBOL             | 0    |
| DERIVES            | 1    |
| ALSODERIVES        | 2    |
| EPSILON            | 3    |
| SEMICOLON          | 4    |
| EOF                | 5    |

| 非终结符 (Nonterminals) | 编号 |
| ----------------------- | ---- |
| Grammar                 | 6    |
| ProductionList          | 7    |
| ProductionList'         | 8    |
| ProductionSet           | 9    |
| ProductionSet'          | 10   |
| RightHandSide           | 11   |
| SymbolList              | 12   |
| SymbolList'             | 13   |

| 产生式 (Productions)                                        | 编号 |
| ----------------------------------------------------------- | ---- |
| Grammar → ProductionList                                    | 0    |
| ProductionList → ProductionSet SEMICOLON ProductionList'    | 1    |
| ProductionList' → ProductionSet SEMICOLON ProductionList'   | 2    |
| ProductionList' → ε                                         | 3    |
| ProductionSet → SYMBOL DERIVES RightHandSide ProductionSet' | 4    |
| ProductionSet' → ALSODERIVES RightHandSide ProductionSet'   | 5    |
| ProductionSet' → ε                                          | 6    |
| RightHandSide → SymbolList                                  | 7    |
| RightHandSide → EPSILON                                     | 8    |
| SymbolList → SYMBOL SymbolList'                             | 9    |
| SymbolList' → SYMBOL SymbolList'                            | 10   |
| SymbolList' → ε                                             | 11   |

## LL(1) 语法分析器

最终，你需要根据你所开发的LL(1) 表格生成器，完成一个LL(1) 语法分析器。跟递归下降分析器要求类似，你的分析器必须能够运行。你需要给出之前三个语法例子的输出结果，说明你的分析器是可正常工作的。对于非法的输入，分析器应该给出有用的错误提示及诊断信息。助教将对你提交的代码进行检查和测试。
