//
// Created by watson on 4/28/23.
//

#include "Song.h"

struct Vectors_Params{
public:
    //timestamp, note on, instrument, note, velocity, pattern
    //vectors={{1,1,1,1,1,1},{0,1,0,1,0,1},{2,2,2,2,2,2},{0,1,0,1,0,2},{0,1,0,1,0,4}};

    //vector<vector<int>> vectors={{0,1,0,1,0,2}, {0,0,0,3,1,2}, {0,0,1,2,0,2}, {0,1,0,0,0,2}, {0,0,0,1,0,2}, {0,1,0,4,0,2}, {0,0,0,4,0,2}, {0,1,0,3,0,2}, {0,0,0,3,0,2}, {0,0,2,0,0,2},
    //                             {0,1,0,1,0,3}, {0,0,0,3,1,3}, {0,0,1,2,0,3}, {0,1,0,0,0,3}, {0,0,0,1,0,3}, {0,1,0,4,0,3}, {0,0,0,4,0,3}, {0,1,0,2,0,3}, {0,0,0,2,0,3}, {0,0,2,0,0,3}};
    //vector<vector<int>> vectors={{0,1,0,1,0,2}, {0,0,0,3,1,2}, {0,0,1,2,0,2}, {0,1,0,0,0,2}, {0,0,0,1,0,2}, {0,1,0,4,0,2}, {0,0,0,4,0,2}, {0,1,0,3,0,2}, {0,0,0,3,0,2}, {0,0,2,0,0,2},
    //                            {0,1,0,1,0,3}, {0,0,0,3,1,3}, {0,0,1,2,0,3}, {0,1,0,0,0,3}, {0,0,0,1,0,3}, {0,1,0,4,0,3}, {0,0,0,4,0,3}, {0,1,0,2,0,3}, {0,0,0,2,0,3}, {0,0,2,0,0,3},
    //                             {1,1,1,1,1,1},{0,1,0,1,0,1},{2,2,2,2,2,2},{0,1,0,1,0,2},{0,1,0,1,0,4},{0,1,0,1,5,1}};
    vector<vector<int>> vectors={{0,1,0,1,0,1},{8,1,0,2,0,1},{0,1,0,2,0,2},{1,1,1,1,1,1},{0,1,1,1,0,1},{0,2,0,2,0,4}}; // TODO een no
};
Vectors_Params PARAMS;

vector<DFA*> Song::convert(vector<RE> &s, bool complement, bool reverse) {
    vector<DFA*> tt;

    string log = getCurrTime() + " Converting Regex's to DFA's ("+ to_string(complement) + ") ("+ to_string(reverse) + ") ...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    for(auto z: s){
        ENFA k = z.toENFA();
        DFA* l = k.toDFA();

        if(complement){
            DFA* temp = l->complement();
            delete l;
            l = temp;
        }
        if(reverse){
            ENFA m = l->reverse();
            delete l;
            l = m.toDFA();
        }

        tt.push_back(l);
    }
    return tt;
}

[[nodiscard]] bool Song::ProperlyInitialized() const {
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

    //Find the correct title of the midi file without the path
    if (find(path.begin(), path.end(), '/') != path.end()){
        title = string(find(path.begin(), path.end(), '/')+1, path.end());
    }else{
        title = path;
    }

    string log = getCurrTime() + " Parsing note_map from .mid file...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    MidiParser m(path);
    note_map = m.getNoteMap();

    int count = 0;
    for(const auto &entry: note_map){
        count += (int) entry.second.size();
    }

    log = getCurrTime() + " Initialising new Song Object...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

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
    string log = getCurrTime() + " Destructor called, cleaning up the rubbish!\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        for(Note* &note: it->second){
            delete note;
        }
    }
}

[[nodiscard]] vector<RE> Song::toRegex(int time_stamp, int duration, int instrument, int note_b, int velocity, int pattern){ //When int>1 -> activate round
    REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");

    char epsilon='*';
    vector<RE> regex_list;
    int count = 0;
    string temp;

    string log = getCurrTime() + " Converting the Object to Regex's...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    for(auto it = note_map.begin(); it!=note_map.end() ; it++){
        if (it->first.second){
            for(Note* note: it->second){
                string z = note->getRE(time_stamp, duration, instrument, note_b, velocity);
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
        //Add the resting regex parts in case we didn't get a full pattern run
        RE regex(temp,epsilon);
        regex_list.push_back(regex);
    }

    return regex_list;
}

double Song::checkTibo(vector<DFA*> &d, vector<RE> &s){

    bool succeeded = false;
    int succes = 0;

    for(long unsigned int i = 0; i<min(d.size(), s.size()); i++){ // Given song
        string test=s[i].re;
        bool b = d[i]->accepts(test); //Addition Anas
        if(b){succes++;}
    }
    double result = (double) succes/((double)d.size());
    if(d.empty()){
        result=0;
    }
    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::checkKars(vector<DFA*> &d, vector<RE> &s){

    bool succeeded = false;
    double succes = 0; //Counter to keep the amount of time the test passes

    for(long unsigned int i = 0; i<d.size(); i++){ // Given song
        for(long unsigned int j = 0; j<s.size(); j++){
            string test=s[j].re;
            bool b = d[i]->accepts(test); //Addition Anas
            if(b){
                if(j!=s.size()-1&&i!=d.size()-1&&d[i+1]->accepts(s[j+1].re)){
                    succes++;
                    break;
                }
            }
        }
    }

    double result = succes / (double) d.size();
    if(d.empty() || s.empty()){
        result=0;
    }

    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::checkKarsAnas(vector<DFA*> &d, vector<RE> &s){

    bool succeeded = false;
    double succes = 0; //Counter to keep the amount of time the test passes

    for(const DFA* i: d){ // Given song
        for(const RE &j: s){
            bool b = i->accepts(j.re);

            if(b){
                succes++;
                break; //Idea Anas
            }
        }
    }

    double result = succes / (double) d.size() ;
    if(d.size()==0){
        result=0;
    }
    if(result>=0 && result<=1){succeeded = true;}

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}

double Song::similarity(Song &song, bool complement, bool reverse) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string m = getCurrTime()+" Applying " + '"' + "similarity (" + to_string(complement) + ") (" + to_string(reverse) + ")" +  '"' +" on Song: " + title + " and Song: " + song.getTitle() + "\n";
    if(console){cout << m;}
    logs.push_back( m );

    double result;
    double WNFA_result=1;
    bool succes = false;
    vector<vector<double>> results;

    //Do different checks on different Regex's
    for(const vector<int> &v: PARAMS.vectors){
        //No roundings
        pair<vector<RE>,vector<RE>> toCheck = {song.toRegex(v[0], v[1], v[2], v[3], v[4], v[5]), this->toRegex(min(v[0], 1), min(v[1], 1), min(v[2], 1), min(v[3], 1), min(v[4], 1), v[5]) }; //time_stamp,  note_on, instrument, note_b, velocity, pattern, rounder
        results.push_back( similar(toCheck,complement,reverse) ); // 0,1,0,1,0, 1,0
    }

    //Check Notes
    WNFA_result = checkWNFA(song.toRegex(0, 0, 0, 1, 0, -1)[0],this->toRegex(0, 0, 0, 1, 0, -1)[0]); //Set pattern to -1==1 long pattern
    cout << WNFA_result << endl; // TODO dit zou in de log moeten komen denk ik, redelijk handig
    result = (magimathical(results)+WNFA_result)/2; // TODO mischien parameter adden.
    
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

    m = getCurrTime()+" Comparison ended, showing a match-percentage off: "+ formatted + " %\n\n";
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
    int count=0;
    for(auto v=PARAMS.vectors.begin(); v!=PARAMS.vectors.end(); v++, count++){
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
        double boolparam = ((40.0/V0)*e + (17.0/V1)*d + (6.0/V2)*c + (17.0/V3)*b + (20.0/V4)*a+ pow(pow(2,3.0/4.0)* sqrt(5),(*v)[5]))/200; //TODO (int time_stamp, int note_on, int instrument, int note_b, int velocity, int pattern); als int 1 is de index = 0, als int 0 is wordt er geen rekening gehouden met de param
        result+=boolparam*(7*results[count][0]+2*results[count][1]+1*results[count][2])/(double)results[0].size();
    }
    result=result/((double)results.size());
    if (result>1){
        result=1;
    }
    return result;
}

vector<double> Song::similar(pair<vector<RE>, vector<RE>> &toCheck, bool complement, bool reverse) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    vector<double> results;
    vector<DFA*> d;

    pair<vector<RE>,vector<RE>> toCheck2 = sort(toCheck);
    d = convert(toCheck.first,complement,reverse);
    results.push_back( checkTibo(d , toCheck.second ) );

    //Check Kars
    results.push_back(checkKars(d, toCheck.second) );

    //Check KarsAnas
    results.push_back(checkKarsAnas(d, toCheck.second) );
    //Free used memory
    for(auto &k: d){
        delete k;
    }

    return results;
}

[[nodiscard]] map<int,unsigned int> Song::countNotes() {
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

[[nodiscard]] double Song::noteCountSimilarity(Song &s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string m = getCurrTime()+" Applying" + '"' + "noteCountSimilarity" + '"' +" on Song: " + title + " and Song: " + s.getTitle() + "\n";
    if(console){cout << m;}
    logs.push_back( m );

    map<int,unsigned int> count = this->countNotes();
    map<int,unsigned int> scount = s.countNotes();

    double succes = 0.0;
    unsigned int occurrences = count.size();
    bool succeed = false;

    for(auto k = count.begin(); k!=count.end(); k++){
        if( scount.find(k->first)!=scount.end()){ //Sharing a note
            if(scount[k->first]==k->second){
                succes+=1;
            }
        }
    }

    double result = succes/occurrences;
    if(result<=1 && result>=0){succeed = true;}
    ENSURE(succeed, "Percentage must be between 0 and 1");

    m = getCurrTime()+" Comparison ended, "+"showing a matchpercentage off: "+ to_string(result*100) + "%\n";
    if(console){cout << m;}
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

    for(const string &k: logs){
        out << k;
    }

    out.close();

    ENSURE(FileExists(file),"No log file has been created");
}

[[nodiscard]] const string &Song::getTitle() const {
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

    ENSURE(FileExists(path),"No file has been created");
}

void Song::switchConsoleOutput() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    console = !console;
}

Song::Song(DFA* s, vector<int> &param, bool console): console(console){ //param = {int r_time_stamp, int r_duration, int r_instrument, int r_note, int r_velocity, int octaaf}
    //Start up
    fInitCheck = this; // TODO soms wordt er een plus gehanteerd voor meerdere noten op een tijdstip, mischien moeten wij plus zo hanteren als da niet groter is dan een bepaalde waarde.
    title = s->getStartingState()->name;

    string log = getCurrTime() + " Creating Song Object from your perfect DFA...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //State Elimination
    log = getCurrTime() + " Eliminating States...\n\n";
    if(console){cout << log;}
    logs.push_back(log);
    const char epsilon='*';
    RE k(s->ToRe(), epsilon);

    //RE to Song
    log = getCurrTime() + " Started decoding...\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //For each element of the RE
    for(string::iterator it=k.re.begin(); it!=k.re.end(); it++){
        vector<unsigned int> note_values;
        if(*it=='+'){ //TODO kan niet de perfecte fix zijn
            continue;
        }
        if((*it)=='('){
            it++;
            while((*it)!='+'){
                note_values.push_back(toInt(*it));
                it++;
            }
            while((*it)!=')'){
                it++;
            }
            Note* n=new Note(note_values[0]*1000,tan(note_values[1]/155*1.6)/1.5*1000,note_values[3],note_values[4]*3,note_values[2]);
            note_map[{note_values[0]*1000,1}].push_back(n);
            Note* n2=new Note(note_values[0]*1000+tan((double) note_values[1]/155.0*1.6)/1.5*1000,tan((double) note_values[1]/155.0*1.6)/1.5*1000,note_values[3],note_values[4]*3,note_values[2]);
            note_map[{note_values[0]*1000+tan((double) note_values[1]/155.0*1.6)/1.5*1000,0}].push_back(n2);
            continue;
        } else{
            if((*it)=='*'){
                //cerr << "kleene star in regex" << endl;
                //throw std::exception();
                continue;
            }else {
                for(auto it2=param.begin(); it2!=param.end()-1; it2++){
                    if(*it2==true){ //Param used
                        note_values.push_back(toInt(*it)); //Re-convert the value
                        it++;
                    } else {
                        note_values.push_back(0); //Not used, add 0
                    }
                }
                it--;

                Note* n=new Note(note_values[0]*1000,tan((double) note_values[1]/155.0*1.6)/1.5*1000,note_values[3],note_values[4]*3,note_values[2]);
                note_map[{note_values[0]*1000,1}].push_back(n);
                Note* n2=new Note(note_values[0]*1000+tan((double) note_values[1]/155.0*1.6)/1.5*1000,tan((double) note_values[1]/155.0*1.6)/1.5*1000,note_values[3],note_values[4]*3,note_values[2]); //Make the note with the given data
                note_map[{note_values[0]*1000+tan((double) note_values[1]/155.0*1.6)/1.5*1000,0}].push_back(n2); //Add the note
            }
        }
    }

    log = getCurrTime() + " Construction Finished!\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
}

double Song::checkWNFA(RE &r,RE &s){
    string log = getCurrTime() + " Started using gold? (WNFA/WDFA)..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Convert to WDFA
    ENFA e = r.toENFA();
    json j = e.getJsonNfa();
    NFA n(j);

    WNFA w = n.toWNFA();
    WDFA m = w.toWDFA();

    //Calculate value
    double d = m.weightedaccepts(s.re);
    return d;
}

vector<double> Song::similar(vector<vector<DFA*>> &dfa1, vector<RE> &toCheck, bool complement, bool reverse) {
    vector<double> results;

    results.push_back( checkTibo2(dfa1 , toCheck));
    vector<DFA*> check_assemble;
    for(auto d: dfa1){
        for(auto c: d){
            check_assemble.push_back(c);
        }

    }

    //Check Kars
    results.push_back(min(checkKars(check_assemble, toCheck)*2, 1.0) );

    //Check KarsAnas
    results.push_back(min(checkKarsAnas(check_assemble, toCheck)* (double)check_assemble.size()/(double)dfa1.size(), 1.0) );

    return results;
}

double Song::checkTibo2(vector<vector<DFA *>> &d, vector<RE> &s) {
    bool succeeded = false;
    int succes = 0;

    for(long unsigned int i = 0; i<min(d.size(), s.size()); i++){ // Given song
        string test=s[i].re;
        bool b = false;
        for (int j=0; j < d[i].size(); j++){
            if (d[i][j]->accepts(test)){
                b = true;
            }
        }

        if(b){succes++;}
    }
    double result = (double) succes/((double)min(d.size(), s.size()));
    if(d.empty()){
        result=0;
    }
    if(result>=0 && result<=1){succeeded = true;} //Result bust me a percentage

    ENSURE(succeeded, "Operation did not work properly");
    return result;
}
