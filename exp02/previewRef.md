# 提交说明

## 预期结果

以下是根据文法计算得到的 **FIRST**、**FOLLOW** 和 **FIRST+** 集合，以及 LL(1) 语法分析表

### 1. FIRST 集

| 非终结符        | FIRST 集          |
| --------------- | ----------------- |
| Grammar         | {SYMBOL}          |
| ProductionList  | {SYMBOL}          |
| ProductionList' | {SYMBOL, ε}       |
| ProductionSet   | {SYMBOL}          |
| ProductionSet'  | {ALSODERIVES, ε}  |
| RightHandSide   | {SYMBOL, EPSILON} |
| SymbolList      | {SYMBOL}          |
| SymbolList'     | {SYMBOL, ε}       |

---

### 2. FOLLOW 集

| 非终结符        | FOLLOW 集                         |
| --------------- | --------------------------------- |
| Grammar         | {EOF}                             |
| ProductionList  | {EOF}                             |
| ProductionList' | {EOF}                             |
| ProductionSet   | {SEMICOLON}              |
| ProductionSet'  | {SEMICOLON}              |
| RightHandSide   | {SEMICOLON, ALSODERIVES} |
| SymbolList      | {SEMICOLON, ALSODERIVES} |
| SymbolList'     | {SEMICOLON, ALSODERIVES} |

---

### 3. FIRST+ 集

| 产生式                                                      | FIRST+ 集                         |
| ----------------------------------------------------------- | --------------------------------- |
| Grammar → ProductionList                                    | {SYMBOL}                          |
| ProductionList → ProductionSet SEMICOLON ProductionList'    | {SYMBOL}                          |
| ProductionList' → ProductionSet SEMICOLON ProductionList'   | {SYMBOL}                          |
| ProductionList' → ε                                         | {EOF}                             |
| ProductionSet → SYMBOL DERIVES RightHandSide ProductionSet' | {SYMBOL}                          |
| ProductionSet' → ALSODERIVES RightHandSide ProductionSet'   | {ALSODERIVES}                     |
| ProductionSet' → ε                                          | {SEMICOLON}              |
| RightHandSide → SymbolList                                  | {SYMBOL}                          |
| RightHandSide → EPSILON                                     | {EPSILON}                         |
| SymbolList → SYMBOL SymbolList'                             | {SYMBOL}                          |
| SymbolList' → SYMBOL SymbolList'                            | {SYMBOL}                          |
| SymbolList' → ε                                             | {SEMICOLON, ALSODERIVES} |

---

### 4. LL(1) 语法分析表

| 非终结符        | SYMBOL | `:`  | `\|`  | `EPSILON` | `;`  | `EOF` |
| --------------- | ------ | ---- | ---- | --------- | ---- | ----- |
| Grammar         | 0      |      |      |           |      |       |
| ProductionList  | 1      |      |      |           |      |       |
| ProductionList' | 2      |      |      |           |      | 3     |
| ProductionSet   | 4      |      |      |           |      |       |
| ProductionSet'  |        |      | 5    |           | 6    |       |
| RightHandSide   | 7      |      |      | 8         |      |       |
| SymbolList      | 9      |      |      |           |      |       |
| SymbolList'     | 10     |      | 11   |           | 11   |       |

---
