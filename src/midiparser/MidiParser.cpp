
#include "MidiParser.h"

MidiParser::MidiParser(const string &path) {
    /**
     * Welkom in the midiparser, here some binary data will be converted to
     * more usefull data. Are you intrested in this very intresting journey of deciphering Bytes
     * Follow me through this documentation and you will achieve an basic understanding of our midi-parser
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

    stream.close();
}

ByteX MidiParser::byteRead(int length) {
    /**
     * reads the given amount of bytes (length)
     * */
    char c = '\000';
    long unsigned int original_byte = 0;
    for (int i = 0; i<length; i++){
        /**
         * we read a character first
         * later we convert this character to an unsigned int
         * cause we need to make first a normal integer it can be negative
         * so in that case we at 256
         * */
        stream.get(c);
        long unsigned int b;
        if (int(c) >= 0){
            b = int(c);
        }else{
            b = int(c) + 256;
        }
        /**
         * shift the already obtained value a byte to the left
         * */
        original_byte = (original_byte << 8) | b;
    }

    /**
     * store in a byte object
     * */
    ByteX output(original_byte, length);
    return output;
}

bool MidiParser::readComponent() {
    /**
     * in this function we will read a component from the midi file
     * */
    if (stream.eof()){
        /**
         * in case of an issue with the midi parser
         * it will throw an exception
         * */
        throw 1;
    }

    /**
     * reads the first delta time byte
     * if its MSB is true, read the next byte, till it is not true anymore
     * */
    ByteX delta_time = byteRead(1);
    ByteX current_delta = delta_time;
    while (current_delta.getMSB(0)){
        current_delta = byteRead(1);
        delta_time.concatinateDeltaTime(current_delta);
    }

    delta_time_counter += delta_time.getValue();

    /**
     * reads the basic data (2 bytes) the first byte/nibble will determine which action will
     * the second determines on the action
     * */
    ByteX basic_data = byteRead(2);

    if(!basic_data.getMSB(0) && status_running != -1){
        /**
        * this if statement is for support of running status
        * */
        unsigned int time = delta_time_counter*(ms_per_quarter_note/ticks_per_quarter_note)/1000;
        bool note_on = basic_data.getByte(1) != 0 && start;
        Note* current_note = new Note(time, note_on, basic_data.getByte(0), basic_data.getByte(1), instrument);
        addNote(time, note_on, current_note);
        //cout << "note " << basic_data.toHex() << " " << time << "i: "<< link_channel[channel] << endl;
        if(note_duration.find((channel << 8) + basic_data.getByte(0)) != note_duration.end()){
            Note* n = note_duration.at((channel << 8) + basic_data.getByte(0));

            int duration = current_note->getTimeStamp() - n->getTimeStamp();

            n->setDuration(duration);
            note_duration.erase((channel << 8) + basic_data.getByte(0));
        }

        if (note_on){
            note_duration[(channel << 8) + basic_data.getByte(0)] = current_note;
        }

    }else if (basic_data.equalsHex("ff", 0)){
        status_running = -1;
        /**
         * here are midi events
         * */

        if (basic_data.getNibble(1, true) == 0 && basic_data.getNibble(1, false) < 9){
            /**
            * these instructions are all instructions containing an extra byte giving an size of the data
            * this data contains of title/copyright text,...
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
            /**
             * changes the ms per quarter note
             * */
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
            /**
             * end of track event (returns false)
             * */
            byteRead(1);

            for(auto entry: note_duration){
                unsigned int time_stamp = entry.second->getTimeStamp();

                //entry.second->setDuration(delta_time_counter-time_stamp);
                entry.second->setDuration(1000);
            }

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
            /**
             * in case an undefined instruction occurs
             * */
            throw 2;
        }

    }
    else if (basic_data.equalsHex("c", 0)) {
        status_running = -1;
        /**
         * control message
         * says which channel corresponds with which instrument
         * */
        link_channel[basic_data.getNibble(0, false)] = basic_data.getByte(1);
    }else if(basic_data.equalsHex("d", 0)){
        status_running = -1;
        /**
         * checks for d nibble
         * */
    }else if (basic_data.equalsHex("a", 0) || basic_data.equalsHex("b", 0)|| basic_data.equalsHex("e", 0)){
        status_running = -1;
        /**
         * reads 1 extra byte in case of (status nibble a, b, e)
         * */
        byteRead(1);


    }else if(!basic_data.getMSB(0)){
        /**
         * checks for data byte
         * these will just be ignored if occured
         * */
    }else if (basic_data.equalsHex("f0", 0)){
        for (unsigned int i=0; i <basic_data.getByte(1); i++){
            byteRead(1);
        }
    }else if (basic_data.equalsHex("9", 0) || basic_data.equalsHex("8", 0)){
        /**
         * Here the bytes will be considered a Note On/ Note Off
         * the note will also be stored in the note_map
         * **/
        ByteX velocity = byteRead(1);
        unsigned int time = delta_time_counter*(ms_per_quarter_note/ticks_per_quarter_note)/1000;
        bool note_on = velocity.getValue() != 0 && basic_data.getNibble(0, true) == 9;
        status_running = basic_data.getNibble(1, true);
        start = note_on;
        //cout << "note " << basic_data.toHex() << " " << time << "v: " << velocity.getValue()<< "i: "<< link_channel[basic_data.getNibble(1, false)] << endl;
        Note* current_note =  new Note(time, note_on, basic_data.getByte(1), velocity.getValue(), link_channel[basic_data.getNibble(0, false)]);
        addNote(time, note_on, current_note);
        channel = basic_data.getNibble(0, false);
        instrument = link_channel[channel];

        if(note_duration.find((channel << 8) + basic_data.getByte(1)) != note_duration.end()){
            Note* n = note_duration.at((channel << 8) + basic_data.getByte(1));



            int duration = current_note->getTimeStamp() - n->getTimeStamp();
            n->setDuration(duration);
            note_duration.erase((channel << 8) + basic_data.getByte(1));
        }

        if (note_on){
            note_duration[(channel << 8) + basic_data.getByte(1)] = current_note;
        }

    }else{
        cout << "error basic " << basic_data.toHex()<< endl;
    }

    return true;
}

void MidiParser::readTrack() {
    /**
     * reads track data
     * */
    ByteX track_char = byteRead(4);
    ByteX track_size = byteRead(4);
    delta_time_counter = 0;
    link_channel.clear();
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
    /**
     * correctly add a note to the note map
     * */
    pair<unsigned int, bool> p = make_pair(time, note_on);
    note_map[p].push_back(note);
}

const map<pair<unsigned int, bool>, vector<Note *>> &MidiParser::getNoteMap() const {
    return note_map;
}

