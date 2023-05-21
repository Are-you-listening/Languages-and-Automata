//
// Created by anass on 1-5-2023.
//

#ifndef TA__TOG_DFA_H
#define TA__TOG_DFA_H

#include <string>
#include <vector>
#include <set>
#include <map>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "ENFA.h"
#include "json.hpp"
#include "Toi_Tibo/DFAT.h"
#include "Toi_Emil/DFAE.h"
#include "Toi_Emil/REE.h"
#include "WDFA.h"
#include "Utils.h"
#include "DesignByContract.h"

using namespace std;

using json = nlohmann::json;

class ENFA;

class state{
public:
    state();

    state(const string &name,bool starting, bool accepting);

    map<string,state*> states;
    map<string,set<state*>> statesENFA;
    string name;
    bool starting;
    bool accepting;
    void addTransitionFunction(string c ,state* q);
    void addTransitionFunctionENFA(const string &c ,state* q);
    state* getComplement();
};

class DFA {
private:
    state* startingState;
    vector<state*> states;
    set<string> alphabet;
    vector<state*> endstates;
    json json_data;

public:
    void load(const json& j);
    void load(const set<string>& alfa, const vector<state*> &states, state* start_state, const vector<state*>& end_states);
    state*getStartingState() const;

    vector<vector<DFA*>> split(int split_size);
    void inRange(int range, set<state*>& out, set<state*>& last, set<state*>& end, state* current);

    void setStartingState(state*startingState);

    const vector<state*> &getStates() const;

    void setStates(const vector<state*> &states);

    const set<string> &getAlphabet() const;

    void setAlphabet(const set<string> &alphabet);

    const vector<state*> &getEndstates() const;

    void setEndstates(const vector<state*> &endstates);

    void AddState(state* k);

    DFA(const string& c);

    DFA();

    DFA(DFA* dfa1, DFA* dfa2, bool c);

    bool accepts(string c)const&;

    void print()const&;

    DFA* minimize();

    bool operator==(const DFA& d);

    json getJson() const;

    string ToRe();

    DFA* complement();

    ENFA reverse();

    ~DFA();
};

#endif //TA__TOG_DFA_H
