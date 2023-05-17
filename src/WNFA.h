//
// Created by emill on 10/05/2023.
//

#ifndef WNFA_WNFA_H
#define WNFA_WNFA_H

#include "Toi_Emil/weightedNode.h"
#include <list>
#include <string>
#include <set>
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;
using json = nlohmann::json;

class WNFA {
public:
    map<string, weightedNode*> states;
    weightedNode* startState = nullptr;
    map<string, weightedNode*> endStates = {};
    string type;
    vector<string> alfabet;

    pair<weightedNode *, bool> getweightedState(string name);

    void addState(string name, bool start, bool endState);

    pair<weightedNode*, bool> getWeightedState(string name) const;

    bool isStartState(string name);

    bool isEndState(string name);

    pair<Node*, bool> getState(string name);
    
    WNFA();
    WNFA(const string &filename);
    
    /**
     * \brief 
     * REQUIRE: Alphabet is compatible with the input string
     * @param input 
     * @return 
     */
    double weightedaccepts(string input);
    void print();
};


#endif //WNFA_WNFA_H
