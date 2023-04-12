//
// Created by anass on 26-3-2023.
//

#ifndef DFA_RE_H
#define DFA_RE_H
#include <string>
#include "ENFA.h"

using namespace std;

class RE {
protected:
    ENFA* plus(ENFA& enfa1, ENFA& enfa2)const&;
    void kleene(ENFA& enfa)const&;
    void concatenation(ENFA& enfa1, ENFA& enfa2)const&;
    ENFA* onechar(const string& c)const&;
public:
    string re;
    string epsilon;
    RE(const string& re,const char epsilon);
    ENFA toENFA()const&;
};


#endif //DFA_RE_H
