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

    int percentage = 0;

    bool succes = false;

    if(percentage<=100 && percentage>=0){
        succes = true;
    }
    //IDEA KARS
    auto t = song.toRegex(0,1,1,1,0,2,1); // als je een hogere match precentage wilt dat nog steeds accuraat is, maak instrument 0 en velocity 0 song.toRegex(0,1,0,1,0,1, 1);
    auto t2 = this->toRegex(0,1,1,1,0,2,0);

    vector<DFA> tt;
    //vector<DFA> tt2;
    for(auto z: t){
        ENFA k = z.toENFA();
        DFA s = k.toDFA();
        tt.push_back(s);
    }

    /*
    for(int i=0; i<t.size(); i++){ // dit zorgt nog voor een error
        auto a1 = t[i];
        auto a2 = t2[i];

    }
     */

    /*
    for(auto z: t2){
        ENFA k = z.toENFA();
        DFA s = k.toDFA();
        tt2.push_back(s);
    }
    double slagen=0.;
    int count=0;
    for(auto s: tt){
        for(auto k: tt2){
            if(s==k){
                slagen++;
                break;//Percentage
            }
            count++;
        }
    }
    double resultaat = slagen/count;
    cout << "kars: " << resultaat << " slagen: " << slagen << " coutns: " << count  <<endl;
    */

    double slagen=0.0;
    int count=0;
    bool b;
    //IDEA TIBO en Anas
    for(vector<DFA>::const_iterator s=tt.begin(); s!=tt.end(); s++){ // Given song
        for(vector<RE>::const_iterator s2=t2.begin(); s2!=t2.end(); s2++){ //this.Song
            string test=s2->re;
            b = s->accepts(test);
            if(b){
                slagen++;
                //cout << test << endl;
                break;
            }
        }
        count++;
    }
    double resultaat=slagen/count;
    cout << "Tibo & Anas: " << resultaat << " slagen: " << slagen << " coutns: " << count  <<endl;

    ENSURE(succes, "Percentage must be between 0 and 100");
    return resultaat;
}

void Song::parse(const string &path) {
    REQUIRE(FileExists(path) , "Given file not found");

    fInitCheck=this;

    MidiParser m(path);
    note_map = m.getNoteMap();
    int count = 0;
    for(auto entry: note_map){
        count += entry.second.size();
    }
    cout << count << endl;
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

vector<RE> Song::toRegex(bool time_stamp, bool note_on, bool instrument, bool note_b, bool velocity, int pattern, bool rounder) const {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    char epsilon='*';
    vector<RE> regex_list;
    int count = 0;
    string temp = "";

    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        for(Note* note: it->second){
            string z = note->getRE(time_stamp, note_on, instrument, note_b, velocity, rounder);
            temp+=z;
            count++;
            if(count==pattern){
                //DO ACTUAL MERGE/MAKE REGEX

                RE regex(temp,epsilon);
                regex_list.push_back(regex);
                count=0;
                temp.clear();
                temp = "";
            }
        }

    }

    if (count != 0){
        //Add the resting regex parts incase we didn't got a full pattern run
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

Song::Song(const map<pair<unsigned int, bool>, vector<Note *>> &noteMap) : note_map(noteMap) {
    fInitCheck = this;
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::Song() {
    fInitCheck = this;
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::Song(const string &path) {
    REQUIRE(FileExists(path) , "Given file not found");
    fInitCheck = this;
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song &Song::operator=(const Song &a) {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    map<pair<unsigned int, bool>, vector<Note*>> map2;

    for(auto it = note_map.begin(); it!=note_map.end(); it++){
        vector<Note*> temp;
        for(Note* &n: it->second){ //Construct new Note objects on heap
            temp.push_back( new Note(*n) );
        }
        map2[it->first]=temp;
    }

    Song s = Song(map2); //Make actual new object
    ENSURE(s.ProperlyInitialized(), "Constructor must end in properly initialised state!");
    return s;
}

