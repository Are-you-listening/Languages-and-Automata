//
// Created by watson on 4/12/23.
//

#include "State.h"

vector<string> State::DoTransition(const char &a) const {
    if(transition.find(a) == transition.end()){
        return {};
    }else{
        return transition.find(a)->second;
    }
}

const bool State::getStarting() const {
    return starting;
}

const bool State::getAnEnd() const {
    return end;
}

void State::AddTransition(const char &a, string &state) {
    if(transition.find(a)!=transition.end()){
        transition.find(a)->second.push_back(state);
    } else{
        transition.insert( {a, {state} } );
    }
}

State::State(const string &name, const bool starting, const bool anEnd)
        : name(name), starting(starting), end(anEnd) {}

const string &State::getName() const {
    return name;
}





