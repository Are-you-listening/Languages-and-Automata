//
// Created by emil on 27/04/23.
//

#ifndef DFA_WNFA_H
#define DFA_WNFA_H

#include "weightedautomaat.h"

class weightedNode;

class WNFA: public weightedautomaat{
public:
    WNFA(const string &filename);
    double weightedaccepts(string input) override;
};


#endif //DFA_WNFA_H
