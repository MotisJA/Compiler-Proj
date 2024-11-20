#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

struct Production {
    string lhs;
    vector<string> rhs;
};

/// @brief 消除直接左递归
/// @param productions 
void eliminateDirectLeftRecursion(vector<Production>& productions) {
    vector<Production> newProductions;
    for (auto& prod : productions) {
        vector<string> alpha, beta;
        for (auto& rhs : prod.rhs) {
            if (rhs.substr(0, prod.lhs.size()) == prod.lhs) {
                alpha.push_back(rhs.substr(prod.lhs.size()));
            } else {
                beta.push_back(rhs);
            }
        }
        if (!alpha.empty()) {
            string newNonTerminal = prod.lhs + "'";
            for (auto& b : beta) {
                newProductions.push_back({prod.lhs, {b + " " + newNonTerminal}});
            }
            for (auto& a : alpha) {
                newProductions.push_back({newNonTerminal, {a + " " + newNonTerminal}});
            }
            newProductions.push_back({newNonTerminal, {"epsilon"}});
        } else {
            newProductions.push_back(prod);
        }
    }
    productions = newProductions;
}

/// @brief 消除间接左递归
/// @param productions 
void eliminateIndirectLeftRecursion(vector<Production>& productions) {
    unordered_map<string, vector<string>> prodMap;
    for (auto& prod : productions) {
        prodMap[prod.lhs].insert(prodMap[prod.lhs].end(), prod.rhs.begin(), prod.rhs.end());
    }
    for (size_t i = 0; i < productions.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            vector<string> newRhs;
            for (auto& rhs : productions[i].rhs) {
                if (rhs.substr(0, productions[j].lhs.size()) == productions[j].lhs) {
                    for (auto& subRhs : productions[j].rhs) {
                        newRhs.push_back(subRhs + " " + rhs.substr(productions[j].lhs.size()));
                    }
                } else {
                    newRhs.push_back(rhs);
                }
            }
            productions[i].rhs = newRhs;
        }
        eliminateDirectLeftRecursion(productions);
    }
}

void printProductions(const vector<Production>& productions) {
    for (const auto& prod : productions) {
        cout << prod.lhs << " -> ";
        for (size_t i = 0; i < prod.rhs.size(); ++i) {
            cout << prod.rhs[i] << ' ';
            if (i < prod.rhs.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

int main() {
    vector<Production> productions = {
        {"Grammar", {"ProductionList"}},
        {"ProductionList", {"ProductionSet SEMICOLON", "ProductionList ProductionSet SEMICOLON"}},
        {"ProductionSet", {"SYMBOL DERIVES RightHandSide", "ProductionSet ALSODERIVES RightHandSide"}},
        {"RightHandSide", {"SymbolList", "EPSILON"}},
        {"SymbolList", {"SymbolList SYMBOL", "SYMBOL"}}
    };

    cout << "Original Productions:" << endl;
    printProductions(productions);

    eliminateIndirectLeftRecursion(productions);

    cout << "\nProductions after eliminating left recursion:" << endl;
    printProductions(productions);

    return 0;
}