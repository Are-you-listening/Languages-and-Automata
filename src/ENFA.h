//
// Created by anass on 1-5-2023.
//

#ifndef TA_TOG_ENFA_H
#define TA_TOG_ENFA_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "json.hpp"
#include "ENFAT.h"
#include "DFA.h"

using namespace std;

using json = nlohmann::json;

class state;
class DFA;

class ENFA {
protected:
    map<pair<char,string>, vector<string>> possibleStates(state* nextState, const string &name)const&;
    vector<state*> ECLOSE(state* Etransition)const&;
    vector<state*> ECLOSE2(state* Etransition, string input)const&;
    void Etransitions(vector<state*> &states)const&;
    void Etransitions(vector<state*> &states, vector<state*> &found)const&;

public:
    bool deletable = false;
    state* startingState;
    vector<state*> states;
    set<string> alphabet;
    vector<state*> endstates;
    string eps;
    explicit ENFA(const string& c);
    ENFA(const ENFA& enfa,state* state);

    ENFA(state*startingState, const vector<state*> &states, const set<string> &alphabet,
         const vector<state*> &endstates, const string &eps);

    ENFA();
    [[nodiscard]] bool accepts(string c)const&;
    void print()const&;
    DFA* toDFA()&;
    void printStats()const&;
    void concatenate(const ENFA& enfa)&;
    void load(const json& j);
    [[nodiscard]] json getJson() const;

    /**
     * \brief Get the JSON of the converted NFA
     * ONLY works for Concatination ENFA
     * @return
     */
    [[nodiscard]] json getJsonNfa() const;

    ~ENFA();
};

#endif //TA_TOG_ENFA_H
