//
// Created by emill on 27/02/2023.
//

#ifndef DFA_NODE_H
#define DFA_NODE_H
#include <string>
#include <list>
#include <set>
using namespace std;

class Node {
public:
    string name;
    list<pair<Node*, set<char>>> connections;
    list<pair<Node*, string>> regexconnections;
    string selfconnection;

    explicit Node(const string &name);
    void addconnection(Node* otherState, char symbol);

    const string &getName() const;

    virtual const list<pair<Node *, set<char>>> &getConnections() const;
};


#endif //DFA_NODE_H
