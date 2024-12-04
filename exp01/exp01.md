# 实验参考

## MBNF 文法

你的程序需要理解此文法，来完成对于给定输入的语法分析。请注意，这一文法是存在左递归的。你需要对文法做变换，消除左递归，而后完成自上而下的递归下降语法分析。文法变换可以手动完成，也可以通过你编写的程序完成。后者是加分项。词法分析表中所有包含全部大写字母的符号（比如SEMICOLON）表示终结符，其余符号为非终结符。在这一文法规则下，输入的字（word）之间是可以由空白字符相连的。空白字符包括空格（blanks），制表符（tab characters），行终止符（end-of-line）。

Grammar        :  ProductionList ;
ProductionList :  ProductionSet SEMICOLON  |  ProductionList ProductionSet SEMICOLON ;
ProductionSet  :  SYMBOL DERIVES RightHandSide  |  ProductionSet ALSODERIVES RightHandSide ;
RightHandSide  :  SymbolList  |  EPSILON ;
SymbolList     :  SymbolList SYMBOL  |  SYMBOL ;

## 消除左递归后的文法

Grammar        →  ProductionList

ProductionList →  ProductionSet SEMICOLON ProductionList'

ProductionList'→  ProductionSet SEMICOLON ProductionList' | ε

ProductionSet  →  SYMBOL DERIVES RightHandSide ProductionSet'

ProductionSet' →  ALSODERIVES RightHandSide ProductionSet' | ε

RightHandSide  →  SymbolList | EPSILON

SymbolList     →  SYMBOL SymbolList'

SymbolList'    →  SYMBOL SymbolList' | ε

## 词法分析器

| Terminal Symbol | Regular Expression                  | Token Type |
|-----------------|-------------------------------------|------------|
| SEMICOLON       | ;                                   | 0          |
| DERIVES         | :                                   | 1          |
| ALSODERIVES     | \|                                  | 2          |
| EPSILON         | EPSILON \| epsilon \| Epsilon       | 3          |
| SYMBOL          | [a-z \| A-Z \| 0-9]+                | 4          |
| EOF             | (not applicable)                    | 5          |

## 实验任务

本实验的第一项任务，是手动编写一个词法分析器，对给定输入做词法分析。图2给出了MBNF文法中终结符字所对应的正则表达式（regular expression）和字段类型（token type）。你的词法分析器需要对给定输入字符串进行处理，建立一个查找表格（lookup table），记录输入中包含的字段及其类型。图2中的EOF（End Of File）为文件终止符号，无正则表达式。编写的词法分析器，应该是可靠且高效的，这需要使用高效的数据结构，比如哈希表和哈希集。关于如何建立高效的查找表格，请参考教材Engineering a Compiler第二版的Appendix B。词法分析器应当创建一个字段表，为符合图2中第一列终结符的每个输入字段，给出第三列的字段类型。

完成词法分析器后，你可以开始转换图1给出的MBNF语法，并手工编写一个递归下降语法分析器。此分析器需要建立一个语法产生式（production）的内部模型，方便后续计算FIRST，FOLLOW，FIRST+集合。建议你回顾一下自上而下语法分析过程中的相关算法，来决定使用什么样的内部模型。

> 第一次提交的内容是你所完成的（1）词法分析器和（2）递归下降语法分析器。你需要提交两个分析器的代码，这些代码必须能够运行。不要提交不能编译或无法正常完成词法语法分析的代码。你需要给出三个符合MBNF语法的输入及运行结果，来说明你的分析器，至少对这三个例子是可以正常工作的。此外，对于非法的输入，你的分析器应该给出有用的错误提示及诊断信息。助教将对你提交的代码进行检查和测试。我们将更多的测试输入来检测你的分析器是否可以正常工作。这些输入我们不会公开。
