//
// Created by emil on 24/03/23.
//

#include <sstream>
#include <queue>
#include "json.hpp"
#include <utility>
#include "automaat.h"
#include "NFA.h"
#include "weightedNode.h"

string NFA::ssc_helper(const string& currentState, char input) {
    vector<string> temp;
    for (const auto& state : splitString(currentState)){
        for (auto connection : getState(state).first->getConnections()){
            if (connection.second.find(input) != connection.second.end()){
                if (std::find(temp.begin(), temp.end(), connection.first->getName()) == temp.end()){
                    temp.push_back(connection.first->getName());
                }
            }
        }
    }
    string result = "{";
    sort(temp.begin(), temp.end());
    for(const auto& part : temp){
        result += part;
        if (part != temp.back()){
            result += ",";
        }
    }
    result += "}";
    return result;
}

/**
 * @brief creates a new automata that is the DFA equivalent of *this using the subset construction
 * @return DFA
 */

DFA NFA::toDFA() {

    DFA result;
    result.alfabet = alfabet;     // the constructed DFA always uses the same alphabet
    string startstate = "{" + startState->getName() + "}";
    queue<string> toProcess;
    toProcess.push(startstate);

    bool starting;
    bool accepting;
    string processing;
    vector<string> split;

    while (not toProcess.empty()) {
        starting = false;
        accepting = false;
        processing = toProcess.front();
        split = splitString(processing);

        if (splitString(processing)[0] == startState->getName() && splitString(processing).size() == 1){
            starting = true;
        }
        for (const auto& state : split){
            if (find(endStates.begin(), endStates.end(), getState(state).first) != endStates.end()){
                accepting = true;
            }
        }
        if (!result.getState(processing).second) {      // tests if we already created the current state
            result.addState(processing, starting, accepting);
        }
        for (char symbol: alfabet) {
            string temp = ssc_helper(processing, symbol);
            if (!result.getState(temp).second) {
                toProcess.push(temp);
            }
        }

        toProcess.pop();
    }

    for(auto state : result.states) {
        vector<string> temp = splitString(state->getName());
        vector<string> currentconnections;
        for (auto symbol: alfabet) {
            currentconnections = {};
            for (const auto& NFA_state: temp) {
                auto tempconnections = getState(NFA_state).first->getConnections();
                for (auto connection : tempconnections){
                    if (connection.second.find(symbol) != connection.second.end()){
                        if(std::find(currentconnections.begin(), currentconnections.end(), connection.first->getName()) == currentconnections.end()) {
                            currentconnections.push_back(connection.first->getName());
                        }
                    }
                }
            }
            string otherstate = "{";
            sort(currentconnections.begin(), currentconnections.end());
            for(const auto& part : currentconnections){
                otherstate += part;
                if (part != currentconnections.back()){
                    otherstate += ",";
                }
            }
            otherstate += "}";
            state->addconnection(result.getState(otherstate).first, symbol);
        }
    }
    return result;
}

NFA::NFA(const string &fileName) : automaat(fileName) {
    type = "NFA";
}

NFA::NFA() {
    type = "NFA";
}

WNFA NFA::toWNFA() {
    WNFA result = WNFA();
    bool startingstate;
    bool acceptingstate;
    for (Node* state : states){
        startingstate = (state == startState);
        if (std::find(endStates.begin(), endStates.end(),state) != endStates.end()){
            acceptingstate = true;
        }

        result.addState(state->getName(), startingstate, acceptingstate);

    }
    for (weightedNode* state : result.states){

        for (const auto& connection : getState(state->getName()).first->getConnections()) {
            for (char symbol: get<1>(connection)) {
                state->addconnection(result.getWeightedState(get<0>(connection)->getName()).first, symbol, 1);
            }
        }
    }
    return result;
}
