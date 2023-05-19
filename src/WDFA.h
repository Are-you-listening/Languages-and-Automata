//
// Created by emill on 10/05/2023.
//

#ifndef WNFA_WDFA_H
#define WNFA_WDFA_H

#include <list>
#include <string>
#include <set>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "WNFA.h"

using namespace std;
using json = nlohmann::json;

class WDFA: public WNFA {
private:

public:
    WDFA();
    WDFA(const string &filename);

    virtual void print() const override;

    virtual double weightedaccepts(string input) const override;
};

#endif //WNFA_WDFA_H
