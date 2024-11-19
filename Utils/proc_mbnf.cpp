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
                newProductions.push_back({prod.lhs, {b + newNonTerminal}});
            }
            for (auto& a : alpha) {
                newProductions.push_back({newNonTerminal, {a + newNonTerminal}});
            }
            newProductions.push_back({newNonTerminal, {"ε"}});
        } else {
            newProductions.push_back(prod);
        }
    }
    productions = newProductions;
}

void eliminateIndirectLeftRecursion(vector<Production>& productions) {
    unordered_map<string, vector<string>> prodMap;
    for (auto& prod : productions) {
        prodMap[prod.lhs].insert(prodMap[prod.lhs].end(), prod.rhs.begin(), prod.rhs.end());
    }
    for (auto& prod : productions) {
        for (auto& rhs : prod.rhs) {
            if (prodMap.find(rhs.substr(0, rhs.find(' '))) != prodMap.end()) {
                vector<string> newRhs;
                for (auto& subRhs : prodMap[rhs.substr(0, rhs.find(' '))]) {
                    newRhs.push_back(subRhs + rhs.substr(rhs.find(' ')));
                }
                prod.rhs = newRhs;
            }
        }
    }
    eliminateDirectLeftRecursion(productions);
}

void printProductions(const vector<Production>& productions) {
    for (const auto& prod : productions) {
        cout << prod.lhs << " -> ";
        for (size_t i = 0; i < prod.rhs.size(); ++i) {
            cout << prod.rhs[i];
            if (i < prod.rhs.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

int main() {
    vector<Production> productions = {
        {"A", {"Aa", "b"}},
        {"B", {"Bc", "d"}}
    };

    cout << "Original Productions:" << endl;
    printProductions(productions);

    eliminateIndirectLeftRecursion(productions);

    cout << "\nProductions after eliminating left recursion:" << endl;
    printProductions(productions);

    return 0;
}