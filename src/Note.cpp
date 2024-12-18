//
// Created by tibov on 29/04/23.
//

#include "Note.h"

Note::Note(unsigned int time_stamp, int duration, int note,  int velocity, unsigned int instrument): instrument(instrument), note_value(note), time_stamp(time_stamp),
                                                                                                     velocity(velocity), duration(duration)    {
    //Note::duration = -1;
}

[[nodiscard]] string Note::getRE(int r_time_stamp, int r_duration, int r_instrument, int r_note, int r_velocity) const{
    string s = "";
    s += RoundTime_stamp(r_time_stamp);
    s += RoundDuration(r_duration);
    s += RoundInstrument(r_instrument);
    s += RoundNote(r_note%12,1+r_note/12);
    s += RoundVelocity(r_velocity);
    return s;
}

int Note::getNoteValue() const {
    return note_value;
}

string Note::RoundTime_stamp(int r_time_stamp) const{
    string s;
    if(r_time_stamp>1){
        s="(";
        for(int i=::lround(time_stamp/time_split)-(r_time_stamp-1);  i!=::lround(time_stamp/time_split)+(r_time_stamp-1); i++){
            int temp =i;
            if (temp < 0){
                temp = 0;
            }
            if (temp > 155){
                temp = 155;
            }
            s+=toChar(temp);
            s+="+";
        }
        s[s.size()-1] = ')';
    } else if (r_time_stamp==1){
        int temp=lround(time_stamp/time_split);
        if(temp>155){
            temp=155;
        }
        s=toChar(temp);
    } else {
        s="";
    }
    return s;
}

string Note::RoundDuration(int r_duration) const{
    string s; //1
    if(r_duration>1){
        s="(";
        double temp_duration = lround((atan(1.5*(duration/1000.0))/1.6)*155);
        for(int i=temp_duration-(r_duration-1);  i<=temp_duration+(r_duration-1); i++){
            int v = i;
            if (v < 0){
                v = 0;
            }else if (v > 155){
                v = 155;
            }
            s+=toChar(v);
            s+="+";
        }
        s[s.size()-1] = ')';
    } else if (r_duration==1){
        s=toChar( lround((atan(1.5*(double) duration/1000.0)/1.6)*155));
    }else{
        s = "";
    }
    return s;
}

string Note::RoundVelocity(int r_velocity) const{
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
        s = toChar(velocity/3);
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
    }else if (r_instrument == 1){
        string s = "";
        s += toChar(instrument);
        return s;
    }else{
        return "";
    }
}

string Note::RoundNote(int r_note, int r_octaaf) const {

    if (r_note == 0 && r_octaaf == 0){
        return "";
    }

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
            if (value > 155){
                value = 155;
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

void Note::setDuration(int Duration) {
    Note::duration = Duration;
}