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
 * \brief Merge 2 vectors and remove duplicates
 * @param vec1
 * @param vec2
 * @return
 */
vector<string> MergeVectors(vector<string> &vec1, vector<string> &vec2);

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
char toChar(int &value);

/**
 * \brief Decode a character to it's original integer value
 * @param x
 * @return
 */
int toInt(const char &x);

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

#endif