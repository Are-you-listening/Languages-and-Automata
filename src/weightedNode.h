//
// Created by emil on 27/04/23.
//

#ifndef DFA_WEIGHTEDNODE_H
#define DFA_WEIGHTEDNODE_H

#include <map>
#include <vector>

using namespace std;

class weightedNode{
    map<char,vector<pair< double,weightedNode* >> > connections; //Old: list<tuple<weightedNode *, set<char>, double>>

    string name;

public:
    void addconnection(weightedNode *otherState, char symbol, double &weight);

    const map<char,vector<pair< double,weightedNode* >> > & getweightedconnections() const;

    /**
     * \brief Simple Constructor
     * @param name
     */
    weightedNode(const string &name);

    const string &getName() const;

    vector<pair< double,weightedNode* >> accepts(const char &input);
};

#endif //DFA_WEIGHTEDNODE_H