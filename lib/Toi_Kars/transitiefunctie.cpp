//
// Created by watson on 2/28/23.
//

#include "transitiefunctie.h"

string transitiefunctie::transition(const string &toestand, const string &x) const {
    string start = toestand;
    for(int i = 0 ; i<x.size() ; i++){
        char a = x[i];
        start = simple_transition(start,a);
        }
    return start;
}

string transitiefunctie::simple_transition(const string &toestand,const char &a) const {
    for(int i = 0;i<function.size() ; i++ ){
        if(function[i].first.first==toestand && function[i].first.second==a){
            return function[i].second;
        }
    }
    return "";
}

void transitiefunctie::setFunction(vector<pair<pair<string, char>, string>> function) {
    transitiefunctie::function = function;
}

transitiefunctie::transitiefunctie(vector<pair<pair<string, char>, string>> &function) : function(function) {}

transitiefunctie::transitiefunctie() {}

void transitiefunctie::add(pair<pair<string, char>, string> x) {
    function.push_back(x);
}

const vector<pair<pair<string, char>, string>> &transitiefunctie::getFunction() const {
    return function;
}

void transitiefunctie::clear() {
    this->function.clear();
}

void transitiefunctie::changeStateName(string &state, string &newName) {
    for(auto &k: function){
        if(k.first.first==state){
            k.first.first=newName;
        }
        if(k.second==state){
            k.second=newName;
        }
    }
}
