//
// Created by watson on 4/17/23.
//

#ifndef TOG_SONG_H
#define TOG_SONG_H

#include <vector>
#include <map>

#include "../../../lib/DesignByContract.h"
#include "Note.h"
#include "iostream"
#include "Parser.h"

using namespace std;

class Regex;

class Song {
    string title;
    vector<Note*> notes;
    std::map<int,Note*> notesz; // int = timestamp , Node = Node

public:
    Song();

    void Parse(fstream &file);

    void Partition() const;

    void toRegex() const;	 //(Alles)

    Regex toRegexToonHoogte();		//	A = B // (A+B)CD

    Regex toRegexChannel(); 			//A55:12

    Regex toRegexVelocity();			//A5 A5 A5

    Regex toRegexLettersNote();			// AAAAA
};


#endif //TOG_SONG_H
