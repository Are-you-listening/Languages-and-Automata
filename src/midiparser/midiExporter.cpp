//
// Created by tibov on 09/05/23.
//

#include "midiExporter.h"
#include "DesignByContract.h"
#include "ByteX.h"
#include <fstream>

midiExporter::midiExporter(const string &path, const map<pair<unsigned int, bool>, vector<Note *>> &note_map): note_map(note_map), path(path) {
    changeFormat();
    store();

}

void midiExporter::changeFormat() {
    channel_counter = 0;
    
    for (auto entry: note_map){
        for(Note* n: entry.second){
            addNote(n, entry.first.second);
        }
    }

    ByteX header("4D546864");
    ByteX total_l(6, 4);
    ByteX format("0001");
    ByteX tracks_amount(instrument_map.size(),2);
    ByteX ticks_per_frame(120, 2);
    buffer.push_back(header);
    buffer.push_back(total_l);
    buffer.push_back(format);
    buffer.push_back(tracks_amount);
    buffer.push_back(ticks_per_frame);
    createTracks();

}

void midiExporter::addNote(Note* note, bool note_on) {
    REQUIRE(channel_counter <= 16 ,"out of range");
    if (instrument_map.find(note->getInstrument()) == instrument_map.end()){
         instrument_to_channel[note->getInstrument()] = channel_counter;
         channel_counter++;
         last_timestamp[note->getInstrument()] = 0;
    }

    unsigned int ticks_per_quarter_note = 120;
    unsigned int ms_per_quarter_note = 500*1000;
    unsigned int changed_time = note->getTimeStamp()*1000/(ms_per_quarter_note/ticks_per_quarter_note);
    unsigned int delta_time = changed_time - last_timestamp[note->getInstrument()];
    last_timestamp[note->getInstrument()] = changed_time;

    instrument_map[note->getInstrument()].emplace_back(note, delta_time, note_on);
}

void midiExporter::createTracks() {
    for(auto entry: instrument_map){
        unsigned int instrument = entry.first;
        unsigned int channel = instrument_to_channel.at(instrument);
        unsigned int track_length = 0;
        track_length += 3; //control message
        track_length += entry.second.size()*4; //note events
        track_length += 4; // end track message


        ByteX delta_time_c(0, 1); // delta time van control message
        ByteX control_byte(192+channel, 1); //set channel to instrument
        ByteX instrument_byte(instrument, 1); // set note



        vector<ByteX> sub_buffer;
        for(auto note: entry.second){
            Note* n = get<0>(note);
            unsigned int delta = get<1>(note);
            bool note_on = get<2>(note);
            unsigned int temp_delta = delta;
            unsigned int count = 1;

            vector<ByteX> delta_buffer;
            ByteX delta_time((temp_delta%128), 1);
            delta_buffer.push_back(delta_time);
            while((temp_delta >> 7) != 0){
                temp_delta = temp_delta >> 7;
                ByteX delta_time2((temp_delta%128+128), 1);
                delta_buffer.insert(delta_buffer.begin(), delta_time2);
                track_length += 1;
                count += 1;
            }

            sub_buffer.insert(sub_buffer.end(), delta_buffer.begin(), delta_buffer.end());

            //ByteX delta_time(delta, count); // delta time van control message

            if (note_on){
                ByteX note_on_byte(144+channel, 1); //set channel to instrument
                ByteX note_byte(n->getNoteValue(), 1);
                ByteX velocity(n->getVelocity(), 1);

                sub_buffer.push_back(note_on_byte);
                sub_buffer.push_back(note_byte);
                sub_buffer.push_back(velocity);
            }else{
                ByteX note_on_byte(128+channel, 1); //set channel to instrument
                ByteX note_byte(n->getNoteValue(), 1);
                ByteX velocity(n->getVelocity(), 1);

                sub_buffer.push_back(note_on_byte);
                sub_buffer.push_back(note_byte);
                sub_buffer.push_back(velocity);
            }

            //cout << "note " << n->getNoteValue() << " " << delta << endl;

        }
        ByteX track_header("4d54726b");
        ByteX track_size(track_length, 4);

        buffer.push_back(track_header);
        buffer.push_back(track_size);

        buffer.push_back(delta_time_c);
        buffer.push_back(control_byte);
        buffer.push_back(instrument_byte);

        buffer.insert(buffer.end(), sub_buffer.begin(), sub_buffer.end());

        ByteX end_track("00ff2f00");
        buffer.push_back(end_track);
        //do end line
    }
}

void midiExporter::store() {
    ofstream o(path, ios_base::binary);


    for(auto b: buffer){
        o << b.toString();
    }

    o.close();
}
