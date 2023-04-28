//
// Created by watson on 2/28/23.
//

#ifndef INC_1_DFA_H
#define INC_1_DFA_H

#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>

#include "json.hpp"
#include "transitiefunctie.h"
#include "Utils.h"

using namespace std;

class DFA {
private:
    vector<string> Q;
    vector<string> Alphabet;
    transitiefunctie delta;
    string q0;
    vector<string> F;
    vector< vector<char> >  table;

    //Check if a pair of 2 given states who are equivalent, needs to be merged with another Equivalence Classe, also merge if the case
    void checkEq(vector < vector<string> > &eqClass, string &q1 , string &q2);

    //Check of een staat in F zit (= accepting state)
    bool check_accepting_state(const string &z) const;

    //Find the index in Q of a state
    int find(const string &k) const;

    //generate the Reverse Transition table (used for TFA)
    map<char,map<string,vector<string> > > generateReverseTransitions() const;

    //Make out of 2 vectors of reachable states the new potentially distuingisable states
    vector<pair<string,string> > makePairs(vector<string> &q1, vector<string> &q2) const;

    void TFA();

public:
    //PRE: The states names of every DFA CANNOT contain any of these symbols: '#'
    DFA();

    DFA(const vector<string> &q, const vector<string> &alphabet, const transitiefunctie &delta, const string &q0,
        const vector<string> &f);

    DFA(const string &inputfile);

    //DFA(nlohmann::json dfa);

    DFA minimize();

    bool accepts(const string &invoer) const;

    void print() const;

    void printTable() const;

    bool operator==(DFA &rhs);

    bool operator!=(DFA &rhs);

    const vector<string> &getQ() const;

    const vector<string> &getAlphabet() const;

    const transitiefunctie &getDelta() const;

    const string &getQ0() const;

    const vector<string> &getF() const;

    const vector<vector<char>> &getTable() const;
};


#endif //INC_1_DFA_H
