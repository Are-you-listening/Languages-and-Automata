//============================================================================
// Name        : Utils.h
// Author      :
// Version     : ?
// Copyright   : Serge Demeyer: DirectoryExists, FileExists, FileIsEmpty, FileCompare ; max: getCurrTime (https://stackoverflow.com/questions/16357999/current-date-and-time-as-string)
//============================================================================

// Below are two lines serving as a so-called ``header guard''.
// Prevents from loading the same .h file multiple times
#ifndef Utils_H
#define Utils_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <ctime>

#include "weightedNode.h"

using namespace std;

class RE;
class State;
class DFA;

/**
 * \brief Check if the given directory exists
 * @param dirname
 * @return
 */
bool DirectoryExists(const std::string &dirname);

/**
 * \brief Check if a File is existing
 * @param dirname
 * @return
 */
bool FileExists(const std::string &dirname);

/**
 * \brief Check if a file is empty
 * @param filename
 * @return
 */
bool FileIsEmpty(const std::string &filename);

/**
 * \brief Compare 2 files
 * @param leftFileName
 * @param rightFileName
 * @return
 */
bool FileCompare(const std::string &leftFileName, const std::string &rightFileName);

/**
 * \brief Export Curr time to String
 * @return
 */
string getCurrTime();

/**
 * \brief Convert a value to an unique character
 * @param value
 * @return
 */
char toChar(int value);

/**
 * \brief Decode a character to it's original integer value
 * @param x
 * @return
 */
int toInt(const char x);

/**
 * \brief Convert 'info' to possible note combinations
 * @param info { {possible time_stamps}, {possible durations}, {possible notes},  {possible velocity's}, {possible instruments} }
 * @return a list with the Notes (and its params)
 */
vector<vector<int>> makeNotes(const vector<vector<int>> &info);

/**
 * \brief Convert different notes to a string in equivalent class form (E.g. {1,2,3} )
 * @param s
 * @return
 */
string NodesToString(const map<string,weightedNode*> &s);

/**
 * \brief Set the biggest vector to the second argument
 * @param t
 * @return
 */
[[nodiscard]] pair<vector<RE>, vector<RE>> sort(const pair<vector<RE>, vector<RE>> &t);

/**
 * \brief Merge 2 Vectoren
 * @param a
 * @param b
 * @return
 */
vector<string> MergeVectors(vector<string> &a, vector<string> &b);

/**
 * \brief Check if an item exists in a vector
 * @param a
 * @param k
 * @return
 */
bool Find(vector<string> &a, string &k);

/**
 * \brief Convert the names of a state set correctly
 * @param a
 * @return
 */
string NameConvert(vector<string> &a);

/**
 * \brief Convert the names of a state set correctly without "{}";
 * @param a
 * @return
 */
string NameConvert2(vector<string> &a);

/**
 * \brief Merge 2 Vectoren
 * @param a
 * @param b
 * @return
 */
vector<State*> MergeVectors(vector<State*> &a, vector<State*> &b);

/**
 * \brief Convert a name (E.g. {1,2,3} to vector{1 , 2 ,3}
 * @param a
 * @return
 */
vector<string> eqClassToState(const string &a);

/**
 * \brief Check if an item exists in a vector
 * @param a
 * @param k
 * @return
 */
bool Find(vector<State*> &a, State* &k);

/**
 * \brief Check if a state is in a equivalents class; if yes: return the eqclass-name; if not: return class name
 * @param eqClass
 * @param s
 * @return
 */
string ConvertToEqclassName(vector < vector<string> > &eqClass, const string &s);

/**
 * \brief Internal helper function for NFA
 * @param processed
 * @param name
 * @return
 */
bool Found(vector<pair<string, vector<pair<char, string>>>> &processed, string &name);

/**
 * \brief Merge the alpabets of 2 DFA's, helper function for Genre
 * @param z
 * @param z2
 */
void MergeAlpabets(DFA &z, DFA &z2);

#endif