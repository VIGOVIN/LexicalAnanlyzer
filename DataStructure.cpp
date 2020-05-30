//
// Created by Semir on 2020/5/2.
//

#include "DataStructure.h"

Formula stringToFormula(const string& str) {
    Formula formula;
    for (char i : str) {
        Sign sign;
        sign += i;
        formula.emplace_back(sign);
    }
    return formula;
}

void printFormula(const Formula& formula) {
    for (const auto & i : formula) {
        cout << i;
    }
}

void printFormulaEdl(const Formula &formula) {
    for (const auto & i : formula) {
        cout << i;
    }
    cout << endl;
}

void printProductions(Productions &productions) {
    for (auto & production : productions) {
        cout << production.first << " -> ";
        int flag = false;
        for (auto & formula : production.second) {
            if (flag) {
                cout << " | ";
            } else {
                flag = true;
            }
            printFormula(formula);
        }
        cout << endl;
    }
}

void printStringVector(const vector<string>& vec) {
    for (const auto& s : vec) {
        cout << s << endl;
    }
}

FirstNodeMap createFirstNodeMap(const Productions& productions) {
    FirstNodeMap firstNodeMap;
    firstNodeMap.clear();
    for (const auto& p : productions) {
        Sign sign = p.first;
        FirstNode firstNode;
        firstNode.finished = false;
        for (const auto& formula : p.second) {
            firstNode.formulaSet.insert(formula);
        }
        firstNodeMap[sign] = firstNode;
    }
    return firstNodeMap;
}

FinalFirstNodeMap createFinalFirstNodeMap(const FirstNodeMap& firstNodeMap) {
    FinalFirstNodeMap finalFirstNodeMap;
    try {
        for (const auto& p : firstNodeMap) {
            if (!p.second.finished || !p.second.formulaSet.empty()) {
                throw "FIRST Set Unfinished";
            }
            Sign sign = p.first;
            for (const auto& s : p.second.firstSet) {
                finalFirstNodeMap[sign].insert(s);
            }
        }
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    return finalFirstNodeMap;
}

void printFirstSet(const FinalFirstNodeMap& finalFirstNodeMap) {
    cout << "FIRST SET:" << endl;
    for (const auto& p : finalFirstNodeMap) {
        cout << p.first << "\t";
        for (const auto& s : p.second) {
            cout << s << " ";
        }
        cout << endl;
    }
}

void printFollowSet(const FinalFollowNodeMap& finalFollowNodeMap) {
    cout << "FOLLOW SET:" << endl;
    for (const auto& p : finalFollowNodeMap) {
        cout << p.first << "\t";
        for (const auto& s : p.second) {
            cout << s << " ";
        }
        cout << endl;
    }
}

//typedef map<Sign, map<Sign, set<Formula>>> PredictionMap;

void printPredictionMap(const PredictionMap& predictionMap) {
    cout << "Prediction Analysis Map:" << endl;
    for (const auto& pOut : predictionMap) {
        Sign targetOut = pOut.first;
        cout << targetOut << "\t";
        bool commaFlag = false;
        for (const auto& pIn : pOut.second) {
            Sign targetIn = pIn.first;
            if (commaFlag) {
                cout << ",";
            }
            cout << "{" << targetIn << ":";
            bool backslash = false;
            for (const auto& formula : pIn.second) {
                if (backslash) {
                    cout << "/";
                }
                printFormula(formula);
                backslash = true;
            }
            cout << "}";
            commaFlag = true;
        }
        cout << endl;
    }
}




