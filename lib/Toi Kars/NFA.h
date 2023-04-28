//
// Created by watson on 4/12/23.
//

#ifndef INC_1_NFA_H
#define INC_1_NFA_H

#include "DFA.h"
#include "json.hpp"
#include "State.h"
#include "Utils.h"
#include "queue"

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


#endif //INC_1_NFA_H
