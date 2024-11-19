#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

class LeftRecursionEliminator {
public:
    void eliminateLeftRecursion(std::unordered_map<std::string, std::vector<std::string>>& productions) {
        for (auto it = productions.begin(); it != productions.end(); ++it) {
            std::string A = it->first;
            std::vector<std::string> alpha, beta;

            for (const std::string& production : it->second) {
                if (production[0] == A[0]) {
                    alpha.push_back(production.substr(1));
                } else {
                    beta.push_back(production);
                }
            }

            if (!alpha.empty()) {
                std::string A_prime = A + "'";
                productions[A].clear();
                for (const std::string& b : beta) {
                    productions[A].push_back(b + A_prime);
                }
                for (const std::string& a : alpha) {
                    productions[A_prime].push_back(a + A_prime);
                }
                productions[A_prime].push_back("EPSILON");
            }
        }
    }
};

// 示例用法
int main() {
    std::unordered_map<std::string, std::vector<std::string>> productions = {
        {"Grammar", {"ProductionList"}},
        {"ProductionList", {"ProductionSet SEMICOLON", "ProductionList ProductionSet SEMICOLON"}},
        {"ProductionSet", {"SYMBOL DERIVES RightHandSide", "ProductionSet ALSODERIVES RightHandSide"}},
        {"RightHandSide", {"SymbolList", "EPSILON"}},
        {"SymbolList", {"SymbolList SYMBOL", "SYMBOL"}}
    };

    LeftRecursionEliminator eliminator;
    eliminator.eliminateLeftRecursion(productions);

    for (const auto& pair : productions) {
        std::cout << pair.first << " -> ";
        for (const auto& prod : pair.second) {
            std::cout << prod << " | ";
        }
        std::cout << std::endl;
    }

    return 0;
}