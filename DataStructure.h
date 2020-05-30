//
// Created by Semir on 2020/5/2.
//

#ifndef _LEXICALANALYZER_DATASTRUCTURE_H
#define _LEXICALANALYZER_DATASTRUCTURE_H

#include<bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <deque>
#include <queue>
#include <ctime>
#include <set>
#include <stack>
#include <vector>
#include <climits>
#include "unistd.h"

using namespace std;

//符号：Sign，包括终结符和非终结符
typedef string Sign;

/**
 * 式子：Formula，由多个符号构成
 * 如产生式 A -> Aa | b 中
 * Aa 和 b 均作为一个式子
 */
typedef vector<Sign> Formula;

Formula stringToFormula(const string& str);

/**
 * 产生式集合：Productions
 * 产生式的左部作为哈希表的键，产生式的右部作为哈希表的值（本质上是一个式子的集合）
 */
typedef map<Sign, set<Formula>> Productions;

void printFormula(const Formula& formula);

void printFormulaEdl(const Formula& formula);

void printProductions(Productions &productions);

void printStringVector(const vector<string>& vec);

/**
 * 用于辅助计算FIRST集的数据结构
 * firstSet存放该非终结符已经导出的FIRST集
 * formulaSet存放待处理的产生式
 * finished表示该非终结符的FIRST是否已经处理完毕
 */
struct FirstNode {
    set<Sign> firstSet;
    set<Formula> formulaSet;
    bool finished;
    FirstNode() : finished(false) {}
};

/**
 * 辅助构建FIRST集
 * 建立Sign和FirstNode的关联
 */
typedef map<Sign, FirstNode> FirstNodeMap;

FirstNodeMap createFirstNodeMap(const Productions& productions);

//FIRST集
typedef map<Sign, set<Sign>> FinalFirstNodeMap;

FinalFirstNodeMap createFinalFirstNodeMap(const FirstNodeMap& firstNodeMap);

void printFirstSet(const FinalFirstNodeMap& finalFirstNodeMap);

//辅助构建Follow集
//<first, second> 代表需要将first的FOLLOW集拷贝到second的FOLLOW集
typedef set<pair<Sign, Sign>> FollowLinks;

//FOLLOW集
typedef map<Sign, set<Sign>> FinalFollowNodeMap;

void printFollowSet(const FinalFollowNodeMap& finalFollowNodeMap);

/**
 * 预测分析表，二维表结构
 * 由于表中的一个元素可能包含多个产生式（不符合LL1）
 * 因此表中的每个元素用set保存
 */
typedef map<Sign, map<Sign, set<Formula>>> PredictionMap;

void printPredictionMap(const PredictionMap& predictionMap);


#endif //_LEXICALANALYZER_DATASTRUCTURE_H
