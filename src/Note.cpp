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

[[nodiscard]] string Note::getRE(int r_time_stamp, int r_note_on, int r_instrument, int r_note, int r_velocity, int octaaf) const{
    string s = "";
    s += RoundTime_stamp(r_time_stamp);
    s += RoundNote_on(r_note_on);
    s += RoundInstrument(r_instrument);
    s += RoundNote(r_note,0);
    s += RoundVelocity(r_velocity);
    return s;
}

int Note::getNoteValue() const {
    return note_value;
}

string Note::RoundTime_stamp(int r_time_stamp) const{ //TODO hier wordt nog geen rekening gehouden met range
    string s;
    if(r_time_stamp>1){
        s="(";
        for(int i=-r_time_stamp;  i!=r_time_stamp+1; i++){
            if(i<=time_stamp+(r_time_stamp-1)){
                s+=toChar(div(i,155).rem);
                s+="+";
            } else {
                s+=")";
            }
        }
    } else if (r_time_stamp==1){
        s=toChar(div(time_stamp,155).rem);
    } else {
        s="";
    }
    return s;
}

string Note::RoundNote_on(int r_note_on) const{ //TODO hier wordt nog geen rekening gehouden met range
    string s; // TODO note_on wordt hierbehanld als duration. naamgeving moet nog wijziginen
    if(r_note_on>1){
        s="(";
        for(int i=note_on-(r_note_on-1);  i<=note_on+(r_note_on-1); i++){
            if(i<=note_on+(r_note_on-1)-1){
                s+=toChar(div(i,155).rem);
                s+="+";
            } else {
                s+=")";
            }
        }
    } else if(r_note_on==1){
        s=toChar(div(note_on,155).rem);
    } else {
        s="";
    }
    return s;
}

string Note::RoundVelocity(int r_velocity) const{ //TODO hier wordt nog geen rekening gehouden met range
    string s;
    if(r_velocity > 1){
        s="(";
        for(int i=velocity-(r_velocity-1);  i<=velocity+(r_velocity-1); i++){
            if(i<=velocity+(r_velocity-1)-1){
                s+=toChar(div(i,155).rem);
                s+="+";
            } else {
                s+=")";
            }
        }
    } else if(r_velocity==1){
        s = toChar(div(velocity,155).rem);
    } else {
        s="";
    }
    return s;
}

string Note::RoundInstrument(int r_instrument) const {
    if (r_instrument > 2){
        string s = "(";
        int base_instrument = instrument/8;
        for (int i=0; i<8; i++){
            s += toChar(((base_instrument*8)+i));
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

string Note::RoundNote(int r_note, int r_octaaf) const { // TODO deze functie moet zeker nog een keer besproken worken. octaaf is redelijk belangerijk

    if (r_note < 2 && r_octaaf < 2){
        string s = "";
        s += toChar(note_value*r_note);
        return s;
    }

    int note_number = note_value % 12;
    int octave = note_value/ 12 - 1;

    string s = "(";
    for (int i=note_number-(r_note-1); i<=note_number+(r_note-1); i++){
        for (int j=octave-(r_octaaf-1); j<=octave+(r_octaaf-1); j++){

            int value = 12 * (j + 1) + i;
            if (value < 0){
                value = 0;
            }
            if (value > 255){
                throw "oei";
            }

            s += toChar(value*r_note);
            s += "+";
        }
    }

    s[s.size()-1] = ')';
    return s;
}

Note::Note(const Note &n) {
    this->time_stamp = n.time_stamp;
    this->note_on = n.note_on;
    this->note_value = n.note_value;
    this->velocity = n.velocity;
    this->instrument = n.instrument;
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
