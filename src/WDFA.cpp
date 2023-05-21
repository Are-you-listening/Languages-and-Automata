//
// Created by emill on 10/05/2023.
//

#include "WDFA.h"

WDFA::WDFA(const string &filename): WNFA(filename) {
    type="WDFA";
}

WDFA::WDFA() {type="WDFA";}

double WDFA::weightedaccepts(const string &input) const {
    weightedNode* currentState = startState;
    double result = 0.0;
    for (const char &symbol : input){ //Loop on every character of the string
        auto nextStates = currentState->accepts(symbol); //Get the reachable states with this symbol

        if(nextStates.empty()){ //No transitions
            result+=0;
        }else{ //The state accepted this symbol
            result+=nextStates[0].first; //If this is a correct DFA, there is only 1 reachable state by this input
            currentState = nextStates[0].second;
        }
    }
    return result/( (double) input.size());
}

void WDFA::print() const {
    json Jout;
    Jout["type"] = type;
    for (const auto &symbol : alfabet){
        Jout["alphabet"].push_back(symbol);
    }
    for (const auto &state: states) {  // add all the states
        json temp;
        temp["name"] = state.first;
        temp["starting"] = isStartState(state.first);
        temp["accepting"] = isEndState(state.first);
        Jout["states"].push_back(temp);
    }

    for(const auto &state: states) {  // add all the transitions
        for(const auto &transition: state.second->getweightedconnections()) {
            json temp;
            temp["from"] = state.second->getName();
            temp["to"] = transition.second[0].second->getName();
            temp["input"] = string(1, transition.first);
            temp["weight"] = transition.second[0].first;
            Jout["transitions"].push_back(temp);
        }
    }
    cout << setw(4) << Jout << endl;
}
