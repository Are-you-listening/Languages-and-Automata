//
// Created by emill on 10/05/2023.
//

#ifndef WNFA_WDFA_H
#define WNFA_WDFA_H

#include <list>
#include <string>
#include <set>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "json.hpp"
#include "weightedNode.h"

using namespace std;
using json = nlohmann::json;

class WDFA {
public:
    map<string,weightedNode*> states;
    weightedNode* startState = nullptr;
    list<weightedNode*> endStates = {};
    string type;
    vector<string> alfabet;

    void addState(string name, bool start, bool endState);

    bool isStartState(string name);

    bool isEndState(string name);

    pair<weightedNode *, bool> getState(string name);
    
    WDFA();
    WDFA(const string &filename);
    double weightedaccepts(string input);
    void print();
};


#endif //WNFA_WDFA_H
