//
// Created by watson on 4/12/23.
//

#include "Utils.h"

//Merge 2 Vectoren
vector<string> MergeVectors(vector<string> &a, vector<string> &b){
    for(auto k: b){
        if(!Find(a,k)){
            a.push_back(k);
        }
    }
    return a;
}

//Check if an item exists in a vector
bool Find(vector<string> &a, string &k){
    for(auto z: a){
        if(k==z){
            return true;
        }
    }
    return false;
}

//Convert the names of a state set correctly
string NameConvert(vector<string> &a){
    std::sort(a.begin(), a.end());

    string t = "{";

    for(int i=0; i<a.size()-1;i++){
        t+=a[i]+", ";
    }

    t+=a[a.size()-1]+"}"; //Voeg laatste toe

    return t;
}

//Merge 2 Vectoren
vector<State*> MergeVectors(vector<State*> &a, vector<State*> &b){
    for(State* k: b){
        if(!Find(a,k)){
            a.push_back(k);
        }
    }
    return a;
}

//Check if an item exists in a vector
bool Find(vector<State*> &a, State* &k){
    for(auto z: a){
        if(k==z){
            return true;
        }
    }
    return false;
}

string NameConvert2(vector<string> &a) {
    string t;

    for(int i=0; i<a.size()-1;i++){
        t+=a[i]+",";
    }

    t+=a[a.size()-1]; //Voeg laatste toe

    return t;
}

vector<string> eqClassToState(const string &a){
    string tempstring;
    vector<string> states;

    if(a[0]!='{'){
        return {a};
    }

    for(char k:a) {
        if ((k == '{')) {
            continue;
        } else if ((k == ',') || (k == '}')) {
            states.push_back(tempstring);
            tempstring = ""; //Reset temp
        } else if (k==' '){
            continue;
        } else {
            tempstring += k;
        }
    }
    return states;
}

string ConvertToEqclassName(vector < vector<string> > &eqClass, const string &s){
    for(vector<string> &k: eqClass){
        for(const string &z: k){
            if(s==z){
                return NameConvert(k);
            }
        }
    }
    return "{"+s+"}";
}