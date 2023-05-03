//
// Created by tibov on 29/04/23.
//

#include "Note.h"
#include "iostream"
#include <string>
Note::Note(unsigned int time_stamp, bool note_on, int channel, int note,  int velocity): note_value(note), channel(channel), note_on(note_on), velocity(velocity) {
}

char Note::getRE(bool r_time_stamp, bool r_note_on, bool r_channel, bool r_note, bool r_velocity) const{
    long unsigned int index = time_stamp*r_time_stamp;
    index = index << 1;
    index += note_on*r_note_on;
    index = index << 4;
    index += channel*r_channel;
    index = index << 8;
    index += note_value*r_note;
    index = index << 8;
    index += velocity*r_velocity;
    std::cout << index << std::endl;
    wchar_t character = static_cast<wchar_t>(index);  //Convert to Unicode
    std::cout << "b " << character << std::endl;
    return character;
}
