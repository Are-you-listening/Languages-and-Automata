//
// Created by watson on 4/28/23.
//

#include "Song.h"

bool Song::ProperlyInitialized() const {
    if(fInitCheck==this){
        return true;
    }
    return false;
}

int Song::similarity(Song &song) const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    int percentage = 5;

    bool succes = false;

    if(percentage<=100 && percentage>=0){
        succes = true;
    }

    ENSURE(succes, "Percentage must be between 0 and 100");
    return percentage;
}

void Song::parse(const string &path) {
    MidiParser m(path);
    note_map = m.getNoteMap();
}

vector<RE> Song::toRegex(bool time_stamp, bool note_on, bool instrument, bool note_b, bool velocity, int pattern) const {
    char epsilon='*';
    vector<RE> regex_list;
    int count = 0;
    string temp;

    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        for(Note* note: it->second){
            string z = note->getRE(time_stamp, note_on, instrument, note_b, velocity);
            temp+=z;
            count++;
            if(count==pattern){
                //DO ACTUAL MERGE/MAKE REGEX
                RE regex(temp,epsilon);
                regex_list.push_back(regex);
                count=0;
                temp.clear();
            }
        }
    }

    if (count != 0){ //Add the resting regex parts incase we didn't got a full pattern run
        RE regex(temp,epsilon);
        regex_list.push_back(regex);
    }

    return regex_list;
}

Song::~Song(){
    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        for(Note* note: it->second){
            delete note;
        }
    }
}

