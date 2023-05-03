
#include "MidiParser.h"
#include "ByteX.h"

MidiParser::MidiParser(const string &path) {
    /**
     * Welkom in the midiparser, here some binary data will be converted to
     * more usefull data. Are you intrested in this very intresting journey of deciphering Bytes
     * Follow me through this documentation and you will achieve an basic understanding of our midi-parser
     * Voor we beginnen zullen we eerst duidelijk moeten
     * */
    stream.open(&path[0], ios_base::binary);
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
    long unsigned int original_byte = 0;
    for (int i = 0; i<length; i++){
        stream.get(c);

        long unsigned int b;
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

    if (stream.eof()){
        throw 1;
    }

    ByteX delta_time = byteRead(1);
    ByteX current_delta = delta_time;
    while (current_delta.getMSB(0)){
        current_delta = byteRead(1);
        delta_time.concatinateDeltaTime(current_delta);
    }

    delta_time_counter += delta_time.getValue();

    ByteX basic_data = byteRead(2);
    if (basic_data.equalsHex("ff", 0)){


        if (basic_data.getNibble(1, true) == 0 && basic_data.getNibble(1, false) < 9){
            /**
            * these instructions are all instructions containing an extra byte giving an size of the data
            **/
            ByteX data_length = byteRead(1);

            ByteX temp_data = data_length;
            while (temp_data.getMSB(0)){
                temp_data = byteRead(1);

                data_length.concatinateDeltaTime(temp_data);
            }

            for (long unsigned int i=0; i < data_length.getValue(); i ++){
                byteRead(1);
            }

        }else if (basic_data.equalsHex("4b", 1)){
            ByteX data_length = byteRead(1);
            for (unsigned int i=0; i < data_length.getValue(); i ++){
                byteRead(1);
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
        }else if (basic_data.equalsHex("54", 1)){
            byteRead(1);
            byteRead(2);
            byteRead(3);
        }else if (basic_data.equalsHex("59", 1)){
            byteRead(1).getValue();
            byteRead(2).toHex();
        }else if (basic_data.equalsHex("09", 1)) {
        }else if (basic_data.equalsHex("20", 1)) {

            byteRead(2);
        }else if (basic_data.equalsHex("7f", 1)){
            ByteX length = byteRead(1);
            for (unsigned int i=0; i < length.getValue(); i++){
                byteRead(1);
            }
        }else{
            cout << "error" << basic_data.toHex() << endl;
        }

    }else if (basic_data.equalsHex("c", 0)) {
        link_channel[delta_time_counter][basic_data.getNibble(0, false)] = basic_data.getByte(1);
    }else if(basic_data.equalsHex("d", 0)){
        /**
         * checks for c/e nibble
         * */
    }else if (basic_data.equalsHex("a", 0) || basic_data.equalsHex("b", 0)|| basic_data.equalsHex("e", 0)){
        /**
         * reads 1 extra byte
         * */
        byteRead(1);


    }else if(!basic_data.getMSB(0)){
        /**
         * checks for data byte
         * */

    }else if (basic_data.equalsHex("f0", 0)){
        for (unsigned int i=0; i <basic_data.getByte(1); i++){
            byteRead(1);
        }
    }else if (basic_data.equalsHex("9", 0) || basic_data.equalsHex("8", 0)){
        /**
         * Here the bytes will be considered a Note On/ Note Off
         * **/
        ByteX velocity = byteRead(1);
        unsigned int time = delta_time_counter*(ms_per_quarter_note/ticks_per_quarter_note)/1000;
        bool note_on = velocity.getValue() != 0 && basic_data.getNibble(0, true) == 9;

        addNote(time, note_on, new Note(time, note_on, basic_data.getNibble(0, false),
                                        basic_data.getByte(1), velocity.getValue(),
                                        get_closest_change(time, basic_data.getNibble(0, false))));
    }else{
        cout << "error basic " << basic_data.toHex()<< endl;
    }

    return true;
}

void MidiParser::readTrack() {
    ByteX track_char = byteRead(4);

    ByteX track_size = byteRead(4);
    delta_time_counter = 0;
}

void MidiParser::readHeader() {
    /**
     * Reads the header file, containing information about the amount of tracks, ticks per second
     * */
    ByteX header = byteRead(4);
    ByteX header_size = byteRead(4);

    ByteX format_type = byteRead(2);
    if (format_type.getValue() > 1){
        cout << "invalid format" << endl;
        throw 1;
    }
    ByteX tracks_amount = byteRead(2);
    track_amount = tracks_amount.getValue();

    ByteX ticks_per_frame = byteRead(2);
    ticks_per_quarter_note = ticks_per_frame.getValue();
    ms_per_quarter_note = 500000;
    delta_time_counter = 0;
}

void MidiParser::addNote(unsigned int time, bool note_on, Note* note) {
    pair<unsigned int, bool> p = make_pair(time, note_on);
    note_map[p].insert(note);
}

const map<pair<unsigned int, bool>, set<Note *>> &MidiParser::getNoteMap() const {
    return note_map;
}


const map<unsigned int, map<unsigned int, unsigned int>> &MidiParser::getLinkChannel() const {
    return link_channel;
}

unsigned int MidiParser::get_closest_change(unsigned int time, unsigned int channel) {
    unsigned int best_time = 0;
    unsigned int best = 0;
    for (auto entry: link_channel){
        if (entry.second.find(channel) != entry.second.end() && entry.first < time && entry.first > best_time){
            unsigned int value = entry.second.at(channel);
            best = value;
            best_time = entry.first;
        }
    }
    return best;
}
