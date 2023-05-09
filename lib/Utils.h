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

vector<string> MergeVectors(vector<string> vec1, vector<string> vec2);

string getCurrTime();

#endif