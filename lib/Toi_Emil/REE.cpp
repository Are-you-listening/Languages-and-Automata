//
// Created by anass on 2-5-2023.
//

#include "REE.h"
#include "string"
#include "iostream"

void REE::print() {
    cout << regex << endl;
}

REE::REE(const string &regex) : regex(regex) {}

const string &REE::getRegex() const {
    return regex;
}
