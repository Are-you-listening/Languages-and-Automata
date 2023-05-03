//
// Created by tibov on 26/04/23.
//

#ifndef MIDIPARSER_MIDIPARSER_H
#define MIDIPARSER_MIDIPARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "../Note.h"
#include <map>
#include <set>
#include "ByteX.h"

using namespace std;


class MidiParser {
public:
    MidiParser(const string& path);

    const map<pair<unsigned int, bool>, set<Note *>> &getNoteMap() const;

    bool valid();

private:
    ifstream stream;

    ByteX byteRead(int length);

    bool readComponent();

    void readTrack();

    void readHeader();

    unsigned int track_amount;
    unsigned int delta_time_counter;
    unsigned int ms_per_quarter_note;
    unsigned int ticks_per_quarter_note;

    void addNote(unsigned int time, bool note_on, Note* note);

    map<pair<unsigned int, bool>, set<Note*>> note_map;


};

#endif //MIDIPARSER_MIDIPARSER_H
