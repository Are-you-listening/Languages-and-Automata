//
// Created by anass on 1-5-2023.
//

#include "DFA.h"

DFA::DFA(const string& c) {
    REQUIRE(FileExists(c),"No file found");
    ifstream input(c);
    json j;
    input >> j;

    load(j);
    input.close();
}

void DFA::load(const json &j) {
    if (j.value("type","string") == "DFA"){
        vector<string> a=j["alphabet"];
        for(vector<string>::const_iterator it=a.begin();it!=a.end();it++){
            DFA::alphabet.insert((*it));
        }
        vector<json> b =j["states"];
        int count=0;
        while(count!=b.size()){
            json d1 =b[count];
            state* q= new state(d1["name"],d1["starting"],d1["accepting"] );
            if (q->starting){
                DFA::startingState = q;
            }
            if (q->accepting){
                DFA::endstates.push_back(q);
            }
            DFA::states.push_back(q);
            count++;
        }
        vector<json> d =j["transitions"];
        int count2=0;
        while(count2!=d.size()){
            json d1 =d[count2];
            for(vector<state*>::const_iterator it=DFA::states.begin(); it!=DFA::states.end();it++){
                if (d1["from"] == (*it)->name){
                    for(vector<state*>::const_iterator it2=DFA::states.begin(); it2!=DFA::states.end();it2++){
                        if (d1["to"]==(*it2)->name){
                            (*it)->addTransitionFunction(d1["input"],(*it2));
                        }
                    }
                }
            }
            count2++;
        }
    }
}

bool DFA::accepts(string c) const& {
    state* nextState = startingState;
    for(string::const_iterator it=c.begin();it!=c.end();it++){
        string itString;
        itString.assign(&(*it),&(*it)+1);
        nextState = nextState->states[itString];
        if( find(alphabet.begin(), alphabet.end(), itString)==alphabet.end() ){
            return false;
        }
    }
    return find(endstates.begin(),endstates.end(),nextState)!=endstates.end();
}

void DFA::print()const&{
    json j;
    j["type"] = "DFA";
    j["alphabet"]=DFA::alphabet;
    vector<json> states;
    for(vector<state*>::const_iterator it=DFA::states.begin(); it!=DFA::states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        states.push_back(temp);
    }
    j["states"]=states;
    vector<json> transitions;
    for(vector<state*>::const_iterator it=DFA::states.begin(); it!=DFA::states.end(); it++){
        for(set<string>::const_iterator it2=DFA::alphabet.begin(); it2!=DFA::alphabet.end(); it2++){
            json temp;
            temp["from"]=(*it)->name;
            temp["input"] = (*it2);
            temp["to"]=(*it)->states[(*it2)]->name;
            transitions.push_back(temp);
        }
    }
    j["transitions"]=transitions;
    cout << setw(4) << j <<endl;
}

DFA::DFA() {};

state *DFA::getStartingState() const {
    return startingState;
}

void DFA::setStartingState(state *startingState) {
    DFA::startingState = startingState;
}

const vector<state *> &DFA::getStates() const {
    return states;
}

void DFA::setStates(const vector<state *> &states) {
    DFA::states = states;
}

const set<string> &DFA::getAlphabet() const {
    return alphabet;
}

void DFA::setAlphabet(const set<string> &alphabet) {
    DFA::alphabet = alphabet;
}

const vector<state *> &DFA::getEndstates() const {
    return endstates;
}

void DFA::setEndstates(const vector<state *> &endstates) {
    DFA::endstates = endstates;
}

void state::addTransitionFunction(string c, state * q){
    string* ptr = &c;
    string cCopy = *ptr;
    int count=0;
    while (states.find(cCopy) != states.end()){
        cCopy = *ptr;
        cCopy+=to_string(count);
        count++;
    }
    states[cCopy]=q;
}

state* state::getComplement() {
    state* new_state = new state( name, starting, !accepting ) ;
    new_state->states = states; //TODO Dit is memory wise best wel een probleem
    return new_state;
}

void state::addTransitionFunctionENFA(const string &c, state *q) {
    //string* ptr = &c;
    //string cCopy = *ptr;
    statesENFA[c].insert(q);
}

state::state() {}

state::state(const string &name,bool starting, bool accepting) : name(name), starting(starting),
                                              accepting(accepting) {}

DFA::DFA(DFA& dfa1, DFA& dfa2, bool c) {
    DFA final;
    bool a= true;

    //Merge the alpabets
    MergeAlpabets(dfa1,dfa2);
    final.alphabet=dfa1.alphabet;
    final.alphabet.insert(dfa2.alphabet.begin(),dfa2.alphabet.end());

    //Set Starting State
        state* startstate = new state("("+dfa1.startingState->name+","+dfa2.startingState->name+")" , true, false);
        startstate->starting= true;

        //Set Accepting true/false according (Depending on union/intersection)
        if(dfa1.startingState->accepting && dfa2.startingState->accepting && c){
            startstate->accepting= true;
        } else if((dfa1.startingState->accepting || dfa2.startingState->accepting) && !c) {
            startstate->accepting=true;
        } else{
            startstate->accepting = false;
        }
    final.startingState=startstate;

    set<state*> check_states = {startstate};
    set<tuple<state*, state*, state*>> current_states = {make_tuple(startstate, dfa1.startingState, dfa2.startingState)};
    set<tuple<state*, state*, state*>> new_states;
    set<string> current_names = {startstate->name};
    set<string> new_names;

    while (a){
        //cout << get<0>(*current_states.begin())->name << " " << current_states.size() << endl;
        a=false;
        for(auto &tup: current_states){
            for(set<string>::const_iterator it2=final.alphabet.begin(); it2!=final.alphabet.end(); it2++){

                state* temp;
                state* state1 = get<1>(tup);
                state* state2 = get<2>(tup);

                string name= "(" + state1->states[(*it2)]->name + "," + state2->states[(*it2)]->name + ")";

                temp = new state();
                temp->name = name;
                if ((state1->states[(*it2)]->accepting && state2->states[(*it2)]->accepting && c) || (state1->states[(*it2)]->accepting || state2->states[(*it2)]->accepting && !c)) {
                    temp->accepting = true;
                }

                if (new_names.find(name) == new_names.end() && current_names.find(name) == current_names.end()){
                    check_states.insert(temp);
                    new_states.insert(make_tuple(temp, state1->states[(*it2)], state2->states[(*it2)]));
                    new_names.insert(name);

                    if (temp->accepting){
                        final.endstates.push_back(temp);
                    }

                    a=true;
                }

                get<0>(tup)->addTransitionFunction((*it2),temp);
            }
        }

        if (new_states.size() == 0){
            break;
        }
        current_states.clear();
        current_names.clear();
        current_states = new_states;
        current_names = new_names;
        new_states.clear();
        new_names.clear();
    }

    std::copy(check_states.begin(),check_states.end(), std::back_inserter(final.states));

    json data = final.getJson();
    DFAT temp;
    temp.load(data);

    load(temp.getJson());
}

DFA DFA::minimize() {
    /*
    fstream FILE;
    FILE.open("JSONWORK.json", std::ofstream::out | std::ofstream::trunc);
    streambuf* sbufFILE = FILE.rdbuf();
    streambuf* coutold = cout.rdbuf();
    cout.rdbuf(sbufFILE);
    DFA::print();
    FILE.close();
    DFAT dfatemp("JSONWORK.json");
    DFAT dfatemp2;
    dfatemp2=dfatemp.minimize();
    ofstream FILE2("JSONWORK.json");
    FILE2.open("JSONWORK.json", std::ofstream::out | std::ofstream::trunc);
    sbufFILE = FILE2.rdbuf();
    cout.rdbuf(sbufFILE);
    dfatemp2.print();
    DFA temp("JSONWORK.json");
    cout.rdbuf(coutold);
    FILE2.close();*/

    json data = getJson();
    DFAT temp;

    temp.load(data);
    temp = temp.minimize();

    DFA final;
    final.load(temp.getJson());
    return final;
}

bool DFA::operator==(const DFA &d) {
    json data = getJson();
    json data2 = d.getJson();

    DFAT temp;
    DFAT temp2;
    temp.load(data);
    temp2.load(data2);

    return temp == temp;
}

json DFA::getJson() const{
    json j;
    j["type"] = "DFA";
    j["alphabet"]=DFA::alphabet;
    vector<json> states;
    for(vector<state*>::const_iterator it=DFA::states.begin(); it!=DFA::states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        states.push_back(temp);
    }
    j["states"]=states;
    vector<json> transitions;
    for(vector<state*>::const_iterator it=DFA::states.begin(); it!=DFA::states.end(); it++){
        for(set<string>::const_iterator it2=DFA::alphabet.begin(); it2!=DFA::alphabet.end(); it2++){
            json temp;
            temp["from"]=(*it)->name;
            temp["input"] = (*it2);
            temp["to"]=(*it)->states[(*it2)]->name;
            transitions.push_back(temp);
        }
    }
    j["transitions"]=transitions;
    return j;
}

string DFA::ToRe(){
    json data = getJson();
    DFAE temp;
    temp.load(data);
    REE r = temp.toREE();
    return r.getRegex();
}

DFA DFA::complement() {
    vector<state*> new_states;
    for (state* s: states){
        state* new_state = s->getComplement();
        new_states.push_back(new_state);
    }
    json j = getJson();

    vector<json> states;
    for(vector<state*>::const_iterator it=new_states.begin(); it!=new_states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        states.push_back(temp);
    }

    j["states"]=states;
    DFA d;
    d.load(j);
    return d;
}

ENFA DFA::reverse() {
    vector<state*> new_states;
    map<string, set<pair<state*, string>>> reverse_link_map;
    map<string, state*> name_to_state_map;
    for (state* s: states){
        state* new_state = new state();
        new_state->name = s->name;
        new_state->accepting = s->starting;
        new_state->starting = false;

        for (auto entry: s->states){
            string transition_char = entry.first;
            state* target = entry.second;
            reverse_link_map[target->name].insert(make_pair(new_state, transition_char));
            name_to_state_map[new_state->name] = new_state;
        }
        new_states.push_back(new_state);
    }

    for (auto entry: reverse_link_map){
        state* state_go = name_to_state_map.at(entry.first);
        for (auto p: entry.second){
            state* state_target = p.first;
            string transition_char = p.second;

            state_go->addTransitionFunctionENFA(transition_char, state_target);
        }
    }

    state* new_start = new state();
    new_start->name = "reversed_start";
    new_start->starting = true;
    new_start->accepting = false;
    for (auto s: states){
        if (s->accepting){
            state* target = name_to_state_map.at(s->name);
            new_start->addTransitionFunctionENFA("*", target);
        }
    }

    new_states.push_back(new_start);

    vector<json> states;
    for(vector<state*>::const_iterator it=new_states.begin(); it!=new_states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        states.push_back(temp);
    }
    json j = getJson();
    j["states"] = states;

    set<string> alphabet2 = alphabet;
    alphabet2.insert("*");

    vector<json> transitions;
    for(vector<state*>::const_iterator it=new_states.begin(); it!=new_states.end(); it++){
        for(set<string>::const_iterator it2=alphabet2.begin(); it2!=alphabet2.end(); it2++){

            if ((*it)->statesENFA.find((*it2)) != (*it)->statesENFA.end()){
                for(auto target: (*it)->statesENFA.at((*it2))){
                    json temp;
                    temp["from"]=(*it)->name;
                    temp["input"] = (*it2);
                    temp["to"]=target->name;
                    transitions.push_back(temp);
                }
            }
        }
    }
    j["transitions"]=transitions;

    j["type"]="ENFA";
    j["eps"] = "*";

    ENFA n;
    n.load(j);
    return n;
}

void DFA::load(const set<string> &alfa, const vector<state *> &states, state *start_state,
               const vector<state *> &end_states) {
    alphabet = alfa;
    DFA::states = states;
    DFA::startingState = start_state;
    DFA::endstates = end_states;
}

void DFA::AddState(state *k) {
    states.push_back(k);
    if(k->accepting){
        endstates.push_back(k);
    }

    if(k->starting){
        startingState=k;
    }
}

/*DFA::~DFA() {
    for(auto &k: states){
        delete k;
    }
}*/
