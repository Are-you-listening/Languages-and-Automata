//
// Created by tibov on 26/04/23.
//

#ifndef MIDIPARSER_MIDIPARSER_H
#define MIDIPARSER_MIDIPARSER_H

#include <string>
#include <vector>
#include <set>
#include <map>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "ByteX.h"
#include "../Note.h"

using namespace std;

class MidiParser {
public:
    MidiParser(const string& path);

    const map<pair<unsigned int, bool>, vector<Note *>> &getNoteMap() const;

    const map<unsigned int, map<unsigned int, unsigned int>> &getLinkChannel() const;

private:
    ifstream stream;

    ByteX byteRead(int length);

    bool readComponent();

    void readTrack();

    void readHeader();

    unsigned int track_amount = 0;
    unsigned int delta_time_counter = 0;
    unsigned int ms_per_quarter_note = 0;
    unsigned int ticks_per_quarter_note = 0;

    void addNote(unsigned int time, bool note_on, Note* note);

    unsigned int get_closest_change(unsigned int time, unsigned int channel);

    map<pair<unsigned int, bool>, vector<Note*>> note_map;
    map<unsigned int, map<unsigned int, unsigned int> > link_channel;
    int status_running = -1;
    int channel = -1;


};

#endif //MIDIPARSER_MIDIPARSER_H
