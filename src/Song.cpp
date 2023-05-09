//
// Created by watson on 4/28/23.
//

#include "Song.h"

vector<DFA> Song::convert(vector<RE> &s, bool complement, bool reverse) const {
    vector<DFA> tt;

    for(auto z: s){
        ENFA k = z.toENFA();
        DFA l = k.toDFA();

        if(complement){
            l = l.complement();
        }

        if(reverse){
            ENFA m = l.reverse();
            l = m.toDFA();
        }

        l.minimize();
        tt.push_back(l);
    }
    return tt;
}

pair<vector<RE>, vector<RE>> Song::sort(const pair<vector<RE>, vector<RE>> &t) const {
    pair<vector<RE>, vector<RE>> k = t;
    if(t.first.size()>t.second.size()){
        k = {t.second, t.first};
    }
    return k;
}

bool Song::ProperlyInitialized() const {
    if(fInitCheck==this){
        return true;
    }
    return false;
}

Song::Song(const map<pair<unsigned int, bool>, vector<Note *>> &noteMap) : note_map(noteMap) {
    fInitCheck = this;
    title = "Are you listening?";
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::Song() {
    fInitCheck = this;
    title = "Are you listening?";
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::Song(const string &path) {
    REQUIRE(FileExists(path) , "Given file not found");

    fInitCheck=this;
    title = path;

    MidiParser m(path);
    note_map = m.getNoteMap();
    int count = 0;
    for(const auto &entry: note_map){
        count += entry.second.size();
    }

    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song &Song::operator=(const Song &a) {
    REQUIRE(a.ProperlyInitialized(), "Constructor must end in properly initialised state!");

    map<pair<unsigned int, bool>, vector<Note*>> map2;

    for(auto it = a.note_map.begin(); it!=a.note_map.end(); it++){
        vector<Note*> temp;
        for(const Note* n: it->second){ //Construct new Note objects on heap
            Note* k = new Note(*n);
            temp.push_back( k );
        }
        map2[it->first]=temp;
    }

    title = a.title;
    logs = a.logs;
    note_map = map2;
    fInitCheck = this; //Make actual new object
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::~Song(){
    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        for(Note* note: it->second){
            delete note;
        }
    }
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
        //Add the resting regex parts in case we didn't got a full pattern run
        RE regex(temp,epsilon);
        regex_list.push_back(regex);
    }

    return regex_list;
}

double Song::checkTibo(vector<DFA> &d, vector<RE> &s) const {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
    REQUIRE(d.size()<=s.size(), "Indices should be always valid");

    bool succeeded = false;
    int succes = 0;

    for(int i = 0; i<d.size(); i++){ // Given song
        string test=s[i].re;
        bool b = d[i].accepts(test); //Addition Anas
        if(b){succes++;}
    };

    double result = succes/d.size();
    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::checkKars(vector<DFA> &d, vector<RE> &s) const {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    bool succeeded = false;
    int succes = 0; //Counter to keep the amount of time the test passes

    for(int i = 0; i<d.size(); i++){ // Given song
        for(int j = 0; j<s.size(); j++){
            string test=s[j].re;
            bool b = d[i].accepts(test); //Addition Anas
            if(b){succes++;}
        }
    }

    double result = succes / (d.size() * s.size()) ;
    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::checkKarsAnas(vector<DFA> &d, vector<RE> &s) const {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    bool succeeded = false;
    int succes = 0; //Counter to keep the amount of time the test passes
    int count = 0; //Counter to keep Nr of Operations

    for(int i = 0; i<d.size(); i++){ // Given song
        for(int j = 0; j<s.size(); j++){
            string test=s[j].re;
            bool b = d[i].accepts(test); //Addition Anas

            if(b){
                succes++;
                count++;
                break; //Idea Anas
            }
        }
    }

    double result = succes / count ;
    if(result>=0 && result<=1){succeeded = true;}

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::similarity(const Song &song, bool complement, bool reverse) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    double result;
    bool succes = false;
    vector<vector<double>> results;
    
    vector<vector<bool>> vectors;
    vectors.push_back({0,1,0,1,0,1,1});
    
    //Do different checks on different Regex's
    for(const vector<bool> &v: vectors){
        //No roundings
        pair<vector<RE>,vector<RE>> toCheck = {song.toRegex(v[0], v[1], v[2], v[3], v[4], v[5], v[6]), this->toRegex(v[0], v[1], v[2], v[3], v[4], v[5],
                                                                                                                     false) }; //time_stamp,  note_on, instrument, note_b, velocity, pattern, rounder
        results.push_back( similar(toCheck,complement,reverse) ); // 0,1,0,1,0, 1,0
    }
    
    result = magimathical(results);
    if(result<=1 && result>=0){succes = true;}
    ENSURE(succes, "Percentage must be between 0 and 1");
    return result;
}

bool Song::operator==(const Song &rhs) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    double percentage = similarity(rhs, false, false);
    if(percentage>=1){
        return true;
    }
    return false;
}

bool Song::operator!=(const Song &rhs) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return !((Song) rhs == *this);
}

double Song::magimathical(vector<vector<double>> &results) const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    double result = 0;
    
    //Anas Working Space
    
    return result;
}

vector<double> Song::similar(pair<vector<RE>, vector<RE>> &toCheck, bool complement, bool reverse) const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    vector<double> results;
    vector<DFA> d;

    pair<vector<RE>,vector<RE>> toCheck2 = sort(toCheck);
    d = convert(toCheck2 .first,complement,reverse);
    cout << d.size() << " " << toCheck2.second.size() << endl;
    results.push_back( checkTibo(d , toCheck2 .second ) );

    //Check Kars
    d = convert(toCheck.first,complement,reverse);
    results.push_back(checkKars(d, toCheck.second) );

    //Check KarsAnas
    results.push_back(checkKarsAnas(d, toCheck.second) );
    
    return results;
}

map<int,unsigned int> Song::countNotes() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    map<int,unsigned int> counts; // map[Note]=occurrences

    for(auto it = note_map.begin(); it!=note_map.end(); it++){
        for(const Note* n: it->second){
            if(counts.find(n->getNoteValue())==counts.end()){ //In case not found
                counts[n->getNoteValue()]=1;
            }else{
                counts[n->getNoteValue()]++;
            }
        }
    }
    return counts;
}

[[nodiscard]] double Song::noteCountSimilarity(const Song &s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string m = getCurrTime()+" Applying" + '"' + "noteCountSimilarity" + "'" +" on " + title + "and " + s.getTitle() + "\n";
    logs.push_back( m );

    map<int,unsigned int> count = this->countNotes();
    map<int,unsigned int> scount = s.countNotes();

    unsigned int succes = 0;
    unsigned int occurences = count.size();
    bool succeed = false;

    for(auto k = count.begin(); k!=count.end(); k++){
        if( scount.find(k->first)!=scount.end()){ //Sharing a note
            if(scount[k->first]==k->second){
                succes++;
            }
        }
    }

    double result = succes/occurences;
    if(result<=1 && result>=0){succeed = true;}
    ENSURE(succeed, "Percentage must be between 0 and 1");

    m = getCurrTime()+" Compared "+"showing a matchpercentage off: "+ to_string(result) + "%\n";
    logs.push_back(m);

    return result;
}

void Song::Export() const {
    REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    ofstream out(title+"_report.txt");

    out << "    -==[(*)]==-   ";
    out << "Report of actions";
    out << "    -==[(*)]==-   "+'\n';

    out << "Date of report: "+getCurrTime()+'\n';


    ENSURE(FileExists(title+"_report.txt"),"No log file has been created");
}

const string &Song::getTitle() const {
    REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return title;
}
