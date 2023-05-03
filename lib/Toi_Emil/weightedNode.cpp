//
// Created by emil on 27/04/23.
//
#include "weightedNode.h"
#include <tuple>

void weightedNode::addconnection(weightedNode *otherState, char symbol, double weight) {
    bool found = false;
    for (auto &connection : connections) {
        if (get<0>(connection) == otherState && get<2>(connection) == weight) {
            get<1>(connection).emplace(symbol);
            found = true;
        }
    }
    if (not found){
        connections.emplace_back(otherState, set<char> {symbol}, weight);
    }
}

const list<tuple<weightedNode *, set<char>, double>> & weightedNode::getconnections() const {
    return connections;
}

weightedNode::weightedNode(const string &name): Node(name){}

