//
// Created by tibov on 09/05/23.
//

#ifndef TOG_SONGEXPORTER_H
#define TOG_SONGEXPORTER_H
#include <string>
#include <iostream>
#include "Note.h"
#include <vector>
#include <map>
using namespace std;
class SongExporter {
public:
    SongExporter(const string& path, const map<pair<unsigned int, bool>, vector<Note *>>& note_map);
private:
    void changeFormat();
    void addNote(Note * note);

    map<pair<unsigned int, bool>, vector<Note *>> note_map;
    map<unsigned int, vector<pair<Note *, unsigned int>>> instrument_map;
    map<unsigned int, unsigned int> instrument_to_channel;

    unsigned int channel_counter;
    unsigned int last_timestamp;
};


#endif //TOG_SONGEXPORTER_H
