//
// Created by tibov on 09/05/23.
//

#ifndef TOG_MIDIEXPORTER_H
#define TOG_MIDIEXPORTER_H
#include <string>
#include <iostream>
#include "Note.h"
#include <vector>
#include <map>
#include "ByteX.h"
using namespace std;
class midiExporter {
public:
    midiExporter(const string& path, const map<pair<unsigned int, bool>, vector<Note *>>& note_map);
private:
    void changeFormat();
    void addNote(Note * note);

    map<pair<unsigned int, bool>, vector<Note *>> note_map;
    map<unsigned int, vector<pair<Note *, unsigned int>>> instrument_map;
    map<unsigned int, unsigned int> instrument_to_channel;

    void createTracks();

    unsigned int channel_counter;
    map<unsigned int, unsigned int> last_timestamp;
    string track_data;
    vector<ByteX> buffer;
    void store();
    string path;

};


#endif //TOG_MIDIEXPORTER_H
