//
// Created by emill on 10/05/2023.
//

#ifndef WNFA_WNFA_H
#define WNFA_WNFA_H


#include <queue>
#include <string>
#include <set>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "json.hpp"
#include "weightedNode.h"
#include "../lib/Utils.h"

using namespace std;
using json = nlohmann::json;

class WDFA;

class WNFA {
private:
    //Get reachable states (eq class') + biggest weight
    pair< map<string,weightedNode*> , double> WSSC_helper(const map<string,weightedNode*> &currentstates, const char& input) const;
    map<string,weightedNode*> splitString(const string& str) const;

protected:
    map<string, weightedNode*> states;
    weightedNode* startState = nullptr;
    map<string, weightedNode*> endStates = {};
    string type;
    vector<string> alfabet;

public:
    const map<string, weightedNode *> &getStates() const;

    void setStates(const map<string, weightedNode *> &states);

    weightedNode *getStartState() const;

    void setStartState(weightedNode *startState);

    const map<string, weightedNode *> &getEndStates() const;

    void setEndStates(const map<string, weightedNode *> &endStates);

    const string &getType() const;

    void setType(const string &type);

    const vector<string> &getAlfabet() const;

    void setAlfabet(const vector<string> &alfabet);

    bool isStartState(string name) const;
    bool isEndState(string name) const;
    void addState(const string& name, bool start, bool endState);
    [[nodiscard]] pair<weightedNode*, bool> getState(string name) const;
    
    WNFA();
    WNFA(const string &filename);

    virtual double weightedaccepts(string input) const;

    virtual void print() const;
    
    WDFA toWDFA();
};


#endif //WNFA_WNFA_H
