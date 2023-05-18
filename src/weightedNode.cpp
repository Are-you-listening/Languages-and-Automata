//
// Created by emil on 27/04/23.
//
#include "weightedNode.h"
#include <tuple>

void weightedNode::addconnection(weightedNode *otherState, char symbol, double &weight) {
    connections[symbol].push_back( make_pair(weight,otherState) );
}

const map<char,vector<pair< double,weightedNode* >> > & weightedNode::getweightedconnections() const {
    return connections;
}

weightedNode::weightedNode(const string &name): name(name){}

vector<pair<double, weightedNode * >> weightedNode::accepts(const char &input) {
    return connections[input];
}
