//
// Created by anass on 2-5-2023.
//

#ifndef TA_TOG_REE_H
#define TA_TOG_REE_H

#include <string>

using namespace std;

class REE {
    string regex;
public:
    void print();

    explicit REE(const string &regex);

    const string &getRegex() const;
};

#endif //TA_TOG_REE_H
