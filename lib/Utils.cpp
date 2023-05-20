//============================================================================
// Name        : TicTacToeUtils.cpp
// Author      : Serge Demeyer
// Version     : ?
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

#include "Utils.h"
#include "../src/RE.h"
#include "../src/DFA.h"

/**
Auxiliary functions for file manipulation.
*/
bool DirectoryExists(const std::string &dirname) {
	struct stat st;
	return stat(dirname.c_str(), &st) == 0;
}

bool FileExists(const std::string &filename) {
	struct stat st;
	if (stat(filename.c_str(), &st) != 0) return false;
	ifstream f(filename.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

bool FileIsEmpty(const std::string &filename) {
	struct stat st;
	if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
	return st.st_size == 0;
}

bool FileCompare(const std::string &leftFileName, const std::string &rightFileName) {
	ifstream leftFile, rightFile;
	char leftRead, rightRead;
	bool result;

	// Open the two files.
	leftFile.open(leftFileName.c_str());
	if (!leftFile.is_open()) {
		return false;
	}
	rightFile.open(rightFileName.c_str());
	if (!rightFile.is_open()) {
		leftFile.close();
		return false;
	}

	result = true; // files exist and are open; assume equality unless a counterexamples shows up.
	while (result && leftFile.good() && rightFile.good()) {
		leftFile.get(leftRead);
		rightFile.get(rightRead);
		result = (leftRead == rightRead);
	}
	if (result) {
		// last read was still equal; are we at the end of both files ?
		result = (!leftFile.good()) && (!rightFile.good());
	}

	leftFile.close();
	rightFile.close();
	return result;
}

string getCurrTime(){
    //Code from https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string str(buffer);
    return "["+str+"]";
}

char toChar(int value){
    int index = value;

    index += 48;
    if(index > 122){
        index += 51;
    }

    if (index > 255){
        cout << "All i want for christmas, is an working function" << endl;
        throw "All i want for christmas, is an working function";
    }

    if (index < 48){
        throw "dwdw";
    }

    char c = (char)(index);

    return c;
}

int toInt(const char x){
    int index = (int)(x);

    if (index < 0){
        index += 256;
    }

    if(index >= 173 ){
        index-=51;
    }
    index -=48;

    return index;
}

vector<vector<int>> makeNotes(const vector<vector<int>> &info){
     vector<vector<int>> notes;

     int index = 0;
     while(info[index].size()==0){ //In case some paramaters are not used, we should move on
         index++;
     }

     for(int k: info[index]){ //Set first options
         notes.push_back({k});
     }

     for(long unsigned int i=index+1; i<info.size(); i++) { //Add all possible, not yet created, options
         vector<vector<int>> notes2;
         for (int m: info[i]) { //Voor elke variatie
             for(auto s: notes){
                 s.push_back(m);
                 notes2.push_back(s);
             }
         }
         notes=notes2;
     }

    return notes;
}

string NodesToString(const map<string,weightedNode*> &s){
    if(s.empty()){
        return "{}";
    }

    string result = "{";
    auto it = s.begin();
    auto itr = s.end();
    itr--; //it is now 1 step before temp.end() = last state
    while(it!=itr){
        result += it->first+",";
        it++;
    }
    result+=itr->first+ "}"; //Add last state (no "," )
    return result;
}

[[nodiscard]] pair<vector<RE>, vector<RE>> sort(const pair<vector<RE>, vector<RE>> &t) {
    pair<vector<RE>, vector<RE>> k = t;
    if(t.first.size()>t.second.size()){
        k = {t.second, t.first};
    }
    return k;
}

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
    for(const auto &z: a){
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

    for(long unsigned int i=0; i<a.size()-1;i++){
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

    for(long unsigned int i=0; i<a.size()-1;i++){
        t+=a[i]+",";
    }

    t+=a[a.size()-1]; //Voeg laatste toe

    return t;
}

vector<string> eqClassToState(const string &a){
    string tempString;
    vector<string> states;

    if(a[0]!='{'){
        return {a};
    }

    for(char k:a) {
        if (k == '{') {
            continue;
        }else if ((k == ',') || (k == '}')) {
            states.push_back(tempString);
            tempString = ""; //Reset temp
        } else if (k==' '){
            continue;
        } else {
            tempString += k;
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

bool Found(vector<pair<string, vector<pair<char, string>>>> &processed, string &name) {
    for(auto &z: processed){
        if(z.first==name){
            return true;
        }
    }
    return false;
}

void MergeAlpabets(DFA &z, DFA &z2) {
    set<string> Difference;
    set<string> Difference2;

    set_difference(z.getAlphabet().begin(), z.getAlphabet().end(), z2.getAlphabet().begin(), z2.getAlphabet().end(),std::inserter(Difference, Difference.begin()));
    set_difference(z2.getAlphabet().begin(), z2.getAlphabet().end(), z.getAlphabet().begin(), z.getAlphabet().end(),std::inserter(Difference2, Difference2.begin()));

    state* deadstate = new state("{}", false, false);
    z2.AddState(deadstate);
    for (auto c = z2.getAlphabet().begin(); c != z2.getAlphabet().end(); c++) {
        deadstate->addTransitionFunction(*c, deadstate);
    }

    for (auto it = Difference.begin(); it != Difference.end(); it++) {
        //add to z2
        for (auto it2 = z2.getStates().begin(); it2 != z2.getStates().end(); it2++) {
            (*it2)->addTransitionFunction((*it), deadstate);
        }
    }

    deadstate = new state("{}", false, false);
    z.AddState(deadstate);
    for (auto c = z.getAlphabet().begin(); c != z.getAlphabet().end(); c++) {
        deadstate->addTransitionFunction(*c, deadstate);
    }

    for (auto it = Difference2.begin(); it != Difference2.end(); it++) {
        //add to z2
        for (auto it2 = z.getStates().begin(); it2 != z.getStates().end(); it2++) {
            (*it2)->addTransitionFunction((*it), deadstate);
        }
    }
}