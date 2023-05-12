//
// Created by tibov on 29/04/23.
//

#include "Note.h"

Note::Note(unsigned int time_stamp, int duration, int note,  int velocity, unsigned int instrument):
note_value(note), instrument(instrument), duration(duration), velocity(velocity), time_stamp(time_stamp) {
}

[[nodiscard]] string Note::getRE(int r_time_stamp, int r_duration, int r_instrument, int r_note, int r_velocity, int octaaf) const{
    string s = "";
    s += RoundTime_stamp(r_time_stamp);
    s += RoundDuration(r_duration);
    s += RoundInstrument(r_instrument);
    s += RoundNote(r_note,1);
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
        for(int i=::lround(time_stamp/1000)-(r_time_stamp-1);  i!=::lround(time_stamp/1000)+(r_time_stamp-1); i++){
            int temp =i;
            if (temp < 0){
                temp = 0;
            }
            if (temp > 155){
                temp = 155;
            }
            s+=toChar(temp); //TODO deze div is temp en moet verandert worden, hetzelfde geld voor de andere divs.
            s+="+";
        }
        s[s.size()-1] = ')';
    } else if (r_time_stamp==1){
        int temp=lround(time_stamp/1000.0);
        if(temp>155){
            temp=155;
        }
        s=toChar(temp);
    } else {
        s="";
    }
    return s;
}

string Note::RoundDuration(int r_duration) const{ //TODO hier wordt nog geen rekening gehouden met range
    string s; //1
    if(r_duration>1){
        s="(";
        for(int i=duration-(r_duration-1);  i<=duration+(r_duration-1); i++){
            if(i<=duration+(r_duration-1)-1){
                s+=toChar(div(i,155).rem);
                s+="+";
            } else {
                s[s.size()-1] = ')';
            }
        }
    } else{
        s=toChar(div(duration,155).rem);
        //s = toChar(duration);
    }
    return s;
}

string Note::RoundVelocity(int r_velocity) const{ //TODO hier wordt nog geen rekening gehouden met range
    string s;
    if(r_velocity > 1){
        s="(";
        for(int i=velocity/3-(r_velocity-1);  i<=velocity/3+(r_velocity-1); i++){
            if(i<=velocity/3+(r_velocity-1)-1){
                int temp =i;
                if (temp < 0){
                    
                    temp = 0;
                }
                if (temp > 255/3){
                    temp = 255/3;
                }
                
                s+=toChar(temp);
                s+="+";
            } else {
                s[s.size()-1] = ')';
            }
        }
    } else if(r_velocity==1){
        s = toChar(velocity);
    } else {
        s="";
    }
    return s;
}

string Note::RoundInstrument(int r_instrument) const {
    if (r_instrument > 1){
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

            s += toChar(value);
            s += "+";
        }
    }

    s[s.size()-1] = ')';
    return s;
}

Note::Note(const Note &n) {
    this->time_stamp = n.time_stamp;
    this->duration = n.duration;
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

int Note::getDuration() const {
    return duration;
}

void Note::setDuration(int duration) {
    Note::duration = duration;
}
