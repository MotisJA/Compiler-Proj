#ifndef LL1TABLEGENERATOR_H
#define LL1TABLEGENERATOR_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class LL1TableGenerator {
public:
    // 添加常量来定义最大索引
    static const int MAX_INDEX = 13;

    LL1TableGenerator();
    void printProductions() const;
    void printFirstSets();
    void printFollowSets();
    void printFirstPlusSets();
    void printParsingTable();
    int getParsingTableEntry(const std::string& nonTerminal, const std::string& terminal) const;
    std::pair<std::string, std::vector<std::string>> getProduction(int index) const;

private:
    void computeFirstSets();
    void computeFollowSets();
    void computeFirstPlusSets();
    void computeParsingTable();

    std::unordered_map<std::string, std::unordered_set<std::string>> firstSets;
    std::unordered_map<std::string, std::unordered_set<std::string>> followSets;
    std::unordered_map<int, std::unordered_set<std::string>> firstPlusSets;

    std::vector<std::pair<std::string, std::vector<std::string>>> productions;
    std::unordered_map<std::string, int> nonTerminals;
    std::unordered_map<std::string, int> terminals;

    std::vector<std::vector<int>> parsingTable;
};

#endif // LL1TABLEGENERATOR_H