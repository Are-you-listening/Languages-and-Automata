//
// Created by anass on 1-5-2023.
//

#ifndef TA__TOG_NFA_H
#define TA__TOG_NFA_H

#include <queue>

#include "DFA.h"
#include "Toi_Kars/State.h"
#include "Toi_Kars/Utils.h"
#include "json.hpp"

class NFA {
private:
    vector<string> Alphabet;
    map<const string, State*> Q;

    //Find all states from a StringSetName: bv. "{p,q}"
    vector<State*> StringToState(const string &a) const;

    //Collect All Reachable States from a starting state
    //Start is in het format "{q0,q1}
    vector<pair<char,string>> FindReachableStates(const string &start) const;

    //HulpFunctie
    bool Found(vector< pair< string, vector<pair<char,string>> > > &processed, string &name) const;

    //Check up on alle names volgorde
    string SortNames(string &a, string &b) const;
    string SortName(string &a) const;

public:
    //Creates an DFA Object AND outputs a json file of that DFA
    DFA toDFA() const;

    NFA(const string &inputfile);

    virtual ~NFA();

};


#endif //TA__TOG_NFA_H
