//
// Created by watson on 4/12/23.
//

#include "State.h"

vector<string> State::DoTransition(const char &a) const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    if(transition.find(a) == transition.end()){
        return {};
    }else{
        return transition.find(a)->second;
    }
}

const bool State::getStarting() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return starting;
}

const bool State::getAnEnd() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return end;
}

void State::AddTransition(const char &a, string &state) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    if(transition.find(a)!=transition.end()){
        transition.find(a)->second.push_back(state);
    } else{
        transition.insert( {a, {state} } );
    }
}

State::State(const string &name, const bool starting, const bool anEnd)
        : name(name), starting(starting), end(anEnd) {
    fInitCheck = this;
    ENSURE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
}

const string &State::getName() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return name;
}

const map<const char, vector<string>> &State::getTransition() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return transition;
}

bool State::ProperlyInitialized() const {
    return fInitCheck==this;
}





