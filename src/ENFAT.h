//
// Created by tibov on 21/03/23.
//

#ifndef P1_ENFA_H
#define P1_ENFA_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "DFA.h"
#include "json.hpp"

using namespace std;

class DFA;

class ENFAT {
private:
    set<string> states;
    set<char> alfabet;

    string start_state;
    set<string> end_states;
    char epsilon;

    map<string, map<char, set<string>>> transition_map;
    map<string , set<string>> epsilon_transition_map;

    set<string> Eclose(const string& state);

    set<string> Eclose(const string& state, const set<string>& found);

    [[nodiscard]] string set_to_string(const set<string> &s) const;

public:
    explicit ENFAT(const string& path);

    ENFAT();

    DFA* toDFA();

    bool accepts(const string& s);

    void load(const nlohmann::json& data);
};

#endif //P1_ENFA_H
