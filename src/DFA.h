//
// Created by anass on 1-5-2023.
//

#ifndef TA__TOG_DFA_H
#define TA__TOG_DFA_H
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
using namespace std;

using json = nlohmann::json;

class state{
public:
    map<string,state*> states;
    string name;
    bool starting;
    bool accepting;
    void addTransitionFunction(string c ,state* q);
};

class DFA {
    state* startingState;
    vector<state*> states;
    set<string> alphabet;
    vector<state*> endstates;
public:
    state *getStartingState() const;

    void setStartingState(state *startingState);

    const vector<state *> &getStates() const;

    void setStates(const vector<state *> &states);

    const set<string> &getAlphabet() const;

    void setAlphabet(const set<string> &alphabet);

    const vector<state *> &getEndstates() const;

    void setEndstates(const vector<state *> &endstates);

    DFA(const string& c);

    DFA();

    DFA(DFA& dfa1, DFA& dfa2, bool c);

    bool accepts(string c)const&;

    void print()const&;



};


#endif //TA__TOG_DFA_H
