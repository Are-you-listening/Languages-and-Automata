//
// Created by emill on 3/05/2023.
//

#ifndef DFA_WEIGHTEDAUTOMAAT_H
#define DFA_WEIGHTEDAUTOMAAT_H

#include "automaat.h"

class weightedNode;

class weightedautomaat: public automaat{
protected:

    list<weightedNode*> states;
    weightedNode* startState = nullptr;
    list<weightedNode*> endStates = {};
    
    virtual pair<weightedNode *, bool> getweightedState(string name);

    void addState(string name, bool startState, bool endState) override;
    
public:
    weightedautomaat(const string &filename);
    virtual double weightedaccepts(string input)=0;
};


#endif //DFA_WEIGHTEDAUTOMAAT_H
