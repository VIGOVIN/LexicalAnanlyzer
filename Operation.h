//
// Created by Semir on 2020/5/2.
//

#ifndef LEXICALANALYZER_OPERATION_H
#define LEXICALANALYZER_OPERATION_H

#include "DataStructure.h"

const string ERROR_MSG = "ERROR";

const int NON_TERMINATOR = 0;
const int ARROW = 1;
const int DIVIDER = 2;
const int TERMINATOR = 3;
const Sign EPSILON = "~";
const Sign END_MARK = "#";
const char END_MARK_CHAR = '#';

//void stringIntoProductions(Productions &productions, const string& str);

void showHelp();

Sign getNextSign(const string& str, int& index);

Productions makeProductions(vector<string>& strings);

bool hasHiddenRecursionDfs(Productions& productions, const Sign& target, map<Sign, bool>& vis, const Sign& left);

bool hasHiddenRecursion(Productions& productions);

bool hasExplicitRecursion(Productions& productions);

void exposeHiddenRecursion(Productions& productions);

void eliminateExplicitRecursion(Productions& productions, const Sign& target);

void eliminateRecursion(Productions& productions);

void productionsToStringVector(const Productions& productions, vector<string>& strings);

void extractFactor(Productions& productions);

FinalFirstNodeMap getFirst(const Productions& productions);

FinalFollowNodeMap getFollow(const Sign& start, const Productions& productions,
        const FinalFirstNodeMap& finalFirstNodeMap);

PredictionMap getPredictionMap(const Productions& productions,
        const FinalFirstNodeMap& finalFirstNodeMap, const FinalFollowNodeMap& finalFollowNodeMap);

bool checkLL1(const PredictionMap& predictionMap);

bool analysis(const PredictionMap& predictionMap, const Sign& start, const string& stringToMatch);

#endif //LEXICALANALYZER_OPERATION_H
