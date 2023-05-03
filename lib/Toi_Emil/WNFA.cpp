//
// Created by emil on 27/04/23.
//

#include "WNFA.h"
#include <iostream>
#include "json.hpp"
#include <fstream>
#include "weightedNode.h"
using json = nlohmann::json;


double WNFA::weightedaccepts(string input) {
    vector<weightedNode*> currentStates = {startState};
    double result = 0.0;
    for (char symbol : input) {
        if (alfabet.find(symbol) == alfabet.end()) {
            cerr << "inputstring contains an unknown symbol" << endl;
            return 0.0;
        }
        vector<weightedNode*> tempstates;
        bool connectionfound = false;
        for (weightedNode* state: currentStates) {
            for (auto connection: state->weightedNode::getconnections()) {
                if (get<1>(connection).find(symbol) != get<1>(connection).end()) {
                    connectionfound = true;
                    result += get<2>(connection);
                    tempstates.push_back(get<0>(connection));
                }
            }
        }
        if (not connectionfound){
            return 0.0;
        }
        currentStates = tempstates;
    }
    for (Node* state : currentStates){
        for (Node* endstate : endStates){
            if (state == endstate){
                return result;
            }
        }
    }
    return 0.0;
}

WNFA::WNFA(const string &filename) : weightedautomaat(filename) {}

