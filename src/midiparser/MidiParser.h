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
    /**
     * Welkom in the midiparser, here some binary data will be converted to
     * more usefull data. Are you intrested in this very intresting journey of deciphering Bytes
     * Follow me through this documentation and you will achieve an basic understanding of our midi-parser
     *
     * Each byte will be read, first we have an Header, containing the amount of Tracks
     * We read each track
     *
     * Their are a lot of byte combinations with their own meaning.
     * The most important and frequent combination is an note_on/note_off event
     * it means a note starts/ends
     * it has the following format: note_on:: delta_time 9[channel] note velocity
     * it has the following format: note_off:: delta_time 8[channel] note velocity(=0)
     *
     * the channel is 1 nibble big (value 0-15) and note, velocity is 1 byte
     * the delta time will be before every event. a delta time is nortmally 1 byte,
     * but if that byte its MSB is true than it also contains of the next byte, (it can go on for infinity)
     * we then concatinate those bytes ignoring the MSB, for example 81 01
     * 1000 0001 0000 0001 - > 00 0000 1000 0001 so 2 bytes become 14 bit etc.
     * */


    /**
     * Midiparser constructor
     * */
    MidiParser(const string& path);

    /**
     * getter for the note_map containing all the information
     * */
    const map<pair<unsigned int, bool>, vector<Note *>> &getNoteMap() const;

private:
    /**
     * stream of the file we are reading for information
     * */
    ifstream stream;

    /**
     * tool to get a certain amount of bytes out of the midiparser max (4) because
     * a unsigned int only exist of 4 bytes
     * */
    ByteX byteRead(int length);

    /**
     * reads a component from the .mid file mostly a music note
     * */
    bool readComponent();

    /**
     * reads a track header info
     * */
    void readTrack();

    /**
     * reads the main header file
     * */
    void readHeader();

    /**
     * some values that are used for some calculations
     * mainly calculating absolute time
     * */
    unsigned int track_amount = 0;
    unsigned int delta_time_counter = 0;
    unsigned int ms_per_quarter_note = 0;
    unsigned int ticks_per_quarter_note = 0;

    /**
     * store a note in the map
     * */
    void addNote(unsigned int time, bool note_on, Note* note);

    /**
     * note_map contains all the note information
     * */
    map<pair<unsigned int, bool>, vector<Note*>> note_map;

    /**
     * links a channel to an instrument
     * */
    map<unsigned int, unsigned int> link_channel;

    /**
     * their is a way midi files are a bit more compressed using running status
     * */
    int status_running = -1;
};

#endif //MIDIPARSER_MIDIPARSER_H
