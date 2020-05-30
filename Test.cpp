//
// Created by Semir on 2020/5/2.
//

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "DataStructure.h"
#include "Operation.h"

using namespace std;

//typedef string Sign;
//
//typedef vector<Sign> Formula;
//
//typedef map<Sign, set<Formula>> Productions;
//
//void printFormula(const Formula& formula) {
//    for (const auto & i : formula) {
//        cout << i;
//    }
//}
//
//void printFormulaEdl(const Formula &formula) {
//    for (const auto & i : formula) {
//        cout << i;
//    }
//    cout << endl;
//}
//
//void printProductions(Productions &productions) {
//    //map<Symbol, set<Formula>>::iterator
//    for (auto & production : productions) {
//        cout << production.first << " -> ";
//        int flag = false;
//        for (auto & formula : production.second) {
//            if (flag) {
//                cout << " | ";
//            } else {
//                flag = true;
//            }
//            printFormula(formula);
//        }
//        cout << endl;
//    }
//}


void test01() {
    Sign symbol1 = "E''";
    Sign symbol2 = "E''";
    cout << symbol1.length() << endl;
    cout << symbol2.length() << endl;
    bool flag = symbol1 == symbol2;
    cout << flag << endl;
}

void test02() {
    Formula formula;
    formula.push_back("(");
    formula.push_back("T''");
    formula.push_back(")");
    cout << formula.size() << endl;
    printFormulaEdl(formula);
}

void test03() {
    Productions productions;
    Sign s1 = "E'";
    Sign s2 = "+";
    Sign s3 = "T";
    Formula f1 = {s1, s2, s3};
    Formula f2 = {s3};
    Productions p;
    p[s1].insert(f1);
    p[s1].insert(f2);
//    p.first = s1;
//    p.second.insert(f1);
//    p.second.insert(f2);
    printProductions(p);
}

void test04() {
    string str = "helloWorld";
    string sub = str.substr(3, 6 - 3);
    cout << sub << endl;
}

void test05() {
    Formula formula;
    Sign sign = "Hello";
    formula.push_back(sign);
    sign = " ";
    formula.push_back(sign);
    sign = "World";
    formula.push_back(sign);
    cout << formula.size() << endl;
}

void test06() {
    set<Formula> formulaSet;
    Formula formula;
    formula = {"Hello", " ", "Hello"};
    formulaSet.insert(formula);
    formula.clear();
    formula = {"World", " ", "World"};
    formulaSet.insert(formula);
    formula.clear();
    cout << formulaSet.size() << endl;
}

void test07() {
    string str = "he";
    if (str.length() < 3) {
        throw "ERROR";
    }
    cout << str << endl;
}

void test08() {
    try {
        test07();
    } catch (const char* msg) {
        cerr << msg;
    }
}

void test09() {
    Sign sign;
    cout << "a" << sign << "b" << endl;
    cout << sign.length() << endl;
}

void test10() {
    string str = "R -> aR'' | caR' | bcaR''||~ ";
    int index = 0;
    Sign sign;
    while (index < str.length()) {
        sign = getNextSign(str, index);
        cout << "----------------------------" << endl;
        cout << "length: " << sign.length() << endl;
        cout << sign << endl;
        cout << "index: " << index << endl;
        cout << "----------------------------" << endl;
    }
}

void test11() {
    vector<string> strings;
    strings.emplace_back("S -> Qc | c");
    strings.emplace_back("Q -> Rb | b");
    strings.emplace_back("R -> aR' | caR' | bcaR'");
    strings.emplace_back("R' -> bcaR' | ~");
    Productions productions = makeProductions(strings);
    cout << productions.size() << endl;
}

void test12() {
    vector<string> strings;
    strings.emplace_back("S -> Qc | c");
    strings.emplace_back("Q -> Rb | b");
    strings.emplace_back("R -> Sa | a | c");
    Productions productions = makeProductions(strings);
    bool b = hasExplicitRecursion(productions);
    cout << b << endl;
}

void test13() {
    set<string> strSet;
    strSet.insert("Hello");
    strSet.insert("Hello");
    strSet.insert("Hello");
    set<string> sss;
    sss.insert("aaa");
    sss.insert("bbb");
    sss.insert("ccc");
    strSet.insert(sss.begin(), sss.end());
    cout << strSet.size() << endl;

}

void test14() {
    vector<string> vec1;
    vec1.emplace_back("Hello");
    vec1.emplace_back("World");
    vector<string> vec2;
    vec2.emplace_back("Hey");
    vec2.emplace_back("Low");
}

void test15() {
    set<string> sss;
    sss.insert("aaa");
    sss.insert("bbb");
    sss.insert("ccc");
    int i = 0;
    for (auto s : sss) {
        cout << s << endl;
        s = "";
        s += 'A' + i;
        i++;
        cout << s << endl;
    }
}

void test16() {
    vector<string> strings;
    strings.emplace_back("S -> Qc | c | Sb");
    strings.emplace_back("Q -> Rb | b");
    strings.emplace_back("R -> Sa | a | Ra");
    Productions productions = makeProductions(strings);
    exposeHiddenRecursion(productions);
    cout << productions.size() << endl;
}

void test17() {
    vector<string> strings;
    strings.emplace_back("S->Aa | b");
    strings.emplace_back("A -> Ac | Sd | ~");
    Productions productions = makeProductions(strings);
    exposeHiddenRecursion(productions);
    cout << productions.size() << endl;
}

void test18() {
    vector<string> strings;
    map<Sign, bool> vis;
    Sign sign = "Q";
    strings.emplace_back("S -> Rbc | c | Sb | bc");
    strings.emplace_back("Q -> Rb | b");
    strings.emplace_back("R -> Sa | a | Ra");
    Productions productions = makeProductions(strings);
    bool b = hasHiddenRecursion(productions);
//    bool b = hasHiddenRecursionDfs(productions, sign, vis, sign);
    cout << productions.size() << endl;
    cout << b << endl;
}

void test19() {
    vector<string> strings;
    map<Sign, bool> vis;
    Sign sign = "Q";
    strings.emplace_back("S -> Rbc | c | Sb | bc");
    strings.emplace_back("Q -> Rb | b");
    strings.emplace_back("R -> Sa | a | Ra");
    Productions productions = makeProductions(strings);
    exposeHiddenRecursion(productions);
    cout << productions.size() << endl;
}


void test20() {
    /*  E -> E + T | T
        T -> T * F | F
        F -> (E) | id
     */
    vector<string> strings;
    map<Sign, bool> vis;
    Sign sign = "E";
    strings.emplace_back("E -> E + T | T");
    strings.emplace_back("T -> T * F | F");
    strings.emplace_back("F -> (E) | id");
    Productions productions = makeProductions(strings);
    eliminateExplicitRecursion(productions, sign);
    cout << productions.size() << endl;
}

void test21() {
    /*  E -> E + T | T
        T -> T * F | F
        F -> (E) | id
     */
    vector<string> strings;
    map<Sign, bool> vis;
    Sign sign = "T";
    strings.emplace_back("S -> (T) | a+S | a");
    strings.emplace_back("T -> T,S | S");
    Productions productions = makeProductions(strings);
    eliminateExplicitRecursion(productions, sign);
    cout << productions.size() << endl;
}

void test22() {
    vector<string> strings;
    strings.emplace_back("E -> E + T | T");
    strings.emplace_back("T -> T * F | F");
    strings.emplace_back("F -> (E) | id");
    Productions productions = makeProductions(strings);
    eliminateRecursion(productions);
    productionsToStringVector(productions, strings);
    printStringVector(strings);
//    cout << productions.size() << endl;
}

void test23() {
    vector<string> strings;
    strings.emplace_back("S -> (T) | a+S | a");
    strings.emplace_back("T -> T,S | S");
//    strings.emplace_back("F -> (E) | id");
    Productions productions = makeProductions(strings);
    extractFactor(productions);
    cout << productions.size() << endl;
}

void test24() {
    vector<int> vector1 = {1,2,3,4,5,6,7};
    vector<int> vector2;
    vector2.assign(vector1.begin() + 1, vector1.end());
    cout << vector2.size() << endl;
}

void test25() {
    set<int> a = {1,2,3,4,5,6,7,8,9,0};
    set<int> b = {1,4,6,9,10};
    set<int> res;
    set_difference(b.begin(), b.end(), a.begin(), a.end(), inserter(res, res.begin()));
    cout << res.size() << endl;
}

int main() {
    test25();

    return 0;
}

