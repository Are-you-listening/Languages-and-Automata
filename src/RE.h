//
// Created by anass on 1-5-2023.
//

#ifndef TA__TOG_RE_H
#define TA__TOG_RE_H

#include <string>

#include "ENFA.h"

using namespace std;

class RE {
protected:
    ENFA * plus(ENFA& enfa1, ENFA& enfa2);
    void kleene(ENFA& enfa);
    void concatenation(ENFA& enfa1, ENFA& enfa2)&;
    ENFA * onechar(const string& c);

public:
    string re;
    string epsilon;
    RE(const string& re,const char epsilon);
    ENFA toENFA()&;
};

#endif //TA__TOG_RE_H
