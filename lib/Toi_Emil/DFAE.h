//
// Created by anass on 2-5-2023.
//

#ifndef TA__TOG_DFAE_H
#define TA__TOG_DFAE_H
#include "automaat.h"
#include "REE.h"
#include <sstream>

using namespace std;

class DFAE: public automaat{
    friend class NFAE;

public:
    DFAE();
    DFAE(const DFAE &other);
    explicit DFAE(const string &fileName);
    void eliminateState(const std::string &eliminatedstate);
    REE toREE() const;
    string getselfconnection();
};

#endif //TA__TOG_DFAE_H
