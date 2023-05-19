//
// Created by emill on 27/02/2023.
//

#include "Node.h"
#include <iostream>

Node::Node(const string &name) : name(name) {}

void Node::addconnection(Node *otherState, char symbol) {
    connections[symbol] = otherState;
}

const string &Node::getName() const {
    return name;
}

const map<char, Node*> &Node::getConnections() const {
    return connections;
}
