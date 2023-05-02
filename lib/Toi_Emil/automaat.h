//
// Created by emill on 27/02/2023.
//

#ifndef DFA_AUTOMAAT_H
#define DFA_AUTOMAAT_H
#include "Node.h"
#include <set>
#include <vector>
#include "json.hpp"
using namespace std;

class automaat {
public:
    list<Node*> states;
    set<char> alfabet;
    Node* startState = nullptr;
    list<Node*> endStates = {};
    string type;

    void load(const nlohmann::json& j);

    virtual void addState(string name, bool startState, bool endState);

    virtual bool isStartState(string name) const;
    virtual bool isEndState(string name) const;

    virtual vector<string> splitString(const string &str);

    virtual pair<Node*,bool> getState(string name) const;

    nlohmann::json getJson() const;

public:
    automaat(string fileName);

    explicit automaat();

    virtual bool accepts(string input);
    virtual void print();

};


#endif //DFA_AUTOMAAT_H
