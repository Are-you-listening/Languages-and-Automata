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
    vector<pair<double, weightedNode*>> currentstates = {make_pair(0.0, this->startState)};

    for (char symbol : input){
        vector<pair<double, weightedNode*>> tempstates;
        for (pair<double, weightedNode*> state : currentstates){
            for (auto connection : state.second->getconnections()){
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

WNFA::WNFA(const string &filename) : weightedautomaat(filename) {}

