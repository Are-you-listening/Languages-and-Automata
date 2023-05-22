//
// Created by anass on 1-5-2023.
//

#ifndef TA_TOG_DFA_H
#define TA_TOG_DFA_H

#include <string>
#include <vector>
#include <set>
#include <map>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>

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
    const state* fInitCheck;
public:
    state();

    state(string name,bool starting, bool accepting);

    map<string,state*> states;
    map<string,set<state*>> statesENFA;
    string name;
    bool starting;
    bool accepting;
    void addTransitionFunction(string c ,state* q);
    void addTransitionFunctionENFA(const string &c ,state* q);
    state* getComplement() const;

    [[nodiscard]] bool ProperlyInitialized() const;
};

class DFA {
private:
    state* startingState;
    map<string,state*> states;
    set<string> alphabet;
    vector<state*> endstates;
    json json_data;

public:
    void load(const json& j);
    void load(set<string> & alfa, map<string,state*> &States, state* start_state, vector<state*> & end_states);
    [[nodiscard]] state* getStartingState() const;

    vector<vector<DFA*>> split(int split_size);
    void inRange(int range, set<state*>& out, set<state*>& last, set<state*>& end, state* current);

    void setStartingState(state*startingState);

    [[nodiscard]] const map<string, state *> &getStates() const;

    void setStates(const map<string, state *> &States);

    [[nodiscard]] const set<string> &getAlphabet() const;

    void setAlphabet(const set<string> &Alphabet);

    void setEndstates(const vector<state*> &Endstates);

    void AddState(state* k);

    explicit DFA(const string& c);

    DFA();

    DFA(DFA* dfa1, DFA* dfa2, bool c);

    [[nodiscard]] bool accepts(string c)const&;

    void print()const&;

    DFA* minimize() const;

    bool operator==(const DFA& d) const;

    [[nodiscard]] json getJson() const;

    [[nodiscard]] string ToRe() const;

    DFA* complement();

    ENFA reverse();

    ~DFA();

    void ExtendAlphabet(set<string> &k);
};

#endif //TA_TOG_DFA_H