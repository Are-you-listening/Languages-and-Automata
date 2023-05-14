//
// Created by watson on 4/28/23.
//

#include "Song.h"

struct Vectors_Params{
public:
    //timestamp, note on, instrument, note, velocity, pattern
    //vector<vector<int>> vectors={{1,1,1,1,0,1}}; //TODO dit zullen er meer worden, maar voor nu debuggen te vergemakkelijken zijn er het 1
    //vectors={{1,1,1,1,1,1},{0,1,0,1,0,1},{2,2,2,2,2,2},{0,1,0,1,0,2},{0,1,0,1,0,4}};

    //vector<vector<int>> vectors={{0,1,0,1,0,2}, {0,0,0,3,1,2}, {0,0,1,2,0,2}, {0,1,0,0,0,2}, {0,0,0,1,0,2}, {0,1,0,4,0,2}, {0,0,0,4,0,2}, {0,1,0,3,0,2}, {0,0,0,3,0,2}, {0,0,2,0,0,2},
    //                             {0,1,0,1,0,3}, {0,0,0,3,1,3}, {0,0,1,2,0,3}, {0,1,0,0,0,3}, {0,0,0,1,0,3}, {0,1,0,4,0,3}, {0,0,0,4,0,3}, {0,1,0,2,0,3}, {0,0,0,2,0,3}, {0,0,2,0,0,3}};
    //vector<vector<int>> vectors={{0,1,0,1,0,2}, {0,0,0,3,1,2}, {0,0,1,2,0,2}, {0,1,0,0,0,2}, {0,0,0,1,0,2}, {0,1,0,4,0,2}, {0,0,0,4,0,2}, {0,1,0,3,0,2}, {0,0,0,3,0,2}, {0,0,2,0,0,2},
    //                            {0,1,0,1,0,3}, {0,0,0,3,1,3}, {0,0,1,2,0,3}, {0,1,0,0,0,3}, {0,0,0,1,0,3}, {0,1,0,4,0,3}, {0,0,0,4,0,3}, {0,1,0,2,0,3}, {0,0,0,2,0,3}, {0,0,2,0,0,3},
    //                             {1,1,1,1,1,1},{0,1,0,1,0,1},{2,2,2,2,2,2},{0,1,0,1,0,2},{0,1,0,1,0,4},{0,1,0,1,5,1}};
    vector<vector<int>> vectors={{0,1,0,1,0,1}};
};
Vectors_Params PARAMS;

vector<DFA> Song::convert(vector<RE> &s, bool complement, bool reverse) {
    vector<DFA> tt;

    string log = getCurrTime() + " Converting Regex's to DFA's ("+ to_string(complement) + ") ("+ to_string(reverse) + ") ...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

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

        //l.minimize();
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

Song::Song(const map<pair<unsigned int, bool>, vector<Note *>> &noteMap, bool console) : console(console), note_map(noteMap) {
    fInitCheck = this;
    title = "Are you listening?";

    string log = getCurrTime() + " Initialising new Song Object...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::Song() {
    console = false;
    fInitCheck = this;
    title = "Are you listening?";

    string log = getCurrTime() + " Creating empty Song Object...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

Song::Song(const string &path, bool console) : console(console) {
    REQUIRE(FileExists(path) , "Given file not found");

    fInitCheck=this;
    title = "Are you listening?";

    string log = getCurrTime() + " Parsing note_map from .mid file...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    MidiParser m(path);
    note_map = m.getNoteMap();

    //TODO This is not used??
    int count = 0;
    for(const auto &entry: note_map){
        count += entry.second.size();
    }

    log = getCurrTime() + " Initialising new Song Object...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //checkWNFA(toRegex(0, 0, 0, 1, 0, -1)[0]);


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

    console = a.console;
    title = a.title;
    logs = a.logs;
    note_map = map2;
    fInitCheck = this; //Make actual new object
    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
    return *this;
}

Song::~Song(){
    string log = getCurrTime() + " Destructor called, cleaning up the rubish!\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        for(Note* note: it->second){
            delete note;
        }
    }
}

vector<RE> Song::toRegex(int time_stamp, int note_on, int instrument, int note_b, int velocity, int pattern){
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    char epsilon='*';
    vector<RE> regex_list;
    int count = 0;
    string temp = "";

    string log = getCurrTime() + " Converting the Object to Regex's...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        if (it->first.second){
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
                    temp = "";
                }
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
    if(d.size()>s.size()){ // TODO dit moet aangepast worden.
        return 0;
    }
    
    REQUIRE(d.size()<=s.size(), "Indices should be always valid");

    bool succeeded = false;
    int succes = 0;

    for(long unsigned int i = 0; i<d.size(); i++){ // Given song
        string test=s[i].re;
        bool b = d[i].accepts(test); //Addition Anas
        if(b){succes++;}
    };

    double result = (double) succes/d.size();
    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::checkKars(vector<DFA> &d, vector<RE> &s) const {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    bool succeeded = false;
    double succes = 0; //Counter to keep the amount of time the test passes

    for(long unsigned int i = 0; i<d.size(); i++){ // Given song
        for(long unsigned int j = 0; j<s.size(); j++){
            string test=s[j].re;
            bool b = d[i].accepts(test); //Addition Anas
            if(b){succes++;}
        }
    }

    double result = succes / (double) (d.size() * s.size()) ;
    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::checkKarsAnas(vector<DFA> &d, vector<RE> &s) const {
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    bool succeeded = false;
    double succes = 0; //Counter to keep the amount of time the test passes
    double count = 0; //Counter to keep Nr of Operations

    for(long unsigned int i = 0; i<d.size(); i++){ // Given song
        for(long unsigned int j = 0; j<s.size(); j++){
            string test=s[j].re;
            bool b = d[i].accepts(test); //Addition Anas

            if(b){
                succes++;
                break; //Idea Anas
            }
        }
        count ++;
    }

    double result = succes / count ;
    if(result>=0 && result<=1){succeeded = true;}

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::similarity(Song &song, bool complement, bool reverse) { // TODO complement en reverse zouden technisch hier niet mogen staan, dit zullen we moeten bespreken.
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    
    string m = getCurrTime()+" Applying " + '"' + "similarity (" + to_string(complement) + ") (" + to_string(reverse) + ")" +  '"' +" on Song: " + title + " and Song: " + song.getTitle() + "\n";
    if(console){cout << m;}
    logs.push_back( m );

    double result;
    double WNFAresult; // TODO WNFA result moet nog geimplemeteerd worden
    bool succes = false;
    vector<vector<double>> results;
    vector<vector<double>> WNFAresults;
    
    //Do different checks on different Regex's
    for(const vector<int> &v: PARAMS.vectors){ // TODO this->toRegex(min(v[0], 1), min(v[1], 1), min(v[2], 1), min(v[3], 1), min(v[4], 1), v[5]) ik weet eingelijk niet of dit volledig juist is
        //No roundings
        pair<vector<RE>,vector<RE>> toCheck = {song.toRegex(v[0], v[1], v[2], v[3], v[4], v[5]), this->toRegex(min(v[0], 1), min(v[1], 1), min(v[2], 1), min(v[3], 1), min(v[4], 1), v[5]) }; //time_stamp,  note_on, instrument, note_b, velocity, pattern, rounder
        results.push_back( similar(toCheck,complement,reverse) ); // 0,1,0,1,0, 1,0
    }
    
    result = magimathical(results);
    if(result<=1 && result>=0){succes = true;}
    ENSURE(succes, "Percentage must be between 0 and 1");

    string formatted;
    result*=100;
    if (result<25){
        formatted = "\033[1;31m" + to_string(result) + "\033[0m";
    } else if (result<50){
        formatted = "\033[1;33m" + to_string(result) + "\033[0m";
    } else if (result<90){
        formatted = "\033[1;32m" + to_string(result) + "\033[0m";
    } else {
        formatted = "\033[1;42m\033[1;1m\033[1;35m" + to_string(result) + "\033[0m" ;
    }

    m = getCurrTime()+" Comparition ended, showing a matchpercentage off: "+ formatted + " %\n\n";
    if(console){cout << m;}
    logs.push_back(m);

    return result;
}

bool Song::operator==(Song &rhs) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime() + " Checking if the songs are the same...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    bool succes;
    double percentage = similarity(rhs, false, false);
    if(percentage>=1){
        succes = true;
        log = getCurrTime() + " " + getTitle() + " is the same as: " + rhs.getTitle() + "\n\n";
    }else{
        succes = false;
        log = getCurrTime() + " " + getTitle() + " is differing from: " + rhs.getTitle() + "\n\n";
    }

    if(console){cout << log;}
    logs.push_back(log);

    return succes;
}

bool Song::operator!=(Song &rhs) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return !((Song) rhs == *this);
}

double Song::magimathical(vector<vector<double>> &results) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    double result = 0;

    string log = getCurrTime() + " Applying Magic...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Anas Working Space
    for(vector<vector<int>>::iterator v=PARAMS.vectors.begin(); v!=PARAMS.vectors.end(); v++){
        int a=1;
        int b=1;
        int c=1;
        int d=1;
        int e=1;
        int V0=(*v)[0];
        int V1=(*v)[1];
        int V2=(*v)[2];
        int V3=(*v)[3];
        int V4=(*v)[4];
        if((*v)[4]==0){
            a=0;
            V4=1;
        }
        if((*v)[3]==0){
            b=0;
            V3=1;
        }
        if((*v)[2]==0){
            c=0;
            V2=1;
        }
        if((*v)[1]==0){
            d=0;
            V1=1;
        }
        if((*v)[0]==0){
            e=0;
            V0=1;
        }
        double boolparam = (40.0/V0)*e + (17.0/V1)*d + (6.0/V2)*c + (17.0/V3)*b + (20.0/V4)*a + pow(5,(*v)[5]); //TODO (int time_stamp, int note_on, int instrument, int note_b, int velocity, int pattern); als int 1 is de index = 0, als int 0 is wordt er geen rekening gehouden met de param
        for(vector<vector<double>>::iterator it=results.begin(); it!=results.end(); it++){
            result+=(boolparam*(0.7*(*it)[0]+0.2*(*it)[1]+0.1*(*it)[2]))/results[0].size();
        }
    }
    result=result/results.size();
    if (result>1){
        result=1;
    }
    return result;
}

vector<double> Song::similar(pair<vector<RE>, vector<RE>> &toCheck, bool complement, bool reverse) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    vector<double> results;
    vector<DFA> d;

    pair<vector<RE>,vector<RE>> toCheck2 = sort(toCheck);
    d = convert(toCheck.first,complement,reverse);
    //cout << toCheck2.first.size() << " " << toCheck2.second.size() << endl;
    results.push_back( checkTibo(d , toCheck.second ) );

    //Check Kars
    d = convert(toCheck.first,complement,reverse);
    results.push_back(checkKars(d, toCheck.second) );

    //Check KarsAnas
    results.push_back(checkKarsAnas(d, toCheck.second) );
    
    return results;
}

map<int,unsigned int> Song::countNotes() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    map<int,unsigned int> counts; // map[Note]=occurrences

    string log = getCurrTime() + " Counting Notes...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

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

[[nodiscard]] double Song::noteCountSimilarity(Song &s) { //TODO dit moet nog in similarity gestoken worden
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string m = getCurrTime()+" Applying" + '"' + "noteCountSimilarity" + '"' +" on Song: " + title + " and Song: " + s.getTitle() + "\n";
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

    m = getCurrTime()+" Comparition ended, "+"showing a matchpercentage off: "+ to_string(result*100) + "%\n";
    logs.push_back(m);

    return result;
}

void Song::output() const {
    REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string file= "report_"+title+".txt";
    string file2 = "report_"+title;
    while(FileExists(file)){
        file2+="(copy)";
        file = file2+".txt";
    }

    ofstream out(file);

    out << "    -==[(*)]==-   ";
    out << "Report of actions";
    out << "    -==[(*)]==-   \n\n";

    out << "Date of report: "+getCurrTime()+"\n\n";

    for(string k: logs){
        out << k;
    }

    out.close();

    ENSURE(FileExists(file),"No log file has been created");
}

const string &Song::getTitle() const {
    REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return title;
}

void Song::setTitle(const string &name) {
    REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    title = name;
    ENSURE(title == name , "Setter didn't work properly");
}

void Song::save(const string &path) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime() + " Saving Song into .mid file...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    midiExporter exp(path, note_map);
    
    //ENSURE FILEXISTS
}

void Song::switchConsoleOutput() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    console = !console;
}

Song::Song(DFA &s, vector<int> &param, bool console): console(console){ //param = {int r_time_stamp, int r_duration, int r_instrument, int r_note, int r_velocity, int octaaf}
    long unsigned int index = 0; //Index ptr van param
    stack<char> tempstack; //Helper variable
    vector<vector<int>> info; //{ {possible time_stamps}, {possible durations}, {possible notes},  {possible velocity's}, {possible instruments} }
    vector<int> options = {}; //Posssible values

    //Start up
    fInitCheck = this; // TODO soms wordt er een plus gehanteerd voor meerdere noten op een tijdstip, mischien moeten wij plus zo hanteren als da niet groter is dan een bepaalde waarde.
    title = s.getStartingState()->name;

    string log = getCurrTime() + " Creating Song Object from your perfect DFA...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //State Elimination
    log = getCurrTime() + " Eliminating States...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    const char epsilon='*';
    RE k(s.ToRe(), epsilon);

    //RE to Song
    log = getCurrTime() + " Started decoding...\n\n";
    if(console){cout << log;}
    logs.push_back(log);
    //For each element of the RE
    for(const char &m: k.re) {
        if (index == param.size()) { //Reached all params, ready to make objects
            //Make notes
            vector<vector<int>> notes = makeNotes(info);

            for(vector<int> &f: notes){ //Create the actual objects
                Note* e = new Note(f[0],f[1],f[2],f[3],f[4]);
                note_map[{f[1],f[0]}].push_back(e);
            }

            index = 0; //Reset Index
        }

        if (param[index] == 0) { //If param not used
            index++;
            info.push_back({0}); //Add Nothing
        }else { //Param is used
            if(m=='('){ //Begin of Regex
                tempstack.push(m);
                continue;
            }else if(m==')') { //Ended 1 character
                index++;
                info.push_back(options); //Push back options
                options.clear(); //Reset options
                tempstack.pop();
            }else if(m=='*'){
                cerr << "Honeseltely no clue what to do with Kleene Star, which lenght should I take?" << endl;
            }else{
                options.push_back(toChar(m));
                if(tempstack.empty()){
                    index++;
                }
            }
        }
    }

    log = getCurrTime() + " Construction Finished!\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

double Song::checkWNFA(RE &r){
    /**
     * format indexes:
     * 0: accepted note line
     * 1: self loop start
     * 2: self loop end
     * 3: self loop erverywhere else
     * 4: arrow to the next state
     * */

    RE wr = toRegex(0, 0, 0, 1, 0, -1)[0];
    ENFA e = wr.toENFA();
    json j = e.getJsonNfa();
    NFA n(j);
    WNFA w = n.toWNFA();
    //cout << w.weightedaccepts(wr.re.substr(0, 3)) << endl;
    //double v = w.weightedaccepts(r.re);
    //cout << v << endl;

    return 0;
};
