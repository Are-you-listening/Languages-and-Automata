//
// Created by anass on 2-5-2023.
//

#ifndef TA__TOG_NFAE_H
#define TA__TOG_NFAE_H
#include <vector>
#include <set>
#include "Node.h"
#include "automaat.h"
#include "DFAE.h"

class NFAE: public automaat {
    friend class DFAE;

private:
    string ssc_helper(const string& currentState, char input);

public:
    explicit NFAE(const string &fileName);

    NFAE();

    DFAE toDFAE();
};


#endif //TA__TOG_NFAE_H
