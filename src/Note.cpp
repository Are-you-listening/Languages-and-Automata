//
// Created by tibov on 29/04/23.
//

#include "Note.h"
#include "iostream"
#include <string>
Note::Note(unsigned int time_stamp, bool note_on, int channel, int note,  int velocity, unsigned int instrument): note_value(note), channel(channel), note_on(note_on), velocity(velocity) {
}

char toChar(int value){
    int index = value;
    index -= 20;
    index += 48;
    if(index > 122){
        index += 51;
    }
    char c = int(index);
    return c;
}

string Note::getRE(bool r_time_stamp, bool r_note_on, bool r_channel, bool r_note, bool r_velocity) const{
    string s;
    s +=toChar(time_stamp*r_time_stamp);
    s +=toChar(note_on*r_note_on);
    s +=toChar(channel*r_channel);
    s +=toChar(note_value*r_note);
    s +=toChar(velocity*r_velocity);
    return s;
}
