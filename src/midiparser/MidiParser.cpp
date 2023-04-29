
#include "MidiParser.h"
#include "ByteX.h"

MidiParser::MidiParser(const string &path) {
    stream.open(&path[0]);
    readHeader();

    for (unsigned int i = 0; i<track_amount; i++){
        readTrack();
        bool b = true;
        while (b){
            b = readComponent();
        }
    }
}

ByteX MidiParser::byteRead(int length) {
    char c = '\000';
    unsigned int original_byte = 0;
    for (int i = 0; i<length; i++){
        stream.get(c);

        unsigned int b;
        if (int(c) >= 0){
           b = int(c);
        }else{
            b = int(c) + 256;
        }

        original_byte = (original_byte << 8) | b;
    }

    ByteX output(original_byte, length);
    return output;
}

bool MidiParser::readComponent() {
    ByteX delta_time = byteRead(1);
    ByteX current_delta = delta_time;
    while (current_delta.getMSB(0)){
        current_delta = byteRead(1);
        delta_time.concatinateDeltaTime(current_delta);
    }

    delta_time_counter += delta_time.getValue();

    ByteX basic_data = byteRead(2);
    if (basic_data.equalsHex("ff", 0)){

        if (basic_data.equalsHex("01", 1) || basic_data.equalsHex("02", 1) || basic_data.equalsHex("03", 1) ||
            basic_data.equalsHex("04", 1)){
            ByteX data_length = byteRead(1);
            for (unsigned int i=0; i < data_length.getValue(); i ++){
                ByteX text = byteRead(1);
            }


        }else if (basic_data.equalsHex("51", 1)){
            ByteX value = byteRead(1);
            if (value.equalsHex("03", 0)){
                ms_per_quarter_note = byteRead(3).getValue();
            }
        }else if (basic_data.equalsHex("58", 1)){
            ByteX value = byteRead(1);
            if (value.equalsHex("04", 0)){
                byteRead(4);
            }
        }else if (basic_data.equalsHex("2f", 1)){
            byteRead(1);
            return false;
        }else if (basic_data.equalsHex("21", 1)){
            byteRead(2);
        }
    }else if (basic_data.equalsHex("c", 0)){

    }else if (basic_data.equalsHex("b", 0)){
        byteRead(1).toHex();
    }else{
        ByteX velocity = byteRead(1);
        unsigned int time = delta_time_counter*(ms_per_quarter_note/ticks_per_quarter_note)/1000;
        bool note_on = velocity.getValue() != 0;
        addNote(time, note_on, new Note(time, note_on, basic_data.getNibble(0, false),
                                        basic_data.getByte(1), velocity.getValue()));
    }

    return true;
}

void MidiParser::readTrack() {
    ByteX track_char = byteRead(4);

    ByteX track_size = byteRead(4);
    delta_time_counter = 0;
}

void MidiParser::readHeader() {
    ByteX header = byteRead(4);
    ByteX header_size = byteRead(4);

    ByteX format_type = byteRead(2);

    ByteX tracks_amount = byteRead(2);
    track_amount = tracks_amount.getValue();

    ByteX ticks_per_frame = byteRead(2);
    ticks_per_quarter_note = ticks_per_frame.getValue();
    ms_per_quarter_note = 0;
    delta_time_counter = 0;
}

void MidiParser::addNote(unsigned int time, bool note_on, Note* note) {
    pair<unsigned int, bool> p = make_pair(time, note_on);
    note_map[p].insert(note);
}

const map<pair<unsigned int, bool>, set<Note *>> &MidiParser::getNoteMap() const {
    return note_map;
}
