//
// Created by emil on 27/04/23.
//

#ifndef DFA_WEIGHTEDNODE_H
#define DFA_WEIGHTEDNODE_H

#include "Node.h"

class weightedNode: public Node{
    list<tuple<weightedNode*, set<char>, double>> connections;
public:
    void addconnection(weightedNode *otherState, char symbol, double weight);

    const list<tuple<weightedNode *, set<char>, double>> & getweightedconnections() const;

    explicit weightedNode(const string &name);
};

#endif //DFA_WEIGHTEDNODE_H
