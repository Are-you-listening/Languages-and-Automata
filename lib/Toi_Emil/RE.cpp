//
// Created by emill on 11/04/2023.
//

#include "RE.h"
#include <iostream>


void RE::print() {
    cout << regex << endl;
}

RE::RE(const string &regex) : regex(regex) {}
