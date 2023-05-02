//
// Created by emill on 27/02/2023.
//

#include "automaat.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <iomanip>
#include "json.hpp"
#include "NFA.h"
#include <queue>
#include <sstream>
using namespace std;
using json = nlohmann::json;

automaat::automaat(){}

automaat::automaat(string fileName) {
    ifstream input(fileName);
    json j;
    input >> j;
    load(j);
}


void automaat::addState(string name, bool start, bool endState) {
    Node* n = new Node(name);
    states.push_back(n);
    if (start){
        startState = n;
    }
    if (endState){
        endStates.push_back(n);
    }
}

/**
 * @brief takes a name of a state and returns a pair with the pointer to that state and a boolean based on success
 * @param name the name of the state we are looking for
 * @return (Node*, bool)
 */

pair<Node*, bool> automaat::getState(string name) const{
    
    for (Node* state : states){
        if (state->getName() == name){
            return make_pair(state, true);
        }
    }
    return make_pair(nullptr, false);
}

/**
 * @brief tests to see if a given string is accepted by this automato
 * @param input the string to be tested
 * @return bool
 */

bool automaat::accepts(string input) {
    Node* currentState = startState;
    for (char symbol : input){
        if (alfabet.find(symbol) == alfabet.end()){
            cerr << "inputstring contains an unknown symbol" << endl;
            return false;
        }
        for (auto connection : currentState->getConnections()){
            if (connection.second.find(symbol) != connection.second.end()){
                currentState = connection.first;
                break;
            }
        }
    }
    for (Node* state : endStates){
        if (state == currentState){
            return true;
        }
    }
    return false;
}

bool automaat::isStartState(string name) const{
    if (getState(name).first == startState){
        return true;
    } else {
        return false;
    }
}

bool automaat::isEndState(string name) const{
    for (auto state : endStates){
        if (state == getState(name).first){
            return true;
        }
    }
    return false;
}

/**
 * @brief writes the automata to output in json format
 */

void automaat::print() {
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
    // these 2 for loops can probably be merged, but I will keep them separate for readability
    for (auto state: states) {  // add all the transitions
        for (const auto& transition: state->getConnections()) {
            for (auto symbol: transition.second) {
                json temp;
                temp["from"] = state->getName();
                temp["to"] = transition.first->getName();
                temp["input"] = string(1, symbol);
                Jout["transitions"].push_back(temp);
            }
        }
    }
        cout << setw(4) << Jout << endl;
}

vector<string> automaat::splitString(const string &str) {
    vector<string> tokens;
    string token;
    string filteredString;
    for (char character : str){
        if (character != '{' and character != '}'){
            filteredString += character;
        }
    }

    istringstream tokenStream(filteredString);
    while (getline(tokenStream, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

void automaat::load(const nlohmann::json& j){
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
        Node *state1 = getState(transition["from"]).first;
        Node *state2 = getState(transition["to"]).first;
        string inputsymbol = transition["input"];
        for (auto symbol: inputsymbol) {
            state1->addconnection(state2, symbol);
        }
    }
}

nlohmann::json automaat::getJson() const{
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
    // these 2 for loops can probably be merged, but I will keep them separate for readability
    for (auto state: states) {  // add all the transitions
        for (const auto& transition: state->getConnections()) {
            for (auto symbol: transition.second) {
                json temp;
                temp["from"] = state->getName();
                temp["to"] = transition.first->getName();
                temp["input"] = string(1, symbol);
                Jout["transitions"].push_back(temp);
            }
        }
    }
    return Jout;
}