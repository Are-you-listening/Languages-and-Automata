//
// Created by emill on 3/05/2023.
//

#include "weightedautomaat.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
#include "weightedNode.h"
using json = nlohmann::json;

weightedautomaat::weightedautomaat(const string &filename) {
    ifstream input(filename);
    json j;
    input >> j;
    type = j["type"];
    set<string> temp = j["alphabet"];
    alfabet.clear();
    for (auto item : temp){
        char symbol = item[0];
        alfabet.emplace(symbol);
    }
    set<json> newstates = j["states"];
    for (auto state : newstates){
        string tempname = state["name"];
        bool tempstart = state["starting"];
        bool tempend = state["accepting"];
        addState(tempname, tempstart, tempend);
    }

    set<json> newtransitions = j["transitions"];
    for (auto transition : newtransitions) {
        weightedNode *state1 = getweightedState(transition["from"]).first;
        weightedNode *state2 = getweightedState(transition["to"]).first;
        string inputsymbol = transition["input"];
        double weight = transition["weight"];
        for (auto symbol: inputsymbol) {
            state1->addconnection(state2, symbol, weight);
        }
    }
}

pair<weightedNode *, bool> weightedautomaat::getweightedState(string name) {
    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

void weightedautomaat::addState(string name, bool start, bool endState) {
    weightedNode* n = new weightedNode(name);
    states.push_back(n);
    if (start){
        startState = n;
    }
    if (endState){
        endStates.push_back(n);
    }
}

weightedautomaat::weightedautomaat() {
}

pair<weightedNode *, bool> weightedautomaat::getWeightedState(string name) {
    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

void weightedautomaat::print() {
    json Jout;
    Jout["type"] = type;
    for (auto symbol : alfabet){
        Jout["alphabet"].push_back(string(1, symbol));
    }
    for (auto state: states) {  // add all the states
        json temp;
        temp["name"] = state->getName();
        temp["starting"] = isStartState(state->getName());
        temp["accepting"] = isEndState(state->getName());
        Jout["states"].push_back(temp);
    }

    for (auto state: states) {  // add all the transitions
        for (const auto& transition: state->getweightedconnections()) {
            for (auto symbol: get<1>(transition)) {
                json temp;
                temp["from"] = state->getName();
                temp["to"] = get<0>(transition)->getName();
                temp["input"] = string(1, symbol);
                temp["weight"] = get<2>(transition);
                Jout["transitions"].push_back(temp);
            }
        }
    }
    cout << setw(4) << Jout << endl;
}


