//
// Created by anass on 2-5-2023.
//

#ifndef TA__TOG_DFAT_H
#define TA__TOG_DFAT_H
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

class DFAT {
public:
    DFAT();
    DFAT(const string &path);
    DFAT(const set<char> &alfabet, const set<string> &states, const map<string , map<char, string>>& transition_map, const string& start_state, const set<string> &end_states);
    bool accepts(const string& s);
    void print() const;
    DFAT minimize();
    void printTable();
    bool operator==(const DFAT& d);

    const map<string, map<char, string>> &getTransitionMap() const;

    const set<string> &getStates() const;

    const string &getStartState() const;

    const set<char> &getAlfabet() const;

    const set<string> &getEndStates() const;

private:
    set<string> states;
    set<char> alfabet;
    map<string , map<char, string>> transition_map;
    string start_state;
    set<string> end_states;
    map<string, map<string, bool>> table_filling_map;

    pair<set<string>, bool> get_eq(const string &state, const vector<set<string>>& equiv_vect);
    void add_eq(const string &state, const set<string>& eq, vector<set<string>>& equiv_vect);

    void printTable(const set<string>& p_states, const map<string, map<string, bool>>& tfa_map);
    void make_TFA();
    bool is_end_state(const string &state);
    void disqualified(const string &state1, const string &state2, const map<string , map<char, string>>& trans_map, map<string, map<string, bool>>& tfa_map, const set<string>& p_states);
    void set_table_filling_map(const string &state1, const string &state2, bool b, map<string, map<string, bool>>& tfa_map);
    bool in_table_filling_map(const string &state1, const string &state2, bool b, const map<string, map<string, bool>>& tfa_map);
    string set_to_string(const set<string>& s) const;


};


#endif //TA__TOG_DFAT_H
