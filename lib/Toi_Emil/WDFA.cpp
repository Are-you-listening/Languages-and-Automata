//
// Created by emil on 27/04/23.
//

#include "WDFA.h"
#include <iostream>
#include "json.hpp"
#include "weightedNode.h"
using json = nlohmann::json;

double WDFA::weightedaccepts(string input) {
    weightedNode* currentState = startState;
    double result = 0.0;
    for (char symbol : input){
        if (alfabet.find(symbol) == alfabet.end()){
            cerr << "inputstring contains an unknown symbol" << endl;
            return 0.0;
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

WDFA::WDFA(const string &filename) : weightedautomaat(filename) {}

