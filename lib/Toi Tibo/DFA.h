//
// Created by tibov on 25/02/23.
//

#ifndef P1_DFA_H
#define P1_DFA_H
#include <set>
#include <string>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

class DFA {
public:
    DFA();
    DFA(const string &path);
    DFA(const set<char> &alfabet, const set<string> &states, const map<string , map<char, string>>& transition_map, const string& start_state, const set<string> &end_states);
    bool accepts(const string& s);
    void print() const;
    DFA minimize();
    void printTable();
    bool operator==(const DFA& d);

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


#endif //P1_DFA_H
