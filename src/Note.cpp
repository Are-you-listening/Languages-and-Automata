//
// Created by tibov on 29/04/23.
//

#include "Note.h"

Note::Note(unsigned int time_stamp, bool note_on, int note,  int velocity, unsigned int instrument):
note_value(note), instrument(instrument), note_on(note_on), velocity(velocity), time_stamp(time_stamp) {
}

char toChar(int value){
    int index = value;
    index -= 20;
    index += 48;
    if(index > 122){
        index += 51;
    }

    if (index > 255){
        cout << "All i want for christmas, is an working function" << endl;
        throw "All i want for christmas, is an working function";
    }

    char c = int(index);
    return c;
}

[[nodiscard]] string Note::getRE(bool r_time_stamp, bool r_note_on, bool r_instrument, bool r_note, bool r_velocity) const{
    string s;
    s +=toChar(time_stamp*r_time_stamp);
    s +=toChar(note_on*r_note_on);
    s +=toChar(instrument*r_instrument);
    s +=toChar(note_value*r_note);
    s +=toChar((velocity/3)*r_velocity);
    return s;
}

int Note::getNoteValue() const {
    return note_value;
}
