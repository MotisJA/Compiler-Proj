#include "LL1TableGenerator.h"
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <iomanip>

LL1TableGenerator::LL1TableGenerator() {
    // 初始化产生式
    productions = {
        {"Grammar", {"ProductionList"}},
        {"ProductionList", {"ProductionSet", "SEMICOLON", "ProductionList'"}},
        {"ProductionList'", {"ProductionSet", "SEMICOLON", "ProductionList'"}},
        {"ProductionList'", {"#"}},
        {"ProductionSet", {"SYMBOL", "DERIVES", "RightHandSide", "ProductionSet'"}},
        {"ProductionSet'", {"ALSODERIVES", "RightHandSide", "ProductionSet'"}},
        {"ProductionSet'", {"#"}},
        {"RightHandSide", {"SymbolList"}},
        {"RightHandSide", {"EPSILON"}},
        {"SymbolList", {"SYMBOL", "SymbolList'"}},
        {"SymbolList'", {"SYMBOL", "SymbolList'"}},
        {"SymbolList'", {"#"}}
    };
    // 初始化非终结符和终结符
    nonTerminals = {
        {"Grammar", 6}, {"ProductionList", 7}, {"ProductionList'", 8},
        {"ProductionSet", 9}, {"ProductionSet'", 10}, {"RightHandSide", 11},
        {"SymbolList", 12}, {"SymbolList'", 13}
    };
    terminals = {
        {"SYMBOL", 0}, {"DERIVES", 1}, {"ALSODERIVES", 2},
        {"EPSILON", 3}, {"SEMICOLON", 4}, {"EOF_TOKEN", 5}
    };
    // 使用常量值初始化分析表格
    parsingTable.resize(MAX_INDEX + 1, std::vector<int>(MAX_INDEX + 1, -1));

    // 依次计算四个表格内容
    computeFirstSets();
    computeFollowSets();
    computeFirstPlusSets();
    computeParsingTable();
}

void LL1TableGenerator::computeFirstSets() {
    // 初始化 FIRST sets
    for (const auto& [terminal, _] : terminals) {
        firstSets[terminal].insert(terminal);
    }
    firstSets["#"].insert("#");
    for (const auto& [nonTerminal, _] : nonTerminals) {
        firstSets[nonTerminal];
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& production : productions) {
            const std::string& nonTerminal = production.first;
            const std::vector<std::string>& rhs = production.second;
            std::unordered_set<std::string> rhsFirstSet;
            size_t i = 0;

            while (i < rhs.size()) {
                const std::string& symbol = rhs[i];
                for (const auto& firstSymbol : firstSets[symbol]) {
                    if (firstSymbol != "#") {
                        rhsFirstSet.insert(firstSymbol);
                    }
                }
                if (firstSets[symbol].count("#") > 0) {
                    i++;
                } else {
                    break;
                }
            }

            if (i == rhs.size()) {
                rhsFirstSet.insert("#");
            }

            size_t oldSize = firstSets[nonTerminal].size();
            firstSets[nonTerminal].insert(rhsFirstSet.begin(), rhsFirstSet.end());

            if (firstSets[nonTerminal].size() > oldSize) {
                changed = true;
            }
        }
    }
}

void LL1TableGenerator::computeFollowSets() {
    // 初始化 FOLLOW sets
    for (const auto& [nonTerminal, _] : nonTerminals) {
        followSets[nonTerminal];
    }
    followSets["Grammar"].insert("EOF_TOKEN");
    // 遍历每个产生式，计算 FOLLOW 集合
    bool changed = true;
    while (changed) {
        changed = false;
        for (const auto& production : productions) {
            const std::string& nonTerminal = production.first;
            const std::vector<std::string>& rhs = production.second;
            std::unordered_set<std::string> trailer = followSets[nonTerminal];
            // 从右向左遍历产生式右部
            for (int i = rhs.size() - 1; i >= 0; --i) {
                const std::string& symbol = rhs[i];
                // 如果是非终结符，将 TRAILER 加入到 FOLLOW(symbol) 中并更新
                if (nonTerminals.count(symbol)) {
                    size_t oldSize = followSets[symbol].size();
                    followSets[symbol].insert(trailer.begin(), trailer.end());
                    // 更新TRAILER
                    if (firstSets[symbol].count("#") > 0) {
                        trailer.insert(firstSets[symbol].begin(), firstSets[symbol].end());
                        trailer.erase("#");
                    } else {
                        trailer = firstSets[symbol];
                    }

                    if (followSets[symbol].size() > oldSize) {
                        changed = true;
                    }
                // 如果是终结符，更新 TRAILER 为 FIRST(symbol)
                } else {
                    trailer.clear();
                    trailer.insert(symbol);
                }
            }
        }
    }
}

void LL1TableGenerator::computeFirstPlusSets() {
    for (size_t i = 0; i < productions.size(); ++i) {
        const std::string& nonTerminal = productions[i].first;
        const std::vector<std::string>& rhs = productions[i].second;
        
        std::unordered_set<std::string> firstPlus;
        bool allDeriveEmpty = true;
        // 如果右侧为空(#)，直接加入FOLLOW集
        if (rhs.size() == 1 && rhs[0] == "#") {
            firstPlus.insert(followSets[nonTerminal].begin(), followSets[nonTerminal].end());
        } else {
            // 依次计算右侧符号的FIRST集
            for (const auto& symbol : rhs) {
                const auto& firstSet = firstSets[symbol];
                
                // 加入除#之外的所有符号
                for (const auto& term : firstSet) {
                    if (term != "#") {
                        firstPlus.insert(term);
                    }
                }
                // 如果当前符号不能推导出#，终止循环
                if (firstSet.find("#") == firstSet.end()) {
                    allDeriveEmpty = false;
                    break;
                }
            }
            // 如果所有符号都可以推导出#，加入FOLLOW集
            if (allDeriveEmpty) {
                firstPlus.insert(followSets[nonTerminal].begin(), followSets[nonTerminal].end());
            }
        }
        firstPlusSets[i] = std::move(firstPlus);
    }
}

void LL1TableGenerator::computeParsingTable() {
    // 初始化分析表，所有位置设为-1（表示error）
    for (const auto& [nonTerm, nIndex] : nonTerminals) {
        for (const auto& [term, tIndex] : terminals) {
            parsingTable[nIndex][tIndex] = -1;
        }
    }

    // 遍历所有产生式
    for (size_t i = 0; i < productions.size(); ++i) {
        const std::string& nonTerminal = productions[i].first;
        int nonTermIndex = nonTerminals[nonTerminal];

        // 遍历该产生式的FIRST+集合中的所有符号
        for (const auto& symbol : firstPlusSets[i]) {
            // 如果是终结符，在分析表中填入产生式编号
            if (terminals.count(symbol) > 0) {
                int termIndex = terminals[symbol];
                parsingTable[nonTermIndex][termIndex] = i;
            }
        }
    }
}

/// @brief 下面的函数均用于打印分析表格
void LL1TableGenerator::printProductions() const {
    // 打印产生式
    std::cout << "Productions:\n";
    for (size_t i = 0; i < productions.size(); ++i) {
        std::cout << i << ": " << productions[i].first << " -> ";
        for (const auto& symbol : productions[i].second) {
            std::cout << symbol << " ";
        }
        std::cout << ";" << std::endl;
    }
}

void LL1TableGenerator::printFirstSets() {
    std::cout << "FIRST sets:\n";
    for (const auto& pair : firstSets) {
        std::cout << pair.first << ": { ";
        for (const auto& symbol : pair.second) {
            std::cout << symbol << ", ";
        }
        std::cout << "}\n";
    }
}

void LL1TableGenerator::printFollowSets() {
    std::cout << "FOLLOW sets:\n";
    for (const auto& pair : followSets) {
        std::cout << pair.first << ": { ";
        for (const auto& symbol : pair.second) {
            std::cout << symbol << ", ";
        }
        std::cout << "}\n";
    }
}

void LL1TableGenerator::printFirstPlusSets() {
    std::cout << "FIRST+ sets:\n";
    for (const auto& pair : firstPlusSets) {
        std::cout << "Production " << pair.first << ": { ";
        for (const auto& symbol : pair.second) {
            std::cout << symbol << ", ";
        }
        std::cout << "}\n";
    }
}

/// @brief 以下函数均用于打印LL1分析表和调整其格式
void LL1TableGenerator::printParsingTable() {
    // 创建有序的非终结符和终结符列表
    std::vector<std::pair<std::string, int>> sortedNonTerminals(nonTerminals.begin(), nonTerminals.end());
    std::vector<std::pair<std::string, int>> sortedTerminals(terminals.begin(), terminals.end());
    
    // 按照值排序
    std::sort(sortedNonTerminals.begin(), sortedNonTerminals.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });
    std::sort(sortedTerminals.begin(), sortedTerminals.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // 计算每列的最大宽度
    std::vector<size_t> columnWidths(terminals.size() + 1, 0);
    columnWidths[0] = std::string("NonTerminal").length();
    
    // 计算非终结符列的宽度
    for (const auto& [name, _] : sortedNonTerminals) {
        columnWidths[0] = std::max(columnWidths[0], name.length());
    }

    // 计算终结符列的宽度
    size_t col = 1;
    for (const auto& [term, _] : sortedTerminals) {
        columnWidths[col] = std::max(term.length(), size_t(3));  // 至少3个字符宽
        col++;
    }

    // 打印表头
    std::cout << std::left << std::setw(columnWidths[0]) << "NonTerminal";
    col = 1;
    for (const auto& [term, _] : sortedTerminals) {
        std::cout << " | " << std::setw(columnWidths[col++]) << term;
    }
    std::cout << "\n";

    // 打印分隔线
    for (size_t i = 0; i < columnWidths[0]; ++i) std::cout << "-";
    for (size_t i = 1; i < columnWidths.size(); ++i) {
        std::cout << "-+-";
        for (size_t j = 0; j < columnWidths[i]; ++j) std::cout << "-";
    }
    std::cout << "\n";

    // 打印表内容
    for (const auto& [nonTerm, nIndex] : sortedNonTerminals) {
        std::cout << std::left << std::setw(columnWidths[0]) << nonTerm;
        col = 1;
        for (const auto& [term, tIndex] : sortedTerminals) {
            int productionIndex = parsingTable[nIndex][tIndex];
            std::string cell = (productionIndex != -1) ? std::to_string(productionIndex) : "-";
            std::cout << " | " << std::setw(columnWidths[col++]) << cell;
        }
        std::cout << "\n";
    }
}

int LL1TableGenerator::getParsingTableEntry(const std::string& nonTerminal, const std::string& terminal) const {
    auto nonTermIt = nonTerminals.find(nonTerminal);
    auto termIt = terminals.find(terminal);
    
    if (nonTermIt != nonTerminals.end() && termIt != terminals.end()) {
        return parsingTable[nonTermIt->second][termIt->second];
    }
    return -1;
}

std::pair<std::string, std::vector<std::string>> LL1TableGenerator::getProduction(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < productions.size()) {
        return productions[index];
    }
    return {"", {}};
}