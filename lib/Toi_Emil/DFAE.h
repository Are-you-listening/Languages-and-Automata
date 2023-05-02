//
// Created by anass on 2-5-2023.
//

#ifndef TA__TOG_DFAE_H
#define TA__TOG_DFAE_H
#include "automaat.h"
#include "REE.h"

class DFAE: public automaat{
    friend class NFA;

public:
    DFAE();
    DFAE(DFAE &other);
    explicit DFAE(const string &fileName);
    void eliminateState(std::string eliminatedstate);
    REE toREE();
    string getselfconnection();
};



#endif //TA__TOG_DFAE_H
