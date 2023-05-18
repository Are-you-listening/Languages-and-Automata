//
// Created by emil on 27/04/23.
//
#include "weightedNode.h"

void weightedNode::addconnection(weightedNode *otherState, char symbol, double weight) {
    connections[symbol].push_back( make_pair(weight,otherState) );
}

const map<char,vector<pair< double,weightedNode* >> > & weightedNode::getweightedconnections() const {
    return connections;
}

weightedNode::weightedNode(const string &name): name(name){}

vector<pair<double, weightedNode * >> weightedNode::accepts(const char &input) {
    if(connections.find(input)!=connections.end()){
        return connections[input];
    }else{
        return {};
    }
}

const string &weightedNode::getName() const {
    return name;
}
