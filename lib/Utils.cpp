//============================================================================
// Name        : TicTacToeUtils.cpp
// Author      : Serge Demeyer
// Version     : ?
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================
#include "Utils.h"
#include "../src/RE.h"

using namespace std;

/**
Auxiliary functions for file manipulation.
*/
bool DirectoryExists(const std::string dirname) {
	struct stat st;
	return stat(dirname.c_str(), &st) == 0;
}

bool FileExists(const std::string filename) {
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

bool FileIsEmpty(const std::string filename) {
	struct stat st;
	if (stat(filename.c_str(), &st) != 0) return true; // File does not exist; thus it is empty
	return st.st_size == 0;
}

bool FileCompare(const std::string leftFileName, const std::string rightFileName) {
	ifstream leftFile, rightFile;
	char leftRead, rightRead;
	bool result;

	// Open the two files.
	leftFile.open(leftFileName.c_str());
	if (!leftFile.is_open()) {
		return false;
	};
	rightFile.open(rightFileName.c_str());
	if (!rightFile.is_open()) {
		leftFile.close();
		return false;
	};

	result = true; // files exist and are open; assume equality unless a counterexamples shows up.
	while (result && leftFile.good() && rightFile.good()) {
		leftFile.get(leftRead);
		rightFile.get(rightRead);
		result = (leftRead == rightRead);
	};
	if (result) {
		// last read was still equal; are we at the end of both files ?
		result = (!leftFile.good()) && (!rightFile.good());
	};

	leftFile.close();
	rightFile.close();
	return result;
}

vector<string> MergeVectors(vector<string> vec1, vector<string> vec2) {
	for(long unsigned int i = 0; i < vec2.size(); i++){
		bool exists = false;
		for(long unsigned int j = 0; j < vec1.size(); j++){
			if(vec1[j] == vec2[i]){
				exists = true;
			}
		}
		if(!exists){
			vec1.push_back(vec2[i]);
		}
	}
	return vec1;
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