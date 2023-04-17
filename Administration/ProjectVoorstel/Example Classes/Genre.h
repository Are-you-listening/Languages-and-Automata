//
// Created by watson on 4/17/23.
//

#ifndef TOG_GENRE_H
#define TOG_GENRE_H

#include "Song.h"

class DFA;

class Genre {
    vector<Song*> songs;
    bool Full; //If enabled: the songs are all checked by the Regex formed by toRegex()
    bool Channel; //If enabled: the songs are all checked by the Regex formed by toRegexChannel()
    bool Letters; //If enabled: the songs are all checked by the Regex formed by toRegexLettersl()
    bool Speed;

public:

    DFA toProductAutomata();
};


#endif //TOG_GENRE_H
