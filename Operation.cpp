//
// Created by Semir on 2020/5/2.
//

#include "Operation.h"

void showHelp() {
    cout << "Run this exe then input.\n"
         << "The first line is the start sign of the grammar\n"
         << "The others are productions\t"
         << "Format: A -> xxx | xxx\n"
         << "Input '~' for epsilon\n"
         << "Input an empty line for end\n\n";
}

bool isNonTerminator(Sign sign) {
    if (sign.length() == 0) {
        throw ERROR_MSG;
    }
    return sign[0] - 'A' >= 0 && sign[0] - 'A' < 26;
}

bool isNonTerminator(char c) {
    return c - 'A' >= 0 && c - 'A' < 26;
}

/**
 * 从index开始获取下一个符号 如果是空格或者制表符号 则忽略  需要更新index
 * @param str
 * @param index
 * @return
 */
Sign getNextSign(const string& str, int& index) {
    if (index >= str.length()) {
        throw ERROR_MSG;
    }
    Sign sign;
    while (index < str.length() && (str[index] == ' ' || str[index] == '\t')) {
        index++;
    }
    if (isNonTerminator(str[index])) {  //非终结符
        sign += str[index];
        index++;
        while (index < str.length() && str[index] == '\'') {
            sign += str[index];
            index++;
        }
    } else if (str[index] == '-') {
        sign += str[index];
        index++;
        if (index < str.length() && str[index] == '>') {
            sign += str[index];
            index++;
        }
    } else if (str[index] == '|') {
        sign += str[index];
        index++;
    } else if (str[index] == 'i') {     //特殊处理id
        sign += str[index];
        index++;
        if (index < str.length() && str[index] == 'd') {
            sign += str[index];
            index++;
        }
    } else {
        sign += str[index];
        index++;
    }
    while (index < str.length() && (str[index] == ' ' || str[index] == '\t')) {
        index++;
    }
    return sign;
}

/**
 * 根据符号获取其类型 方便进一步处理
 * @param sign
 * @return
 */
int getSignType(const string& sign) {
    if (isNonTerminator(sign[0])) {
        return NON_TERMINATOR;
    }
    if (sign == "->") {
        return ARROW;
    }
    if (sign == "|") {
        return DIVIDER;
    }
    return TERMINATOR;
}

/**
 * 将一个字符串转化为 Productions 中的一个元素
 * @param productions
 * @param str
 */
void stringIntoProductions(Productions &productions, const string& str) {
    if (str.find_first_of("->") < 0) {
        throw ERROR_MSG;
    }
    bool leftFlag = true;
    bool hasArrow = false;
    int index = 0;
    Sign tempSign;
    Sign left;
    Formula formula;
    set<Formula> formulaSet;
    while (index < str.length()) {
        tempSign = getNextSign(str, index);
        int type = getSignType(tempSign);
        switch (type) {
            case NON_TERMINATOR:
                if (leftFlag) {
                    left = tempSign;
                    leftFlag = false;
                } else {
                    formula.push_back(tempSign);
                }
                break;
            case ARROW:
                if (leftFlag) {
                    throw "ERROR: No Left";
                } else if (hasArrow) {
                    throw "ERROR: Multi Arrow";
                }
                hasArrow = true;
                break;
            case DIVIDER:
                if (formula.size() == 0) {
                    throw "ERROR: Serial Divider";
                }
                formulaSet.insert(formula);
                formula.clear();
                break;
            case TERMINATOR:
                if (leftFlag) {
                    throw "ERROR: Left can NOT be Terminator";
                }
                formula.push_back(tempSign);
                break;
            default:
                break;
        }
    }
    if (formula.size() > 0) {
        formulaSet.insert(formula);
    }
    if (productions.count(left) > 0) {
        throw "ERROR: Repeat Left";
    }
    productions[left] = formulaSet;
}

/**
 * 将字符串数组转化为一个 Production
 * @param strings
 * @return
 */
Productions makeProductions(vector<string>& strings) {
    Productions productions;
    try {
        for (const auto& str : strings) {
            stringIntoProductions(productions, str);
        }
    } catch (const char* msg) {
        cerr << msg << endl;
    }
    return productions;
}

/**
 * 判断单个产生式是否含有隐式左递归 使用DFS 需要传入辅助的数据结构vis
 * @param productions
 * @param target
 * @param vis 用于记录已经被访问过的非终结符 通过他是否可以产生target 外部调用时需要保证刚传入来的vis是空的
 * @param left
 * @return
 */
bool hasHiddenRecursionDfs(Productions& productions, const Sign& target,
        map<Sign, bool>& vis, const Sign& left) {
    if (vis.count(left) > 0) {
        return vis[left];
    }
    vis[left] = false;  //防止无限递归
    bool res = false;
    for (auto formula : productions[left]) {
        if (isNonTerminator(formula[0]) && formula[0] != left) {
            if (formula[0] == target) {
                res = true;
                break;
            }
            res = res || hasHiddenRecursionDfs(productions, target, vis, formula[0]);
            if (res) {
                break;
            }
        }
    }
    vis[left] = res;
    return res;
}

/**
 * 判断一组产生式是否包含隐式左递归
 * @param productions
 * @return
 */
bool hasHiddenRecursion(Productions& productions) {
    map<Sign, bool> vis;
    for (const auto& p : productions) {
        Sign left = p.first;
        for (auto formula : p.second) {
            if (isNonTerminator(formula[0]) && formula[0] != left) {
                if (hasHiddenRecursionDfs(productions, left, vis, left)) {
                    return true;
                }
            }
        }
        vis.clear();
    }
    return false;
}

/**
 * 判断产生式是否存在显示左递归
 * @param productions
 * @param left
 * @return
 */
bool hasExplicitRecursion(Productions& productions, const Sign& left) {
    bool res = false;
    for (auto formula : productions[left]) {
        if (formula[0] == left) {
            res = true;
            break;
        }
    }
    return res;
}

bool hasExplicitRecursion(const Sign& left, const set<Formula>& formulaSet) {
    bool res = false;
    for (auto formula : formulaSet) {
        if (formula[0] == left) {
            res = true;
            break;
        }
    }
    return res;
}

bool hasExplicitRecursion(Productions& productions) {
    bool res = false;
    for (const auto& p : productions) {
        if (hasExplicitRecursion(p.first, p.second)) {
            res = true;
            break;
        }
    }
    return res;
}


void insertIntoFormulaSet(set<Formula>& toSet, const Formula& origin, const set<Formula>& fromSet) {
    for (auto formula : fromSet) {
        if (formula[0] == EPSILON) {
            formula.clear();
        }
        for (int i = 1; i < origin.size(); i++) {
            formula.emplace_back(origin[i]);
        }
        toSet.insert(formula);
    }
}

/**
 * 将 key为target的产生式 的 隐式左递归暴露出来
 * @param productions
 * @param target
 * @return 1-成功暴露，返回true  0-无法暴露，返回false
 */
bool exposeOne(Productions& productions, const Sign& target) {
    bool res = true;
    set<Formula> formulaSet;
    for (auto formula : productions[target]) {
        if (isNonTerminator(formula[0]) && productions.count(formula[0]) > 0 && formula[0] != target) {
            if (hasExplicitRecursion(productions, formula[0])) {
                res = false;
                break;
            } else {
                insertIntoFormulaSet(formulaSet, formula, productions[formula[0]]);
            }
        } else {    //终结符和以target开头的式子
            formulaSet.insert(formula);
        }
    }
    if (res) {
        productions[target] = formulaSet;
    }
    return res;
}

/**
 * 暴露产生式中的隐式左递归
 * @param productions
 */
void exposeHiddenRecursion(Productions& productions) {
    try {
        map<Sign, bool> auxMap;
        bool hasExposeOne;
        while (true) {
            hasExposeOne = false;
            for (const auto& p : productions) {
                auxMap.clear();
                if (hasHiddenRecursionDfs(productions, p.first, auxMap, p.first)) {
                    //尝试将隐式递归显现出来
                    if (exposeOne(productions, p.first)) {  //成功地暴露一个 含有隐式左递归的 产生式
                        hasExposeOne = true;
                        // 如果成功暴露了一个隐式左递归 说明修改了productions 必须重新遍历productions
                        break;
                    }
                }
            }
            /**
             * 如果一轮下来还不能暴露一个隐式左递归 那么表明这个左递归不能消除
             */
            if (hasExposeOne) {
                if (!hasHiddenRecursion(productions)) {
                    break;
                }
            } else {
                throw "ERROR: Can NOT Expose Hidden Recursion";
            }
        }
    } catch (const char* msg) {
        cerr << msg << endl;
    }
}

/**
 * 消除显式的左递归
 * @param productions
 * @param target
 */
void eliminateExplicitRecursion(Productions& productions, const Sign& target) {
    Sign newSign = target;
    set<Formula> newSet;
    set<Formula> originSet;
    newSign += '\'';
    while (productions.count(newSign) > 0) {
        newSign += '\'';
    }
    for (auto formula : productions[target]) {
        if (formula[0] == target) {
            formula.erase(formula.begin());
            formula.emplace_back(newSign);
            newSet.insert(formula);
        } else {
            if (formula[0] == EPSILON) {
                formula.clear();
            }
            formula.emplace_back(newSign);
            originSet.insert(formula);
        }
    }
    newSet.insert({EPSILON});
    productions[target] = originSet;
    productions[newSign] = newSet;
}

/**
 * 消除左递归
 * @param productions
 */
void eliminateRecursion(Productions& productions) {
    if (hasHiddenRecursion(productions)) {
        exposeHiddenRecursion(productions);
    }
    bool flag;
    while (true) {
        flag = false;
        for (const auto& p : productions) {
            if (hasExplicitRecursion(p.first, p.second)) {
                eliminateExplicitRecursion(productions, p.first);
                flag = true;
                break;
            }
        }
        if (!flag) {
            break;
        }
    }
}


void appendFormulaToString(string& str, const Formula& formula) {
    for (const auto & i : formula) {
        str += i;
    }
}

/**
 * 将productions转化为字符串数组
 * @param productions
 * @param strings
 */
void productionsToStringVector(const Productions& productions, vector<string>& strings) {
    strings.clear();
    string tempStr;
    for (auto & production : productions) {
        tempStr = "";
        tempStr += production.first;
        tempStr += " -> ";
        bool flag = false;
        for (const auto& formula : production.second) {
            if (flag) {
                tempStr += " | ";
            } else {
                flag = true;
            }
            appendFormulaToString(tempStr, formula);
        }
        strings.emplace_back(tempStr);
    }
}


/**
 * 若有进行提取左因子 返回true 否则返回false
 * @param productions
 * @param target
 * @return
 */
bool extractOne(Productions& productions, const Sign& target) {
    set<Sign> pool;
    Sign markSign;
    for (auto formula : productions[target]) {
        if (pool.count(formula[0]) > 0) {
            markSign = formula[0];
            break;
        } else {
            pool.insert(formula[0]);
        }
    }
    if (markSign.length() > 0) {
        set<Formula> newSet;
        Sign newSign = target + '\'';
        while (productions.count(newSign) > 0) {
            newSign += '\'';
        }
        bool flag;
        while (true) {
            flag = false;
            for (auto formula : productions[target]) {
                if (formula[0] == markSign) {
                    productions[target].erase(formula);
                    formula.erase(formula.begin());
                    if (!formula.empty()) {
                        newSet.insert(formula);
                    } else {
                        newSet.insert({EPSILON});
                    }
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                break;
            }
        }

        productions[target].insert({markSign, newSign});
        productions[newSign] = newSet;
        return true;
    } else {
        return false;
    }
}

/**
 * 提取左因子
 * @param productions
 */
void extractFactor(Productions& productions) {
    bool flag;
    while (true) {
        flag = false;
        for (const auto& p : productions) {
            if (extractOne(productions, p.first)) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            break;
        }
    }
}


/**
 * 获取FIRST集的主方法
 * @param firstNodeMap
 * @param target
 */
void getFirstDfs(FirstNodeMap& firstNodeMap, const Sign& target) {
    if (firstNodeMap[target].finished) {
        return;
    }
    while (!firstNodeMap[target].formulaSet.empty()) {
        Formula formula = *firstNodeMap[target].formulaSet.begin();
        if (!isNonTerminator(formula[0])) {     //如果是终结符
            firstNodeMap[target].firstSet.insert(formula[0]);
            firstNodeMap[target].formulaSet.erase(formula);
        } else {    //如果是非终结符
            if (formula[0] == target) {
                continue;
            }
            if (!firstNodeMap[formula[0]].finished) {
                getFirstDfs(firstNodeMap, formula[0]);
            }
            bool hasEpsilon = false;
            for (const auto& s : firstNodeMap[formula[0]].firstSet) {
                if (s == EPSILON) {
                    hasEpsilon = true;
                    if (formula.size() == 1) {
                        firstNodeMap[target].firstSet.insert(EPSILON);
                    }
                } else {
                    firstNodeMap[target].firstSet.insert(s);
                }
            }
            if (hasEpsilon) {
                Formula newFormula(formula.size() - 1);
                newFormula.assign(formula.begin() + 1, formula.end());
                if (!newFormula.empty()) {
                    firstNodeMap[target].formulaSet.insert(newFormula);
                }
            }
            firstNodeMap[target].formulaSet.erase(formula);
        }
    }
    if (firstNodeMap[target].formulaSet.empty()) {
        firstNodeMap[target].finished = true;
    } else {
        throw "Find FIRST Set ERROR";
    }
}

/*
void getFirstDfs(FirstNodeMap& firstNodeMap, const Sign& target) {
    if (firstNodeMap[target].finished) {
        return;
    }
    Formula formulaToErase;
    Formula formulaToInsert;
    do {
        formulaToErase.resize(0);
        formulaToInsert.resize(0);

        for (auto formula : firstNodeMap[target].formulaSet) {
            if (!isNonTerminator(formula[0])) {     //如果是终结符
                firstNodeMap[target].firstSet.insert(formula[0]);
                // firstNodeMap[target].formulaSet.erase(formula);
                formulaToErase = formula;
            } else {    //如果是非终结符
                if (formula[0] == target) {
                    continue;
                }

                if (!firstNodeMap[formula[0]].finished) {
                    getFirstDfs(firstNodeMap, formula[0]);
                }

                bool hasEpsilon = false;
                for (const auto& s : firstNodeMap[formula[0]].firstSet) {
                    if (s == EPSILON) {
                        hasEpsilon = true;
                        if (formula.size() == 1) {
                            firstNodeMap[target].firstSet.insert(EPSILON);
                        }
                    } else {
                        firstNodeMap[target].firstSet.insert(s);
                    }
                }
                if (hasEpsilon) {
                    Formula newFormula(formula.size() - 1);
                    newFormula.assign(formula.begin() + 1, formula.end());
                    // firstNodeMap[target].formulaSet.insert(newFormula);
                    formulaToInsert = newFormula;
                }
                formulaToErase = formula;
            }
            break;
        }

        if (!formulaToErase.empty()) {
            firstNodeMap[target].formulaSet.erase(formulaToErase);
        }
        if (!formulaToInsert.empty()) {
            firstNodeMap[target].formulaSet.insert(formulaToInsert);
        }

    } while (!formulaToErase.empty() || !formulaToInsert.empty());

    if (firstNodeMap[target].formulaSet.empty()) {
        firstNodeMap[target].finished = true;
    } else {
        throw "Find FIRST Set ERROR";
    }
}
*/

/**
 * 获取First集
 * @param productions
 * @param firstNodeMap
 */
FinalFirstNodeMap getFirst(const Productions& productions) {
    FirstNodeMap firstNodeMap = createFirstNodeMap(productions);
    for (const auto& p : firstNodeMap) {
        if (!p.second.finished) {
            try {
                getFirstDfs(firstNodeMap, p.first);
            } catch (const char* msg) {
                cerr << msg << endl;
                exit(1);
            }
        }
    }
    FinalFirstNodeMap finalFirstNodeMap = createFinalFirstNodeMap(firstNodeMap);
    return finalFirstNodeMap;
}

/**
 * 根据FIRST集填充finalFollowNodeMap
 * @param productions
 * @param finalFirstNodeMap
 * @param finalFollowNodeMap
 */
void fillFinalFollowNodeMapWithFinalFirstNodeMap(const Productions& productions, const FinalFirstNodeMap& finalFirstNodeMap,
        FinalFollowNodeMap& finalFollowNodeMap) {
    Sign target;
    for (const auto& p : productions) {
        target = p.first;
        for (const auto& formula : p.second) {
            for (int i = 0; i < formula.size(); i++) {
                if (isNonTerminator(formula[i]) && i + 1 < formula.size()) {
                    int k = i + 1;
                    if (isNonTerminator(formula[k])) {
                        for (const auto& first : finalFirstNodeMap.at(formula[k])) {
                            if (first != EPSILON) {
                                finalFollowNodeMap[formula[i]].insert(first);
                            }
                        }
                    } else {
                        finalFollowNodeMap[formula[i]].insert(formula[k]);
                    }
                }
            }
        }
    }
}

/**
 * 根据产生式集合和FIRST集填充辅助的数据结构FollowLinks
 * @param productions
 * @param finalFirstNodeMap
 * @param followLinks
 */
void fillFollowLinks(const Productions& productions, const FinalFirstNodeMap& finalFirstNodeMap, FollowLinks& followLinks) {
    Sign target;
    for (const auto& p : productions) {
        target = p.first;
        for (const auto &formula : p.second) {
            for (int i = 0; i < formula.size(); i++) {
                if (formula[i] == target) {
                    continue;
                }
                if (isNonTerminator(formula[i])) {
                    int k = i + 1;
                    if (k >= formula.size() || (isNonTerminator(formula[k]) &&
                            finalFirstNodeMap.at(formula[k]).count({EPSILON}) > 0)) {
                        followLinks.insert({target, formula[i]});
                    }
                }
            }
        }
    }
}

/**
 * 根据followLinks完善finalFollowNodeMap
 * @param followLinks
 * @param finalFollowNodeMap
 */
void fillFinalFollowNodeMapWithFollowLinks(const FollowLinks& followLinks, FinalFollowNodeMap& finalFollowNodeMap) {
    bool hasChanged;
    set<Sign> auxSet;
    while (true) {
        hasChanged = false;
        for (const auto& link : followLinks) {
            auxSet.clear();
            set_difference(finalFollowNodeMap[link.first].begin(), finalFollowNodeMap[link.first].end(),
                    finalFollowNodeMap[link.second].begin(), finalFollowNodeMap[link.second].end(),
                    inserter(auxSet, auxSet.begin()));
            if (!auxSet.empty()) {
                hasChanged = true;
                for (const auto& follow : finalFollowNodeMap[link.first]) {
                    finalFollowNodeMap[link.second].insert(follow);
                }
            }
        }
        if (!hasChanged) {
            break;
        }
    }
}

/**
 * 获取Follow集
 * @param productions
 * @param finalFirstNodeMap
 * @return
 */
FinalFollowNodeMap getFollow(const Sign& start, const Productions& productions,
        const FinalFirstNodeMap& finalFirstNodeMap) {
    FinalFollowNodeMap finalFollowNodeMap;
    FollowLinks followLinks;

    //添加“#”
    finalFollowNodeMap[start].insert(END_MARK);
    //根据FIRST集填充finalFollowNodeMap
    fillFinalFollowNodeMapWithFinalFirstNodeMap(productions, finalFirstNodeMap, finalFollowNodeMap);
    //根据Productions填充followLinks
    fillFollowLinks(productions, finalFirstNodeMap, followLinks);
    //根据followLinks完善finalFollowNodeMap
    fillFinalFollowNodeMapWithFollowLinks(followLinks, finalFollowNodeMap);

    return finalFollowNodeMap;
}

/**
 * 判断非终结符matchHead是否能够匹配matchItem
 * @param finalFirstNodeMap
 * @param finalFollowNodeMap
 * @param matchHead
 * @param matchItem
 * @return
 */
bool checkSuitable(const FinalFirstNodeMap & finalFirstNodeMap,
        const FinalFollowNodeMap& finalFollowNodeMap, const Sign& matchHead, const Sign& matchItem) {
    bool hasEpsilon = false;
    for (const auto& first : finalFirstNodeMap.at(matchHead)) {
        if (first == matchItem) {
            return true;
        }
        if (first == EPSILON) {
            hasEpsilon = true;
        }
    }
    if (hasEpsilon) {
        for (const auto& follow : finalFollowNodeMap.at(matchHead)) {
            if (follow == matchItem) {
                return true;
            }
        }
    }
    return false;
}

/**
 * 给定一个非终结符和一个终结符
 * 根据Productions找到合适的Formula并返回
 * @param productions
 * @param target
 * @param matchItem
 * @return
 */
set<Formula> findSuitableFormulas(const Productions& productions,
        const FinalFirstNodeMap& finalFirstNodeMap, const FinalFollowNodeMap& finalFollowNodeMap,
        const Sign& target, const Sign& matchItem) {
    set<Formula> formulaSet;
    for (auto formula : productions.at(target)) {
        if (formula[0] == matchItem || (isNonTerminator(formula[0]) &&
                checkSuitable(finalFirstNodeMap, finalFollowNodeMap, formula[0], matchItem))) {
            formulaSet.insert(formula);
        }
    }
    return formulaSet;
}

/**
 * 获取预测分析表
 * @param productions
 * @param finalFirstNodeMap
 * @param finalFollowNodeMap
 * @return
 */
PredictionMap getPredictionMap(const Productions& productions,
        const FinalFirstNodeMap& finalFirstNodeMap, const FinalFollowNodeMap& finalFollowNodeMap) {

    PredictionMap predictionMap;
    for (const auto& firstNode : finalFirstNodeMap) {
        Sign target = firstNode.first;
        for (const auto& firstItem : firstNode.second) {
            if (firstItem != EPSILON) {
                set<Formula> formulaSet = findSuitableFormulas(productions,
                        finalFirstNodeMap, finalFollowNodeMap, target, firstItem);
                predictionMap[target][firstItem].insert(formulaSet.begin(), formulaSet.end());
            } else {
                for (const auto& followItem : finalFollowNodeMap.at(target)) {
                    predictionMap[target][followItem].insert({EPSILON});
                }
            }
        }
    }
    return predictionMap;
}

/**
 * 根据预测分析表判断文法是否为LL1文法
 * @param predictionMap
 * @return
 */
bool checkLL1(const PredictionMap& predictionMap) {
    for (const auto& p : predictionMap) {
        for (const auto& q : p.second) {
            if (q.second.size() > 1) {
                return false;
            }
        }
    }
    return true;
}

/**
 * 判定待匹配字符串是否合法
 * @param stringToMatch
 * @return
 */
bool isLegalStringToMatch(const string& stringToMatch) {
    for (char i : stringToMatch) {
        if (i == END_MARK_CHAR) {
            return false;
        }
    }
    return true;
}


/**
 * 下推自动机（只适用于判定文法中每个字符都是一个终结符，终结符“id”不适用）
 * @param predictionMap
 * @param start
 * @param stringToMatch
 * @return
 */
bool analysis(const PredictionMap& predictionMap, const Sign& start, const string& stringToMatch) {
    if (!checkLL1(predictionMap)) {
        throw "NOT LL1";
    }
    stack<Sign> opStack;
    opStack.push(END_MARK);
    opStack.push(start);
    string str = stringToMatch;
    if (!isLegalStringToMatch(str)) {
        throw "String to match is illegal";
    }
    str.append(END_MARK);
    Formula formula = stringToFormula(str);
    int index = 0;
    bool match = true;
    while (index < formula.size()) {
        Sign target = opStack.top();
        if (isNonTerminator(target)) {      //栈顶元素为非终结符
            if (predictionMap.count(target) > 0
                    && predictionMap.at(target).count(formula[index]) > 0
                    && !predictionMap.at(target).at(formula[index]).empty()) {
                //更新栈
                Formula item = *predictionMap.at(target).at(formula[index]).begin();
                opStack.pop();
                if (item[0]== EPSILON) {
                    continue;
                }
                for (int i = item.size() - 1; i >= 0; i--) {
                    opStack.push(item[i]);
                }
            } else {
                match = false;
                break;
            }
        } else if (target == END_MARK) {    //终结标识符“#”
            if (index < formula.size() - 1) {
                match = false;
                break;
            } else {
                break;
            }
        } else {    //终结符
            if (target == formula[index]) {
                opStack.pop();
                index++;
            } else {
                match = false;
                break;
            }
        }
    }
    return match;
}

