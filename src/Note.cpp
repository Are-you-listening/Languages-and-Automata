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
    s += RoundNote(rounder, note_value, 3, r_note);
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

string Note::RoundNote(bool round_instrument, int note_value, int round_index, bool r_note) const {
    REQUIRE(note_value+round_index < 255, "note range not allowed");

    if (round_instrument){
        string s = "(";
        for (int i=note_value-round_index; i<=note_value+round_index; i++){
            int i_out = i;
            if (i < 0){
                i_out = 0;
            }
            s += toChar(i_out*r_note);
            s += "+";
        }

        s[s.size()-1] = ')';
        return s;
    }else{
        string s = "";
        s += toChar(note_value*r_note);
        return s;
    }

}
