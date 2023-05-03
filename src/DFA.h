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
#include "Toi_Tibo/DFAT.h"
#include "Toi_Emil/DFAE.h"
#include "Toi_Emil/REE.h"

class ENFA;
using namespace std;

using json = nlohmann::json;

class state{
public:
    map<string,state*> states;
    map<string,set<state*>> statesENFA;
    string name;
    bool starting;
    bool accepting;
    void addTransitionFunction(string c ,state* q);
    void addTransitionFunctionENFA(string c ,state* q);
    state* getComplement();
};

class DFA {
    state* startingState;
    vector<state*> states;
    set<string> alphabet;
    vector<state*> endstates;
    json json_data;

    void load(const json& j);

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

    DFA minimize();

    bool operator==(const DFA& d);

    json getJson() const;

    string ToRe();

    DFA complement();

    ENFA reverse();

};


#endif //TA__TOG_DFA_H
