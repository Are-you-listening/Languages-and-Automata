//
// Created by watson on 2/28/23.
//

#include "DFA.h"

DFA::DFA() {
    Q = {"S0","S1","S2"};
    Alphabet = {"0","1"};
    q0 = "S0";
    F = {"S0"};
    delta.setFunction({ {{"S0",49},"S1" } , {{"S0",48},"S0"} , {{"S1",49},"S0"} , {{"S1",48},"S2"} , {{"S2",49},"S2"} , {{"S2",48},"S1"} }); // 48 = char "0" , 49 = char "1"
    sort(Q.begin(),Q.end());
}

bool DFA::accepts(const string &invoer) const {
    for(int i = 0; i<F.size() ; i++){
        if(F[i] == delta.transition(q0,invoer)){
            return true;
        }
    }
    return false;
}

DFA::DFA(const string &inputfile) {
    using json = nlohmann::json;

    // inlezen uit file
    ifstream input(inputfile);
    json j;
    input >> j;

    //std::ifstream f(inputfile);
    //json data = json::parse(f);

    //Parse Alphabet
    Alphabet = j["alphabet"].get<std::vector<string>>();

    //Parse States
    auto states = j["states"];
    for(int i = 0 ; i<states.size(); i++){
        string toestand = states[i]["name"];
        bool start = states[i]["starting"];
        bool accept = states[i]["accepting"];

        if(start){
            q0=toestand;} //Set q0
        if(accept){F.push_back(toestand);} //Houd acceptstates bij
        Q.push_back(toestand);
    }

    //Parse Delta
    auto transitions = j["transitions"];
    for(int i = 0 ; i<transitions.size() ; i++) {
        string from = transitions[i]["from"];
        string to = transitions[i]["to"];
        string in = transitions[i]["input"];
        char input = in[0];

        pair<pair<string,char>,string> transitie = {{from,input},to};
        delta.add(transitie);
    }
    sort(Q.begin(),Q.end());
}

void DFA::print() const {
    nlohmann::json dfa;

    dfa["type"] = "DFA";
    dfa["alphabet"] = Alphabet;

    //Set states
    int count = 0;
    for(int i = 0 ; i<Q.size() ; i++){
        dfa["states"][i]["name"] = Q[i];

        if(Q[i]==q0){
            dfa["states"][i]["starting"] = true;
        } else{
            dfa["states"][i]["starting"] = false;
        }

        dfa["states"][i]["accepting"] = check_accepting_state(Q[i]);

        //Set transitions (voor elke state)
        for(int j = 0; j<Alphabet.size() ; j++){ //Zorg dat iedere input gebruikt wordt
            dfa["transitions"][count]["from"]=Q[i];
            dfa["transitions"][count]["to"]= delta.transition(Q[i],(Alphabet[j])); //Bereken de volgende staat met de gekende input
            dfa["transitions"][count]["input"]=Alphabet[j];
            count++;
        }
    }

    cout << setw(4) << dfa << endl;
}

bool DFA::check_accepting_state(const string &z) const {
    for(int i = 0; i<F.size() ; i++){
        if(F[i]==z){
            return true;
        }
    }
    return false;
}

DFA::DFA(const vector<string> &q, const vector<string> &alphabet, const transitiefunctie &delta, const string &q0,
         const vector<string> &f) : Q(q), Alphabet(alphabet), delta(delta), q0(q0), F(f) {
    sort(Q.begin(),Q.end());
}

/*DFA::DFA(nlohmann::json dfa) {
    nlohmann::json j = dfa;
    //Parse Alphabet
    Alphabet = j["alphabet"].get<std::vector<string>>();

    //Parse States
    auto states = j["states"];
    for(int i = 0 ; i<states.size(); i++){
        string toestand = states[i]["name"];
        bool start = states[i]["starting"];
        bool accept = states[i]["accepting"];

        if(start){
            q0=toestand;} //Set q0
        if(accept){F.push_back(toestand);} //Houd acceptstates bij
        Q.push_back(toestand);
    }

    //Parse Delta
    auto transitions = j["transitions"];
    for(int i = 0 ; i<transitions.size() ; i++) {
        string from = transitions[i]["from"];
        string to = transitions[i]["to"];
        string in = transitions[i]["input"];
        char input = in[0];

        pair<pair<string,char>,string> transitie = {{from,input},to};
        delta.add(transitie);
    }
}*/

DFA DFA::minimize() {
    TFA();
    vector < vector<string> > eqClass;
    DFA min = *this;
    min.F.clear();
    min.Q.clear();
    min.delta.clear();

    //Generate equivalent classes
    for(int i = 0; i<table.size();i++) {
        for (int j = 0; j < table[i].size(); j++) {
            if (table[i][j] == '-') {
                checkEq(eqClass, Q[i + 1], Q[j]);
            }
        }
    }

    //Add states which are not part of the EqClass
    for(string &z: Q){
        bool test = false;
        for(auto &k: eqClass){
            for(auto &s: k){
                if(s==z){
                    test = true; //State in EqClass
                }
            }
        }
        if(!test){
            string t="{"+z+"}";
            min.Q.push_back(t);
            if(check_accepting_state(z)){
                min.F.push_back(t);
            }
            if(z==q0){
                min.q0=t;
            }
        }
    }


    //Add States
    for(auto &z: eqClass){ //Add each state to the Q & F, set q0
        string newQ = NameConvert(z); //State name for the Class
        min.Q.push_back(newQ); //Add state

        //Check starting/accepting
        for(string &k: z){
            if(check_accepting_state(k)){
                min.F.push_back(newQ); //Add final state
            }
            if(k==this->q0){
                min.q0=newQ; //Set startstate
            }
        }
    }

    //Add Transitions
    for(string &state:min.Q){
        vector<string> singles = eqClassToState(state);

        //Get reachable states
        map<char,string> reachable;
        for(string &s:singles){ //For every singleton, get reachable bigstate
            for(const string &a: Alphabet){
                string to = delta.simple_transition(s,a[0]);

                to = ConvertToEqclassName(eqClass,to);

                reachable[a[0]]=to;
            }
        }

        //Set actual transitions
        for(auto it = reachable.begin(); it!=reachable.end(); it++){
            min.delta.add( { {state,it->first}, it->second } ); // {{from,input},to}
        }
    }

    return min;
}

int DFA::find(const string &k) const {
    for(int i = 0; i<Q.size(); i++){
        if(Q[i]==k){
            return i;
        }
    }
    return -1;
}

map<char,map<string,vector<string> > > DFA::generateReverseTransitions() const {
    map<char,map<string,vector<string> > > FFdelta; // [input][to]={from} bv: [0][q1]={q1,q2}, if q1 and q2 go to q1 on input 0

    vector<pair<pair<string, char>, string>> alfa = delta.getFunction(); // state = {{from,input},to}

    for(auto &state: alfa){
        string to = state.second;
        string from = state.first.first;
        char input = state.first.second;
        FFdelta[input][to].push_back(from);
    }
    return FFdelta;
}

vector<pair<string, string> > DFA::makePairs(vector<string> &q1, vector<string> &q2) const {
    vector<pair<string,string>> pairs;

    if(q1.empty() || q2.empty()){
        return pairs;
    }

    for(string &s: q1){
        for(string &k: q2){
            pairs.push_back( {s,k} );
        }
    }

    return pairs;
}

void DFA::printTable() const {
    for(int i = 0; i<table.size();i++){
        string temp=Q[i+1];
        for(int j = 0 ; j<table[i].size();j++){
            string t = string(1,table[i][j]);
            if(table[i][j]=='k'){
                t = "X";
            }
            temp+="   "+t;
        }
        cout << temp << endl;
    }
    cout << " " << "   ";
    for(int i = 0 ; i<Q.size()-1;i++){
        cout << Q[i]+"   ";
    }
    cout << endl;
}

bool DFA::operator==(DFA &rhs) {
    DFA Dmin = *this;

    //Add states from rhs to min
    auto Q2 = rhs.getQ();
    auto F2 = rhs.getF();
    auto delta2 = rhs.delta.getFunction();

    for(auto &k: delta2){
        Dmin.delta.add(k);
    }
    for(auto &k: Q2){
        if(std::find(Dmin.Q.begin(), Dmin.Q.end(),k)==Dmin.Q.end()){
            Dmin.Q.push_back(k);
        }
    }
    for(auto &k: F2){
        if(std::find(Dmin.F.begin(), Dmin.F.end(),k)==Dmin.F.end()){
            Dmin.F.push_back(k);
        }
    }

    //TFA on min
    Dmin.table.clear();
    Dmin.TFA();

    int iq1 = Dmin.find(rhs.q0);
    int iq2 = Dmin.find(this->q0);
    int maxt = max(iq1,iq2);
    if(maxt!=0){
        maxt--;
    }

    Dmin.printTable();

    if( Dmin.table[maxt][min(iq2,iq1)]=='-'){
        return true;
    }else{
        return false;
    }
}

bool DFA::operator!=(DFA &rhs){
    return !(*this == rhs);
}

void DFA::TFA() {
    map<char,map<string,vector<string> > > FFdelta = generateReverseTransitions(); // map[state][inputsymbol]= {Reachable states} | //Construct "function" table

    //Initialse actual table
    for(int i = 1; i<Q.size();i++){
        vector<char> temp;
        for(int j = 0; j<i;j++){
            if(check_accepting_state(Q[i]) != check_accepting_state(Q[j]) ){ //Distuingbisable | Q[i]; state verticaal, Q[j]; state horizontaal
                temp.push_back('k');
            } else{
                temp.push_back('-');
            }
        }
        table.push_back(temp);
    }

    for(int i = 0; i<table.size();i++){
        for(int j = 0 ; j<table[i].size();j++){
            if(table[i][j]=='k'){
                //State links in colom (verticaal) = Q[i+1], State vanonder, horizontaal in de table = Q[j]
                string q1 = Q[i+1];
                string q2 = Q[j];

                //Find new Distuingisable pairs
                for(const string &a: Alphabet){
                    auto t1 = FFdelta[a[0]][q1];
                    auto t2 = FFdelta[a[0]][q2];
                    vector<pair<string,string>> pairs = makePairs( t1  , t2 );

                    for(auto &z: pairs){ //For every pair, set a new X
                        int iq1 = find(z.first); //Find index in Q off this state
                        int iq2 = find(z.second);
                        int maxt = max(iq1,iq2);
                        if(maxt!=0){
                            maxt--;
                        }

                        if(table[maxt][min(iq2,iq1)]=='-'){ //Only overwrite with k if its not been done already!
                            table[maxt][min(iq2,iq1)]='k'; //does this work properly?
                        }
                    }
                }

                //Mark as Done
                table[i][j]='X';

                //Reset and loop again over the table
                i = 0;
                j = 0;
            }
        }
    }
}

void DFA::checkEq(vector < vector<string> > &eqClass, string &q1, string &q2) {
    for (int i = 0; i < eqClass.size(); i++) {
        bool t1 = false;
        bool t2 = false;
        if ((std::find(eqClass[i].begin(), eqClass[i].end(), q2)) == eqClass[i].end()) {
            t1=true;
        }
        if ((std::find(eqClass[i].begin(), eqClass[i].end(), q1)) == eqClass[i].end()) {
            t2= true;
        }

        if(t1 && t2){
            vector<string> temp = {q1,q2};
            std::sort(temp.begin(), temp.end());
            eqClass.push_back(temp);
            return;
        }
        if(t1){
            eqClass[i].push_back(q2);
        }
        if(t2){
            eqClass[i].push_back(q1);
        }
        std::sort(eqClass[i].begin(), eqClass[i].end()); //Sort the eqclass
        return;
    }
    eqClass.push_back({q1,q2}); //In case no others found, make a new Class
}

const vector<string> &DFA::getQ() const {
    return Q;
}

const vector<string> &DFA::getAlphabet() const {
    return Alphabet;
}

const transitiefunctie &DFA::getDelta() const {
    return delta;
}

const string &DFA::getQ0() const {
    return q0;
}

const vector<string> &DFA::getF() const {
    return F;
}

const vector<vector<char>> &DFA::getTable() const {
    return table;
}

