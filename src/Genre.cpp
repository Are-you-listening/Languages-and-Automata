//
// Created by watson on 4/28/23.
//

#include "Genre.h"

void Genre::addGenre(const Song *&s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    string log;
    if(inGenre(s)){
        members.push_back(s);
        log = getCurrTime()+ " Succesfully added " + s->getTitle() + " to the Genre!\n\n";
    }else{
        log = getCurrTime()+ " Could not add this Song the Genre.\n\n";
    }
    if(console){cout << log;}
    logs.push_back(log);
}

DFA Genre::toProductAutomata() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    vector<DFA> temp;

    string log = getCurrTime()+ " Converting to ProductAutomata..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Loop over each Song
    for(const Song* s: members) {
        vector<RE> t = s->toRegex(param[0], param[0], param[0], param[0], param[0],
                                  -1); //Set pattern to -1 so we can generate 1 big Regex
        ENFA a = t[0].toENFA();
        DFA m = a.toDFA();
        m = m.minimize();
        if (temp.size() == 0) {
            temp.push_back(m);
        } else {
            temp[0] = DFA(temp[0], m, 0); //Make or extend ProductAutomata
        }
    }
    return temp[0];
}

bool Genre::inGenre(const Song *&s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime()+ " Validating if "+s->getTitle()+" is part of this genre..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    DFA m = toProductAutomata();
    vector<RE> st = s->toRegex(param[0],param[0],param[0],param[0],param[0],param[0]);

    bool succes = m.accepts(st[0].re);
    if(succes){
        log = getCurrTime() + " Found matching results on the characteristics on this Genre!\n\n";
    }else{
        log = getCurrTime() + " This song did not match the requirements of this Genre!\n\n";
    }
    if(console){cout << log;}
    logs.push_back(log);

    return succes;
}

Genre::Genre(Song *&s, Song *&k, const vector<int> &params) {
    param = params;

    pair<vector<RE> , vector<RE>> toCheck = {s->toRegex(param[0],param[0],param[0],param[0],param[0],param[0]) , k->toRegex(param[0],param[0],param[0],param[0],param[0],param[0])};
    vector<double> temp = s->similar( toCheck , 0 ,0); //Run Similarity Check

    double result = 0;
    for(double &m: temp){
        result+=m;
    }

    //Set Data
    limit = result/temp.size();
    members={s,k};
    name = s->getTitle() + "_and_"+k->getTitle()+"_Genre";
    fInitCheck = this;
    string log = getCurrTime() + " Created the new Genre: "+name+" , constructed on a "+to_string(limit)+" match %.\n\n";
    if(console){cout << log;}
    logs.push_back(log);
    ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
}

Genre::Genre(const vector<const Song *> &members, double limit, const vector<int> &param, string &name) : members(members),
                                                                                            limit(limit),
                                                                                            param(param), name(name) {
    fInitCheck=this;
    string log = getCurrTime() + " Loaded the new Genre: "+name+" , based on a "+to_string(limit)+" match %.\n\n";
    if(console){cout << log;}
    logs.push_back(log);
    ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
}

void Genre::output() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string file= "report_"+name+".txt";
    string file2 = "report_"+name;
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
}

bool Genre::ProperlyInitialized() const {
    if(fInitCheck== this){
        return true;
    }
    return false;
}

const string &Genre::getName() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    return name;
}

void Genre::setName(const string &name) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    Genre::name = name;
    ENSURE(Genre::name == name , "Setter didn't work properly");
}

void Genre::switchConsole() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    if(console){
        console = false;
    }else{
        console = true;
    }
}
