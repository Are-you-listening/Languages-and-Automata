//
// Created by watson on 4/28/23.
//

#include "Genre.h"

void Genre::addGenre(Song *&s) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    string log;

    members.push_back(s);
    log = getCurrTime()+ " Successfully added " + s->getTitle() + " to the Genre!\n\n";
    toProductAutomata();
    /*
    if(inGenre(s)){
        members.push_back(s);
        log = getCurrTime()+ " Successfully added " + s->getTitle() + " to the Genre!\n\n";
    }else{
        log = getCurrTime()+ " Could not add this Song the Genre.\n\n";
    }*/
    if(console){cout << log;}
    logs.push_back(log);
}

DFA* Genre::toProductAutomata() {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime()+ " Converting to ProductAutomata..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Loop over each Song
    for(long unsigned int i = ProductAutomata.first; i<members.size(); i++) {
        vector<RE> t = members[i]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1, so we can generate 1 big Regex
        ENFA a = t[0].toENFA();
        DFA* s = a.toDFA();

        ProductAutomata.second = new DFA(ProductAutomata.second, s, false); //Extend ProductAutomata
    }
    ProductAutomata.first = (int) members.size();

    if(TFA){
        ProductAutomata.second = ProductAutomata.second->minimize(); //So we will use less space
    }

    return ProductAutomata.second;
}

bool Genre::inGenre(Song *&s) {
    /*
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    string log = getCurrTime()+ " Validating if "+s->getTitle()+" is part of this genre..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    bool succes = false;
    DFA* m = toProductAutomata();
    vector<RE> st = s->toRegex(param[0],param[1],param[2],param[3],param[4],-1);

    //Generate a Song and check how much similar it is with the given song
    Song k = Song(ProductAutomata.second, param, console);
    if(k.similarity(s,false, false)>=limit){
        succes = true;
    }

    //bool succes = m->accepts(st[0].re);
    if(succes){
        log = getCurrTime() + " Found matching results on the characteristics on this Genre!\n\n";
    }else{
        log = getCurrTime() + " This song did not match the requirements of this Genre!\n\n";
    }
    if(console){cout << log;}
    logs.push_back(log);
    */


    return similarity(s)>=limit;
}

Genre::Genre(Song *s, Song *k, const vector<int> &params, const string &name, bool console, bool TFA): param(params), name(name) ,  console(console), TFA(TFA) {
    REQUIRE(params.size()==6, "Params doesn't has all the parameters");

    //Set Data
    fInitCheck = this;
    members={s,k};
    limit = 70;

    string log = getCurrTime() + "The genre will be constructed on a " + to_string(limit) +  " minimum match %\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    //Create Separate DFA's
    vector<RE> t = members[0]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1, so we can generate 1 big Regex
    ENFA a = t[0].toENFA();
    DFA* z = a.toDFA();
    
    //Other DFA
    vector<RE> t2 = members[1]->toRegex(param[0],param[1],param[2],param[3],param[4],-1); //Set pattern to -1, so we can generate 1 big Regex
    ENFA a2 = t2[0].toENFA();
    DFA* z2 = a2.toDFA();

    log = getCurrTime() + " Started Product Automata Construction..\n\n";
    if(console){cout << log;}
    logs.push_back(log);

    DFA* prod = new DFA(z,z2, false);
    if(TFA){
        log = getCurrTime() + " Minimizing our beautiful product..\n\n";
        if(console){cout << log;}
        logs.push_back(log);

        prod = prod->minimize();
    }
    ProductAutomata.second = prod; //Construct First ProductAutomata //True = Doorsnede, False = Unie
    ProductAutomata.first = 2;

    log = getCurrTime() + " Created the new Genre: "+name+" , based on "+ s->getTitle() + " and " + k->getTitle() +"\n\n";
    if(console){cout << log;}
    logs.push_back(log);
    //Free memory
    delete z;
    delete z2;

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

[[nodiscard]] DFA* Genre::getProductAutomata() const {
    return ProductAutomata.second;
}

Genre::~Genre() {
    delete ProductAutomata.second;
}

double Genre::similarity(Song *s) {
    int count = 0;
    for (auto p: param){
        if (p >= 1){
            count += 1;
        }

    }
    vector<RE> r = s->toRegex(param[0], param[1], param[2], param[3], param[4], 1);

    DFA* r2 = ProductAutomata.second;
    auto  v = r2->getStates();
    vector<vector<DFA*>> df = ProductAutomata.second->split(2);

    vector<double> results = Song::similar(df, r, false, false);

    int a=1;
    int b=1;
    int c=1;
    int d=1;
    int e=1;
    int V0=param[0];
    int V1=param[1];
    int V2=param[2];
    int V3=param[3];
    int V4=param[4];
    if(param[4]==0){
        a=0;
        V4=1;
    }
    if(param[3]==0){
        b=0;
        V3=1;
    }
    if(param[2]==0){
        c=0;
        V2=1;
    }
    if(param[1]==0){
        d=0;
        V1=1;
    }
    if(param[0]==0){
        e=0;
        V0=1;
    }
    double result = (7*results[0]+2*results[1]+1*results[2])/10;
    //cout << result << endl;
    return result;
}
