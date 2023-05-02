//
// Created by emil on 24/03/23.
//

#ifndef DFA_DFA_H
#define DFA_DFA_H

#include "automaat.h"
#include "RE.h"

class DFA: public automaat{
friend class NFA;

public:
    DFA();
    DFA(DFA &other);
    explicit DFA(const string &fileName);
    void eliminateState(std::string eliminatedstate);
    RE toRE();
    string getselfconnection();
};


#endif //DFA_DFA_H
