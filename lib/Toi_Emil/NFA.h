//
// Created by emil on 24/03/23.
//

#ifndef DFA_NFA_H
#define DFA_NFA_H

#include <vector>
#include <set>
#include "Node.h"
#include "automaat.h"
#include "DFA.h"
#include "WNFA.h"

class NFA: public automaat {
    friend class DFA;

private:
    string ssc_helper(const string& currentState, char input);

public:
    explicit NFA(const string &fileName);

    NFA();

    DFA toDFA();

    WNFA toWNFA();
};


#endif //DFA_NFA_H
