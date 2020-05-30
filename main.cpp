#include "DataStructure.h"
#include "Operation.h"


int main() {
    showHelp();
    vector<string> strings;
    string str;
    Sign start;
    getline(cin, start);
    while (true) {
        getline(cin, str);
        if (str.length() == 0) {
            break;
        }
        strings.emplace_back(str);
    }

    //构造Productions数据结构
    Productions productions = makeProductions(strings);

    //判断输入的文法开始符号是否合法
    try {
         if (productions.count(start) == 0) {
             throw "Input Error";
         }
    } catch (const char* msg) {
        cerr << msg << endl;
        exit(1);
    }

    //消除左递归
    eliminateRecursion(productions);
    cout << "After Elimination: " << endl;
    productionsToStringVector(productions, strings);
    printStringVector(strings);
    cout << endl;

    //提取左因子
    extractFactor(productions);
    cout << "After Extraction: " << endl;
    productionsToStringVector(productions, strings);
    printStringVector(strings);
    cout << endl;

    //求FIRST集
    FinalFirstNodeMap finalFirstNodeMap = getFirst(productions);
    printFirstSet(finalFirstNodeMap);
    cout << endl;

    //求FOLLOW集
    FinalFollowNodeMap finalFollowNodeMap = getFollow(start, productions, finalFirstNodeMap);
    printFollowSet(finalFollowNodeMap);
    cout << endl;

    //构造预测分析表
    PredictionMap predictionMap = getPredictionMap(productions, finalFirstNodeMap, finalFollowNodeMap);
    printPredictionMap(predictionMap);
    cout << endl;

    bool isLL1 = checkLL1(predictionMap);
    if (isLL1) {
        cout << "It is a LL1" << endl;
        cout << "Input the grammar to match:" << endl << endl;
        string stringToMatch;
        getline(cin, stringToMatch);
        cout << endl;
        bool match;
        try {
            //适用下推自动机分析输入的字符串
            match = analysis(predictionMap, start, stringToMatch);
        } catch (const char* msg) {
            cerr << msg << endl;
            exit(1);
        }
        if (match) {
            cout << "The string \"" << stringToMatch << "\" is matched whit this LL1" << endl;
        } else {
            cout << "The string \"" << stringToMatch << "\" is NOT matched whit this LL1" << endl;
        }
    } else {
        cout << "It is NOT a LL1" << endl;
    }

    system("pause");
    return 0;
}
