//
// Created by emill on 10/05/2023.
//

#ifndef WNFA_WNFA_H
#define WNFA_WNFA_H

#include <queue>
#include <string>
#include <set>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "json.hpp"
#include "weightedNode.h"
#include "../lib/Utils.h"

using namespace std;
using json = nlohmann::json;

class WDFA;

class WNFA {
private:
    /**
     * \brief Get the reachable states from a current set of state's by a symbol input and the greatest associated weight
     * @param currentstates
     * @param input
     * @return
     */
    [[nodiscard]] static pair< map<string,weightedNode*> , double> WSSC_helper(const map<string,weightedNode*> &currentstates, const char& input) ;

    /**
     * \brief Split a string (e.g. "{1,2,3}") and retrieve it's actual Nodes
     * @param str
     * @return
     */
    [[nodiscard]] map<string,weightedNode*> splitString(const string& str) const;

protected:
    /**
     * \brief Collection of the states
     */
    map<string, weightedNode*> states;

    /**
     * \brief The start state
     */
    weightedNode* startState = nullptr;

    /**
     * \brief Collection of end states
     */
    map<string, weightedNode*> endStates;

    /**
     * \brief The type of this automaton (WDFA)
     */
    string type;

    /**
     * \brief The supported alfabet of this automaton
     */
    vector<string> alfabet;

public:
    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] const map<string, weightedNode *> &getStates() const;

    /**
     * \brief Simple setter
     * @return
     */
    void setStates(const map<string, weightedNode *> &states);

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] weightedNode *getStartState() const;

    /**
     * \brief Simple setter
     * @return
     */
    void setStartState(weightedNode *startState);

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] const map<string, weightedNode *> &getEndStates() const;

    /**
     * \brief Simple setter
     * @return
     */
    void setEndStates(const map<string, weightedNode *> &endStates);

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] const string &getType() const;

    /**
     * \brief Simple setter
     * @return
     */
    void setType(const string &type);

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] const vector<string> &getAlfabet() const;

    /**
     * \brief Simple setter
     * @return
     */
    void setAlfabet(const vector<string> &alfabet);

    /**
     * \brief Check if the state is the start state
     * @param name
     * @return
     */
    [[nodiscard]] bool isStartState(const string &name) const;

    /**
     * \brief Check if a state is accepting
     * @param name
     * @return
     */
    [[nodiscard]] bool isEndState(const string &name) const;

    /**
     * \brief Add a new state
     * @param name
     * @param start
     * @param endState
     */
    void addState(string& name, bool start, bool endState);

    /**
     * \brief Retrieve the state with a given name
     * @param name
     * @return {ptr , succes}
     */
    [[nodiscard]] pair<weightedNode*, bool> getState(const string &name) const;

    /**
     * \brief Empty constructor
     */
    WNFA();

    /**
     * \brief Construct from a file//Load
     * @param filename
     */
    explicit WNFA(const string &filename);

    /**
     * \brief Check if a certain string is accepted and returns its weight
     * @param input
     * @return
     */
    [[nodiscard]] virtual double weightedaccepts(const string &input) const;

    /**
     * \brief Print this object to the terminal
     */
    virtual void print() const;

    /**
     * \brief Convert to a WDFA using a form of the SubSetConstruction (SSC)
     * @return The newly created WDFA
     */
    WDFA toWDFA();

    /**
     * \brief Destructor to free used memory
     */
    //virtual ~WNFA();
};

#endif //WNFA_WNFA_H
