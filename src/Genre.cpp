//
// Created by watson on 4/28/23.
//

#include "Genre.h"

void Genre::addGenre(Song *&s) {
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

    string log = getCurrTime()+ " Converting to ProductAutomata..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Loop over each Song
    for(int i = ProductAutomata.first; i<members.size(); i++) {
        vector<RE> t = members[i]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1 so we can generate 1 big Regex
        ENFA a = t[0].toENFA();
        DFA s = a.toDFA();
        //s = s.minimize();
        ProductAutomata.second = DFA(ProductAutomata.second, s, 0); //Extend ProductAutomata
    }
    ProductAutomata.first = members.size();
    return ProductAutomata.second;
}

bool Genre::inGenre(Song *&s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime()+ " Validating if "+s->getTitle()+" is part of this genre..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    DFA m = toProductAutomata();
    vector<RE> st = s->toRegex(param[0],param[1],param[2],param[3],param[4],-1);

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

Genre::Genre(Song *&s, Song *&k, const vector<int> &params, const string &name) {
    param = params;

    //Set Data
    members={s,k};
    ProductAutomata = {0, DFA() };
    this->name = name;
    fInitCheck = this;

    ProductAutomata = {2,toProductAutomata()}; //Construct First ProductAutomata

    string log = getCurrTime() + " Created the new Genre: "+name+" , based on "+ s->getTitle() + " and " + k->getTitle() +"\n\n";
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

void Genre::switchConsoleOutput() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    if(console){
        console = false;
    }else{
        console = true;
    }
}

DFA Genre::getProductAutomata() const {
    return ProductAutomata.second;
}
