//
// Created by anass on 1-5-2023.
//

#ifndef TA_TOG_RE_H
#define TA_TOG_RE_H

#include <string>

#include "ENFA.h"

using namespace std;

class RE {
protected:
    ENFA plus(ENFA &enfa1, ENFA &enfa2) const;
    void kleene(ENFA &enfa) const;
    void concatenation(ENFA &enfa1, ENFA &enfa2);
    ENFA onechar(const string& c);

public:
    string re;
    string epsilon;
    RE(const string& re,const char epsilon);
    ENFA toENFA()&;
};

#endif //TA_TOG_RE_H
