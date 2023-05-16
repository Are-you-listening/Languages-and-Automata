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
            state* q= new state();
            q->name=d1["name"];
            q->starting=d1["starting"];
            q->accepting=d1["accepting"];
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
    state::states[cCopy]=q;
}

state* state::getComplement() {
    state* new_state = new state() ;
    new_state->name = name;
    new_state->states = states;
    new_state->accepting = !accepting;
    new_state->starting = starting;
    return new_state;
}

void state::addTransitionFunctionENFA(string c, state *q) {
    string* ptr = &c;
    string cCopy = *ptr;
    state::statesENFA[cCopy].insert(q);
}

DFA::DFA(DFA& dfa1, DFA& dfa2, bool c) {
    bool a= true;
    state* startstate = new state();
    startstate->starting= true;
    DFA::alphabet=dfa1.alphabet;
    DFA::alphabet.insert(dfa2.alphabet.begin(),dfa2.alphabet.end());
    startstate->name="("+dfa1.startingState->name+","+dfa2.startingState->name+")";
    if(dfa1.startingState->accepting && dfa2.startingState->accepting && c){
        startstate->accepting= true;
    } else if((dfa1.startingState->accepting || dfa2.startingState->accepting) && !c) {
        startstate->accepting=true;
    } else{
        startstate->accepting = false;
    }
    DFA::states.push_back(startstate);
    DFA::startingState=startstate;

    vector<tuple<state*, state*, state*>> current_states = {make_tuple(startstate, dfa1.startingState, dfa2.startingState)};
    vector<tuple<state*, state*, state*>> new_states;
    while (a){
        cout << get<0>(current_states[0])->name << endl;
        a=false;
        for(auto tup: current_states){
            for(set<string>::const_iterator it2=DFA::alphabet.begin(); it2!=DFA::alphabet.end(); it2++){
                /*
                string state1_string;
                string state2_string;
                bool b1=true;
                for(string::iterator it3=(*it)->name.begin()+1; it3!=(*it)->name.end(); it3++){
                    if ((*it3)==','){
                        b1= false;
                    } else {
                        if(b1){
                            state1_string.push_back((*it3));
                        } else {
                            if ((*it3)!=')'){
                                state2_string.push_back((*it3));
                            }
                        }
                    }
                }
                state* state1;
                state* state2;

                for(vector<state*>::const_iterator it3=dfa1.states.begin(); it3!=dfa1.states.end(); it3++){
                    if ((*it3)->name==state1_string){
                        state1 = (*it3);
                    }
                }
                for(vector<state*>::const_iterator it3=dfa2.states.begin(); it3!=dfa2.states.end(); it3++){
                    if ((*it3)->name==state2_string){
                        state2 = (*it3);
                    }
                }*/

                state* temp;
                state* state1 = get<1>(tup);
                state* state2 = get<2>(tup);

                bool b2=false;
                string name= "(" + state1->states[(*it2)]->name + "," + state2->states[(*it2)]->name + ")";
                for(vector<state*>::const_iterator it3=DFA::states.begin(); it3!=DFA::states.end(); it3++){
                    if ((*it3)->name==name){
                        b2=true;
                        temp=(*it3);
                    }
                }
                if(!b2){
                    temp = new state();
                    temp->name = name;
                    if ((state1->states[(*it2)]->accepting && state2->states[(*it2)]->accepting && c) || (state1->states[(*it2)]->accepting || state2->states[(*it2)]->accepting && !c)) {
                        temp->accepting = true;
                        if (find(DFA::endstates.begin(),DFA::endstates.end(),temp)==DFA::endstates.end()) {
                            DFA::endstates.push_back(temp);
                            new_states.push_back(make_tuple(temp, state1->states[(*it2)], state2->states[(*it2)]));
                        }
                    }

                    if (find(DFA::states.begin(),DFA::states.end(),temp)==DFA::states.end()){
                        DFA::states.push_back(temp);
                        new_states.push_back(make_tuple(temp, state1->states[(*it2)], state2->states[(*it2)]));
                        a=true;
                    }
                }
                if (get<0>(tup)->states.find((*it2)) == get<0>(tup)->states.end()){
                    get<0>(tup)->addTransitionFunction((*it2),temp);
                }
            }
        }

        if (new_states.size() == 0){
            break;
        }
        current_states.clear();
        current_states = new_states;
        new_states.clear();
        /*
        for(vector<state*>::const_iterator it=TOBEADDED.begin(); it!=TOBEADDED.end(); it++){
            if (find(DFA::states.begin(),DFA::states.end(),(*it))==DFA::states.end()){
                DFA::states.push_back((*it));
                a=true;
            }
        }*/
        /*
        for(vector<state*>::const_iterator it=TOBEADDEDEND.begin(); it!=TOBEADDEDEND.end(); it++){
            if (find(DFA::endstates.begin(),DFA::endstates.end(),(*it))==DFA::endstates.end()) {
                DFA::endstates.push_back((*it));
            }
        }*/
    }
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