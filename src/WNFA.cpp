//
// Created by emill on 10/05/2023.
//

#include "WNFA.h"

WNFA::WNFA(const string &filename) {
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

WNFA::WNFA() {}

pair<weightedNode *, bool> WNFA::getweightedState(string name) {
    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

void WNFA::addState(string name, bool start, bool endState) {
    weightedNode* n = new weightedNode(name);
    states.push_back(n);
    if (start){
        startState = n;
    }
    if (endState){
        endStates.push_back(n);
    }
}

pair<weightedNode *, bool> WNFA::getWeightedState(string name) const{
    for (weightedNode* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

double WNFA::weightedaccepts(string input) {
    vector<pair<double, weightedNode*>> currentstates = {make_pair(0.0, this->startState)};

    for (char symbol : input){
        string temp = string(1, symbol);
        if (std::find(alfabet.begin(), alfabet.end(), temp) == alfabet.end()){
            cerr << "Symbol " << symbol << " not in alphabet" << endl;
            return -1.0;
        }
        vector<pair<double, weightedNode*>> tempstates;
        for (pair<double, weightedNode*> state : currentstates){
            for (auto connection : state.second->getweightedconnections()){
                if (get<1>(connection).find(symbol) != get<1>(connection).end()){
                    tempstates.emplace_back(state.first + get<2>(connection), get<0>(connection));
                }
            }
        }
        currentstates = tempstates;
    }

    double largestpath = 0.0;
    for (pair<double, weightedNode*> state : currentstates){
        if (state.first > largestpath){
            largestpath = state.first;
        }
    }
    return largestpath;
}

void WNFA::print() {
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

pair<Node*, bool> WNFA::getState(string name) {

    for (Node* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

bool WNFA::isStartState(string name) {
    if (getState(name).first == startState){
        return true;
    } else {
        return false;
    }
}

bool WNFA::isEndState(string name) {
    for (auto state : endStates){
        if (state == getState(name).first){
            return true;
        }
    }
    return false;
}