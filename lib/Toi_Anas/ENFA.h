//
// Created by anass on 6-3-2023.
//

#ifndef DFA_ENFA_H
#define DFA_ENFA_H
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include "DFA.h"
using namespace std;

using json = nlohmann::json;

class state;
class DFA;

class ENFA {
protected:
    map<pair<char,string>, vector<string>> possibleStates(state* nextState, const string &name)const&;
    vector<state *> ECLOSE(state* Etransition)const&;
    vector<state *> ECLOSE2(state* Etransition, string input)const&;
    void Etransitions(vector<state*> &states)const&;
public:
    state* startingState;
    vector<state*> states;
    set<string> alphabet;
    vector<state*> endstates;
    string eps;
    ENFA(const string& c);
    ENFA(const ENFA& enfa,state* state);
    ENFA();
    bool accepts(string c)const&;
    void print()const&;
    DFA toDFA()&;
    DFA toDFA2()&;
    void printStats()const&;
    void concatenate(const ENFA& enfa)&;
};


#endif //DFA_ENFA_H
