//============================================================================
// Name        : TicTacToeUtils.h
// Author      : Serge Demeyer
// Version     : ?
// Copyright   : Project Software Engineering - BA1 Informatica - Serge Demeyer - University of Antwerp
// Description : TicTactToe in C++, Ansi-style
//============================================================================

// Below are two lines serving as a so-called ``header guard''.
// Prevents from loading the same .h file multiple times
#ifndef Utils_H
#define Utils_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <vector>

using namespace std;

bool DirectoryExists(const std::string dirname);

bool FileExists(const std::string dirname);

bool FileIsEmpty(const std::string filename);

bool FileCompare(const std::string leftFileName, const std::string rightFileName);

vector<string> MergeVectors(vector<string> vec1, vector<string> vec2);

#endif