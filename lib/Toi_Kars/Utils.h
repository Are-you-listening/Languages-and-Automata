//
// Created by watson on 4/12/23.
//

#ifndef INC_1_UTILS_H
#define INC_1_UTILS_H

#include <vector>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

class State;

//Merge 2 Vectoren
vector<string> MergeVectors(vector<string> &a, vector<string> &b);

//Check if an item exists in a vector
bool Find(vector<string> &a, string &k);

//Convert the names of a state set correctly
string NameConvert(vector<string> &a);

//Convert the names of a state set correctly without "{}";
string NameConvert2(vector<string> &a);

//Merge 2 Vectoren
vector<State*> MergeVectors(vector<State*> &a, vector<State*> &b);

vector<string> eqClassToState(const string &a);

//Check if an item exists in a vector
bool Find(vector<State*> &a, State* &k);

//Check if a state is in a equivalents class; if yes: return the eqclass-name; if not: return class name
string ConvertToEqclassName(vector < vector<string> > &eqClass, const string &s);

#endif //INC_1_UTILS_H
