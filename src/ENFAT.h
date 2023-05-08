//
// Created by tibov on 21/03/23.
//

#ifndef P1_ENFA_H
#define P1_ENFA_H
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "DFA.h"

using namespace std;


class ENFAT {
public:
    ENFAT(const string& path);
    ENFAT();
    DFA toDFA();
    bool accepts(const string& s);
    void load(const nlohmann::json& data);
private:
    set<string> states;
    set<char> alfabet;
    map<string , map<char, set<string>>> transition_map;
    string start_state;
    set<string> end_states;
    char epsilon;

    set<string> Eclose(const string& state);
    set<string> Eclose(const string& state, const set<string>& found);
    string set_to_string(const set<string> &s) const;

};


#endif //P1_ENFA_H
