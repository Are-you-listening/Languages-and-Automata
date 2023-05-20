//
// Created by anass on 1-5-2023.
//

#ifndef TA_TOG_NFA_H
#define TA_TOG_NFA_H

#include <queue>

#include "DFA.h"
#include "Toi_Kars/State.h"
#include "Utils.h"
#include "json.hpp"
#include "WNFA.h"
#include "weightedNode.h"

using json = nlohmann::json;

class NFA {
private:
    /**
     * \brief The supported alphabet of this Automaton
     */
    vector<string> Alphabet;

    /**
     * \brief Collection of states
     */
    map<const string, State*> Q;

    /**
     * \brief Find all states from a StringSetName: bv. "{p,q}"
     * @param a
     * @return
     */
    [[nodiscard]] vector<State*> StringToState(const string &a) const;

    /**
     * \brief Helper for SSC
     * Collect All Reachable States from a starting state | Start is in the format "{q0,q1}"
     * @param start
     * @return
     */
    [[nodiscard]] vector<pair<char,string>> FindReachableStates(const string &start) const;

    /**
     * \brief Sort the names of strings {B,A} => {A,B}
     * @param a
     * @param b
     * @return
     */
    string SortNames(string &a, string &b) const;

    /**
     * \brief Sort the names of strings {B,A} => {A,B}
     * @param a
     * @return
     */
    string SortName(string &a) const;

    void adaptDistance(vector<weightedNode*>& original, State* s, int distance, int index, double weight, const WNFA& result);

public:
    /**
     * \brief Load from a file
     * @param inputfile
     */
    explicit NFA(const string &inputfile);

    /**
     * \brief Load from json
     * @param j
     */
    explicit NFA(const json &j);

    /**
     * \brief Destructor to free memory
     */
    virtual ~NFA();

    /**
     * \brief Apply SSC to construct a DFA
     * @return
     */
    [[nodiscard]] DFA toDFA() const;

    /**
     * \brief WNFA converter
     * @return
     */
    WNFA toWNFA();
};

#endif //TA_TOG_NFA_H
