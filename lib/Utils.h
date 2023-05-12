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

using namespace std;

bool DirectoryExists(const std::string dirname);

bool FileExists(const std::string dirname);

bool FileIsEmpty(const std::string filename);

bool FileCompare(const std::string leftFileName, const std::string rightFileName);

/**
 * \brief Merge 2 vectors and remove duplicates
 * @param vec1
 * @param vec2
 * @return
 */
vector<string> MergeVectors(vector<string> vec1, vector<string> vec2);

/**
 * \brief Export Curr time to String
 * @return
 */
string getCurrTime();

/**
 * \brief Convert a value to an unique charachter
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

#endif