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

WDFA::WDFA() {}

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
    for (char symbol : input){
        string symbol_str = string(1, symbol);
        if (std::find(alfabet.begin(), alfabet.end(),symbol_str) == alfabet.end()){
            cerr << "Symbol " << symbol << " not in alphabet" << endl;
            return -1.0;
        }
        for (auto connection : currentState->weightedNode::getweightedconnections()){
            if (get<1>(connection).find(symbol) != get<1>(connection).end()){
                result += get<2>(connection);
                currentState = get<0>(connection);
                break;
            }
        }
    }
    for (Node* state : endStates){
        if (state == currentState){
            return result;
        }
    }
    return 0.0;
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

pair<Node*, bool> WDFA::getState(string name) {

    for (Node* state : states){
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