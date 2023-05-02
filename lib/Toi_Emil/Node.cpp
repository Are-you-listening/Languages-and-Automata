//
// Created by emill on 27/02/2023.
//

#include "Node.h"
#include <iostream>

Node::Node(const string &name) : name(name) {}

void Node::addconnection(Node *otherState, char symbol) {
    bool found = false;
    for (auto &connection : connections) {
        if (connection.first == otherState) {
            connection.second.emplace(symbol);
            found = true;
        }
    }
    if (not found){
                connections.push_back(make_pair(otherState, set<char> {symbol}));
        }
}

const string &Node::getName() const {
    return name;
}

const list<pair<Node *, set<char>>> &Node::getConnections() const {
    return connections;
}

