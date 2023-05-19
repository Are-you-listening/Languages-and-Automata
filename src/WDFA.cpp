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
        weightedNode *state1 = getState(transition["from"]).first;
        weightedNode *state2 = getState(transition["to"]).first;
        string inputsymbol = transition["input"];
        double weight = transition["weight"];
        for (auto symbol: inputsymbol) {
            state1->addconnection(state2, symbol, weight);
        }
    }
}

WDFA::WDFA(): type("WDFA") {}

void WDFA::addState(string name, bool start, bool endState) {
    weightedNode* n = new weightedNode(name);
    states[n->getName()]=n;
    if (start){
        startState = n;
    }
    if (endState){
        endStates.push_back(n);
    }
}

double WDFA::weightedaccepts(string input) {
    weightedNode* currentState = startState;
    double result = 0.0;
    for (const char &symbol : input){ //Loop on every character of the string
        auto nextStates = currentState->accepts(symbol); //Get the reachable states with this symbol

        if(nextStates.empty()){ //No transitions
            result--;
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
        temp["name"] = state.first;
        temp["starting"] = isStartState(state.first);
        temp["accepting"] = isEndState(state.first);
        Jout["states"].push_back(temp);
    }

    for(auto state: states) {  // add all the transitions
        for(const auto &transition: state.second->getweightedconnections()) {
            json temp;
            temp["from"] = state.second->getName();
            temp["to"] = transition.second[0].second->getName();
            temp["input"] = string(1, transition.first);
            temp["weight"] = transition.second[0].first;
            Jout["transitions"].push_back(temp);
        }
    }
    cout << setw(4) << Jout << endl;
}

pair<weightedNode *, bool> WDFA::getState(string name) {
    auto result = states.find(name);
    if (result == states.end()){
        return make_pair(nullptr, false);
    }
    return make_pair(result->second, true);
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