//
// Created by watson on 4/12/23.
//

#ifndef INC_1_STATE_H
#define INC_1_STATE_H

#include <string>
#include <map>
#include <vector>

#include "DesignByContract.h"

using namespace std;

class State {
private:
    const string name;
    map<const char,vector<string>> transition; //input, next state
    const bool starting;
    const bool end;

    const State* fInitCheck;

public:
    vector<string> DoTransition(const char &a) const;

    const bool getStarting() const;

    const bool getAnEnd() const;

    void AddTransition(const char &a, string &state);

    State(const string &name, const bool starting, const bool anEnd);

    const string &getName() const;

    const map<const char, vector<string>> &getTransition() const;

    [[nodiscard]] bool ProperlyInitialized() const;
};


#endif //INC_1_STATE_H
