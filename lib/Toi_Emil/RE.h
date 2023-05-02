//
// Created by emill on 11/04/2023.
//

#ifndef DFA_RE_H
#define DFA_RE_H

#include <string>
using namespace std;

class RE {
    string regex;
public:
    void print();

    explicit RE(const string &regex);
};


#endif //DFA_RE_H
