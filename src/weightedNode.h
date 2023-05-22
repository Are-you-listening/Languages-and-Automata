//
// Created by emil on 27/04/23.
//

#ifndef DFA_WEIGHTEDNODE_H
#define DFA_WEIGHTEDNODE_H

#include <map>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class weightedNode{
private:
    /**
     * \brief Sorted map of the connections by input character
     */
    map<char,vector<pair< double,weightedNode* >> > connections;

    /**
     * \brief The name of this state
     */
    string name;

public:
    /**
     * \brief Add a new transition/connection
     * @param otherState
     * @param symbol
     * @param weight
     */
    void addconnection(weightedNode *otherState, char symbol, double weight);

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] const map<char,vector<pair< double,weightedNode* >> > & getweightedconnections() const;

    /**
     * \brief Simple Constructor
     * @param name
     */
    explicit weightedNode(string name);

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] const string &getName() const;

    /**
     * \brief Returns the reachable states with an input from this state
     * @param input
     * @return
     */
    vector<pair< double,weightedNode* >> accepts(const char &input);
};

#endif //DFA_WEIGHTEDNODE_H
