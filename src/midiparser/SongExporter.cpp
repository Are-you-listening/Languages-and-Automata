//
// Created by tibov on 09/05/23.
//

#include "SongExporter.h"
#include "DesignByContract.h"

SongExporter::SongExporter(const string &path, const map<pair<unsigned int, bool>, vector<Note *>> &note_map): note_map(note_map) {
    changeFormat();
}

void SongExporter::changeFormat() {
    channel_counter = 0;
    last_timestamp = 0;

    for (auto entry: note_map){
        for(Note* n: entry.second){
            addNote(n);
        }
    }

}

void SongExporter::addNote(Note* note) {
    REQUIRE(channel_counter <= 16 ,"out of range");
    if (instrument_map.find(note->getInstrument()) == instrument_map.end()){
         instrument_to_channel[note->getInstrument()] = channel_counter;
         channel_counter++;

    }

    unsigned int ticks_per_quarter_note = 480;
    unsigned int ms_per_quarter_note = 500*1000;
    unsigned int changed_time = note->getTimeStamp()*1000/(ms_per_quarter_note/ticks_per_quarter_note);
    unsigned int delta_time = changed_time - last_timestamp;
    last_timestamp = changed_time;

    instrument_map[note->getInstrument()].emplace_back(note, delta_time);
}
