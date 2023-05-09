//
// Created by tibov on 29/04/23.
//

#include "Note.h"

Note::Note(unsigned int time_stamp, bool note_on, int note,  int velocity, unsigned int instrument):
note_value(note), instrument(instrument), note_on(note_on), velocity(velocity), time_stamp(time_stamp) {
}

char toChar(int value){
    int index = value;

    index += 48;
    if(index > 122){
        index += 51;
    }

    if (index > 255){
        cout << "All i want for christmas, is an working function" << endl;
        throw "All i want for christmas, is an working function";
    }

    if (index < 48){
        throw "dwdw";
    }


    char c = (char)(index);

    return c;
}

[[nodiscard]] string Note::getRE(bool r_time_stamp, bool r_note_on, bool r_instrument, bool r_note, bool r_velocity, bool rounder) const{
    string s = "";
    s +=toChar(time_stamp*r_time_stamp);
    s +=toChar(note_on*r_note_on);
    s += RoundInstrument(rounder, r_instrument);
    s += RoundNote(note_value, r_note, rounder, 3, false, 0);
    s +=toChar((velocity/3)*r_velocity);
    return s;
}

int Note::getNoteValue() const {
    return note_value;
}

string Note::RoundInstrument(bool round_instrument, bool r_instrument) const {
    if (round_instrument){
        string s = "(";
        int base_instrument = instrument/8;
        for (int i=0; i<8; i++){
            s += toChar((base_instrument+i)*r_instrument);
            s += "+";
        }

        s[s.size()-1] = ')';
        return s;
    }else{
        string s = "";
        s += toChar(instrument*r_instrument);
        return s;
    }

}

string Note::RoundNote(int note_value, bool r_note, bool round, int round_index, bool round_octave, int round_index2) const {
    REQUIRE(note_value+round_index < 255, "note range not allowed");

    if (!round && !round_octave){
        string s = "";
        s += toChar(note_value*r_note);
        return s;
    }

    if (!round){
        round_index = 0;
    }

    if (!round_octave){
        round_index2 = 0;
    }

    int note_number = note_value % 12;
    int octave = note_value/ 12 - 1;

    string s = "(";
    for (int i=note_number-round_index; i<=note_number+round_index; i++){
        for (int j=octave-round_index2; j<=octave+round_index2; j++){

            int value = 12 * (j + 1) + i;

            if (value < 0 || value > 255){
                throw "oei";
            }

            s += toChar(value*r_note);
            s += "+";
        }

    }

    s[s.size()-1] = ')';
    return s;

}

unsigned int Note::getInstrument() const {
    return instrument;
}

unsigned int Note::getTimeStamp() const {
    return time_stamp;
}

int Note::getVelocity() const {
    return velocity;
}

bool Note::isNoteOn() const {
    return note_on;
}
