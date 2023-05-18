//
// Created by emill on 10/05/2023.
//

#include "WDFA.h"

WDFA::WDFA(const string &filename) {
    ifstream input(filename);
    json j;
    input >> j;
    type = j["type"];
    set<string> temp = j["alphabet"];
    alfabet.clear();
    for (auto item : temp){
        string symbol = item;
        alfabet.push_back(symbol);
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

WDFA::WDFA(): type("WDFA") {}

pair<weightedNode *, bool> WDFA::getweightedState(string name) {
    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

void WDFA::addState(string name, bool start, bool endState) {
    weightedNode* n = new weightedNode(name);
    states.push_back(n);
    if (start){
        start = n;
    }
    if (endState){
        endStates.push_back(n);
    }
}

pair<weightedNode *, bool> WDFA::getWeightedState(string name) {
    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

double WDFA::weightedaccepts(string input) {
    weightedNode* currentState = startState;
    double result = 0.0;
    for (const char &symbol : input){ //Loop on every character of the string
        auto nextStates = currentState->accepts(symbol); //Get the reachable states with this symbol

        if(nextStates.empty()){ //No transitions
            result-=1.0;
        }else{ //The state accepted this symbol
            result+=nextStates[0].first; //If this is a correct DFA, there is only 1 reachable state by this input
            currentState = nextStates[0].second;
        }
    }
    return result/input.size();
}

void WDFA::print() {
    json Jout;
    Jout["type"] = type;
    for (auto symbol : alfabet){
        Jout["alphabet"].push_back(symbol);
    }
    for (auto state: states) {  // add all the states
        json temp;
        temp["name"] = state->getName();
        temp["starting"] = isStartState(state->getName());
        temp["accepting"] = isEndState(state->getName());
        Jout["states"].push_back(temp);
    }

    for(auto state: states) {  // add all the transitions
        for(const auto &transition: state->getweightedconnections()) {
            json temp;
            temp["from"] = state->getName();
            temp["to"] = transition.second[0].second->getName();
            temp["input"] = string(1, transition.first);
            temp["weight"] = transition.second[0].first;
            Jout["transitions"].push_back(temp);
        }
    }
    cout << setw(4) << Jout << endl;
}

pair<weightedNode *, bool> WDFA::getState(string name) {

    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

bool WDFA::isStartState(string name) {
    if (getState(name).first == startState){
        return true;
    } else {
        return false;
    }
}

bool WDFA::isEndState(string name) {
    for (auto state : endStates){
        if (state == getState(name).first){
            return true;
        }
    }
    return false;
}