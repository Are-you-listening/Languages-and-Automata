//
// Created by emill on 27/02/2023.
//

#ifndef DFA_NODE_H
#define DFA_NODE_H
#include <string>
#include <list>
#include <set>
#include <map>
using namespace std;

class Node {
public:
    string name;
    map<char, Node*> connections;
    map<Node*, string> regexconnections;
    string selfconnection;

    explicit Node(const string &name);
    void addconnection(Node* otherState, char symbol);

    const string &getName() const;

    virtual const map<char, Node*> &getConnections() const;
};


#endif //DFA_NODE_H
