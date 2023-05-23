//
// Created by emil on 27/04/23.
//

#include "weightedNode.h"

void weightedNode::addconnection(weightedNode *otherState, char symbol, double weight) {
    connections[symbol].emplace_back(weight,otherState );
}

const map<char,vector<pair< double,weightedNode* >> > & weightedNode::getweightedconnections() const {
    return connections;
}

weightedNode::weightedNode(string name): name(std::move(name)){}

vector<pair<double, weightedNode * >> weightedNode::accepts(const char &input) {
    return connections[input];
}

const string &weightedNode::getName() const {
    return name;
}
