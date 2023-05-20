//
// Created by watson on 4/28/23.
//

#include "Genre.h"

void Genre::addGenre(Song *&s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    string log;
    if(inGenre(s)){
        members.push_back(s);
        log = getCurrTime()+ " Successfully added " + s->getTitle() + " to the Genre!\n\n";
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
    for(long unsigned int i = ProductAutomata.first; i<members.size(); i++) {
        vector<RE> t = members[i]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1, so we can generate 1 big Regex
        ENFA a = t[0].toENFA();
        DFA s = a.toDFA();
        ProductAutomata.second = DFA(ProductAutomata.second, s, false); //Extend ProductAutomata
    }
    ProductAutomata.first = (int) members.size();

    if(TFA){
        ProductAutomata.second = ProductAutomata.second.minimize(); //So we will use less space
    }

    return ProductAutomata.second;
}

bool Genre::inGenre(Song *&s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime()+ " Validating if "+s->getTitle()+" is part of this genre..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    bool succes = false;
    DFA m = toProductAutomata();
    vector<RE> st = s->toRegex(param[0],param[1],param[2],param[3],param[4],-1);

    //Generate a Song and check how much similar it is with the given song
    Song k = Song(ProductAutomata.second, param, console);
    if(k.similarity(*s,false, false)>=limit){
        succes = true;
    }

    //bool succes = m.accepts(st[0].re);
    if(succes){
        log = getCurrTime() + " Found matching results on the characteristics on this Genre!\n\n";
    }else{
        log = getCurrTime() + " This song did not match the requirements of this Genre!\n\n";
    }
    if(console){cout << log;}
    logs.push_back(log);

    return succes;
}

Genre::Genre(Song *&s, Song *&k, const vector<int> &params, const string &name, bool console, bool TFA): param(params), name(name) ,  console(console), TFA(TFA) {
    REQUIRE(params.size()==6, "Params doesn't has all the paramaters");

    //Set Data
    fInitCheck = this;
    members={s,k};
    this->limit=s->similarity(*k,false,false);

    string log = getCurrTime() + "The genre will be constructed on a " + to_string(limit) +  " minimum match %\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Create Separate DFA's
    vector<RE> t = members[0]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1, so we can generate 1 big Regex
    ENFA a = t[0].toENFA();
    DFA z = a.toDFA();
    
    //Other DFA
    vector<RE> t2 = members[1]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1, so we can generate 1 big Regex
    ENFA a2 = t2[0].toENFA();
    DFA z2 = a2.toDFA();

    //Merge the 2 Alphabets
    set<string> Difference;
    set<string> Difference2;
    set_difference(z.getAlphabet().begin(), z.getAlphabet().end(), z2.getAlphabet().begin(), z2.getAlphabet().end(),std::inserter(Difference, Difference.begin()));
    set_difference(z2.getAlphabet().begin(), z2.getAlphabet().end(), z.getAlphabet().begin(), z.getAlphabet().end(),std::inserter(Difference2, Difference2.begin()));
    state* deadstate = new state();
    deadstate->name="{}";
    deadstate->accepting=false;
    deadstate->starting=false;
    vector<state*> Vstates=z2.getStates();
    Vstates.push_back(deadstate);
    z2.setStates(Vstates);
    for(auto c=z2.getAlphabet().begin(); c!=z2.getAlphabet().end(); c++){
        deadstate->addTransitionFunction(*c, deadstate);
    }
    
    for(auto it=Difference.begin(); it!=Difference.end(); it++){
        //add to z2
        for(auto it2=z2.getStates().begin(); it2!=z2.getStates().end(); it2++){
            (*it2)->addTransitionFunction((*it),deadstate);
        }
    }
    deadstate = new state();
    deadstate->name="{}";
    deadstate->accepting=false;
    deadstate->starting=false;
    Vstates=z.getStates();
    Vstates.push_back(deadstate);
    z.setStates(Vstates);
    for(auto c=z.getAlphabet().begin(); c!=z.getAlphabet().end(); c++){
        deadstate->addTransitionFunction(*c, deadstate);
    }
    
    for(auto it=Difference2.begin(); it!=Difference2.end(); it++){
        //add to z2
        for(auto it2=z.getStates().begin(); it2!=z.getStates().end(); it2++){
            (*it2)->addTransitionFunction((*it),deadstate);
        }
    }

    log = getCurrTime() + " Started Product Automata Construction..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    DFA prod = DFA(z,z2, false);
    log = getCurrTime() + " Minimizing our beautiful product..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    if(TFA){
        prod = prod.minimize();
    }

    ProductAutomata = {2,prod}; //Construct First ProductAutomata //True = Doorsnede, False = Unie

    log = getCurrTime() + " Created the new Genre: "+name+" , based on "+ s->getTitle() + " and " + k->getTitle() +"\n\n";
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

    for(const string &k: logs){
        out << k;
    }

    out.close();
}

[[nodiscard]] bool Genre::ProperlyInitialized() const {
    if(fInitCheck== this){
        return true;
    }
    return false;
}

void Genre::switchConsoleOutput() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    console = !console;
}

[[nodiscard]] DFA Genre::getProductAutomata() const {
    return ProductAutomata.second;
}
