//
// Created by anass on 1-5-2023.
//

#include "ENFA.h"

ENFA::ENFA(const string& c) {
    ifstream input(c);
    json j;
    input >> j;
    if (j.value("type","string") == "ENFA"){
        ENFA::eps=j["eps"];
        vector<string> a=j["alphabet"];
        for(string &it: a){
            ENFA::alphabet.insert(it);
        }
        vector<json> b =j["states"];
        int count=0;
        while(count!=b.size()){
            json d1 =b[count];
            state* q(new state());
            q->name=d1["name"];
            q->starting=d1["starting"];
            q->accepting=d1["accepting"];
            if (q->starting){
                ENFA::startingState = q;
            }
            if (q->accepting){
                ENFA::endstates.push_back(q);
            }
            ENFA::states.push_back(q);
            count++;
        }
        vector<json> d =j["transitions"];
        int count2=0;
        while(count2!=d.size()){
            json d1 =d[count2];
            for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end();it++){
                if (d1["from"] == (*it)->name){
                    for(vector<state*>::const_iterator it2=ENFA::states.begin(); it2!=ENFA::states.end();it2++){
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

bool ENFA::accepts(string c) const& {
    string itStringCHECK;
    for(string::const_iterator it=c.begin(); it!=c.end(); it++){
        itStringCHECK.assign(&(*it),&(*it)+1);
        if (ENFA::alphabet.find(itStringCHECK)==ENFA::alphabet.end()){
            return false;
        }
    }
    state* nextState = startingState;

    string itString;
    for(string::const_iterator it=c.begin();it!=c.end();it++){
        itString.assign(&(*it),&(*it)+1);
        for(map<string,state*>::const_iterator it2=nextState->states.begin(); it2 != nextState->states.end(); it2++){
            if ((*it2->first.begin()) == (*ENFA::eps.begin())){
                ENFA enfa(*this, nextState->states[it2->first]);
                if (enfa.accepts(c)){
                    return true;
                }
            }
            if (nextState->states.find(itString.substr(0, 1)) != nextState->states.end()){
                ENFA enfa(*this, nextState->states[itString.substr(0, 1)]);
                if (itString.empty()){
                    return find(endstates.begin(),endstates.end(),nextState->states[itString.substr(0, 1)]) != endstates.end();
                }
                string COPY = c;
                if (enfa.accepts(COPY.erase(0,COPY.find(itString)+1))){
                    return true;
                }
            }
        }
        return false;
    }
    for(map<string,state*>::const_iterator it2=nextState->states.begin(); it2 != nextState->states.end(); it2++) {
        if ((*it2->first.begin()) == (*ENFA::eps.begin())) {
            ENFA enfa(*this, nextState->states[it2->first]);
            if (enfa.accepts(c)) {
                return true;
            }
        }
    }
    if (!(itString.empty())){
        return false;
    }
    return find(endstates.begin(),endstates.end(),nextState)!=endstates.end();
}

void ENFA::print()const&{
    json j;
    j["type"] = "ENFA";
    j["alphabet"]=ENFA::alphabet;

    vector<json> States;
    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        States.push_back(temp);
    }
    j["states"]=States;
    vector<json> transitions;

    set<string> alphabet2 = alphabet;
    alphabet2.insert("*");

    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        for(set<string>::const_iterator it2=alphabet2.begin(); it2!=alphabet2.end(); it2++){
            json temp;
            if ((*it)->states.find((*it2)) != (*it)->states.end()){
                temp["from"]=(*it)->name;
                temp["input"] = (*it2);
                temp["to"]=(*it)->states[(*it2)]->name;
                transitions.push_back(temp);
            }
        }
    }
    j["transitions"]=transitions;
    cout << setw(4) << j <<endl;
}

ENFA::ENFA(const ENFA &enfa, state* state) {
    startingState=state;
    states=enfa.states;
    alphabet=enfa.alphabet;
    endstates=enfa.endstates;
    eps=enfa.eps;
}

vector<state*> ENFA::ECLOSE(state*Etransition)const&{
    vector<state*> EStates;
    for(map<string,state*>::const_iterator it=Etransition->states.begin(); it != Etransition->states.end(); it++) {
        if((*it->first.begin())==(*ENFA::eps.begin()) && find(EStates.begin(),EStates.end(), it->second)==EStates.end()){
            if (find(EStates.begin(),EStates.end(),it->second)==EStates.end()){
                EStates.push_back(it->second);
            }
        }
    }
    return EStates;
}

vector<state*> ENFA::ECLOSE2(state*Etransition, string input)const&{
    vector<state*> EStates;
    for (map<string, state*>::const_iterator it = Etransition->states.begin(); it != Etransition->states.end(); it++) {
        if ((*it->first.begin()) == (*ENFA::eps.begin())) {
            vector<state*> temp = ECLOSE2(Etransition->states[ENFA::eps], input);
            EStates.insert(EStates.end(), temp.begin(), temp.end());
        } else {
            if (Etransition->states.find(input) != Etransition->states.end()) {
                if (find(EStates.begin(), EStates.end(), Etransition->states[input]) == EStates.end()) {
                    EStates.push_back(Etransition->states[input]);
                }
            }
        }
        for (int i = 0; Etransition->states.find(input + to_string(i)) != Etransition->states.end(); i++) {
            state*temp = Etransition->states[input + to_string(i)];
            if (find(EStates.begin(), EStates.end(), temp) == EStates.end()) {
                EStates.push_back(temp);
            }
        }
    }
    return EStates;
}

void ENFA::Etransitions(vector<state*> &States)const&{
    vector<state*> new_states;
    new_states.insert(new_states.begin(), States.begin(), States.end());
    Etransitions(new_states, States);
}

void ENFA::Etransitions(vector<state*> &States, vector<state*> &found)const&{
    for(vector<state*>::const_iterator it=States.begin(); it != States.end(); it++){
        vector<state*> states2= ECLOSE((*it));
        vector<state*> new_states;
        for(vector<state*>::const_iterator it2=states2.begin(); it2!=states2.end(); it2++){
            if(find(found.begin(),found.end(),(*it2))==found.end()){
                new_states.push_back((*it2));
            }
        }
        found.insert(found.end(), new_states.begin(), new_states.end());
        Etransitions(new_states, found);
    }
}

map<pair<char,string>, vector<string>> ENFA::possibleStates(state* nextState, const string &name)const&{
    map<pair<char,string>, vector<string>> states2;
    for(map<string,state*>::const_iterator it2=nextState->states.begin(); it2 != nextState->states.end(); it2++){
        if ((*it2->first.begin())!=(*ENFA::eps.begin()) && it2->first.size()==1){
            if (nextState->states.find(it2->first + "0") != nextState->states.end()){
                vector<state*> eclosure;
                state* temp = nextState->states[it2->first];
                eclosure.push_back(temp);
                for(int i=0; nextState->states.find(it2->first + to_string(i)) != nextState->states.end(); i++){
                    temp = nextState->states[it2->first + to_string(i)];
                    vector<state*> tempeclosure = ENFA::ECLOSE(temp);
                    eclosure.push_back(temp);
                    eclosure.insert(eclosure.end(),tempeclosure.begin(),tempeclosure.end());
                }

                vector<state*> eclosureinput;
                for(map<string,state*>::const_iterator it3=nextState->states.begin(); it3 != nextState->states.end(); it3++){
                    if ((*it3->first.begin())==(*ENFA::eps.begin())){
                        eclosureinput = ENFA::ECLOSE2(nextState->states[it3->first], it2->first);
                    }
                }
                eclosure.insert(eclosure.end(),eclosureinput.begin(),eclosureinput.end());

                if(!eclosure.empty()){
                    ENFA::Etransitions(eclosure);
                }
                for(vector<state*>::const_iterator it3=eclosure.begin(); it3!=eclosure.end();it3++){
                    if (find(states2[make_pair((*it2->first.begin()),name)].begin(),states2[make_pair((*it2->first.begin()),name)].end(),(*it3)->name)==states2[make_pair((*it2->first.begin()),name)].end()){
                        states2[make_pair((*it2->first.begin()),name)].push_back((*it3)->name);
                    }
                }
            } else {
                if(name.size()>1){
                    for(string::const_iterator it3=name.begin(); it3!=name.end(); it3++){
                        for(vector<state*>::const_iterator it4=ENFA::states.begin(); it4!=ENFA::states.end(); it4++){
                            if((*it3)==(*(*it4)->name.begin())){
                                map<pair<char,string>, vector<string>> temp = ENFA::possibleStates((*it4),(*it4)->name);
                                for(map<pair<char,string>, vector<string>>::const_iterator it4=temp.begin(); it4!=temp.end(); it4++){
                                    if (it4->first.first==(*it2->first.begin())){
                                        if(it4->first.first==(*it2->first.begin())){
                                            for(vector<string>::const_iterator it5=it4->second.begin(); it5!=it4->second.end(); it5++){
                                                if (find(states2[make_pair((*it2->first.begin()),name)].begin(),states2[make_pair((*it2->first.begin()),name)].end(),(*it5))==states2[make_pair((*it2->first.begin()),name)].end()){
                                                    states2[make_pair((*it2->first.begin()),name)].push_back((*it5));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    continue;
                }
                vector<string> state1;
                state1.push_back(it2->second->name);
                states2[make_pair((*it2->first.begin()),name)]=state1;
                state* ETransition = it2->second;
                vector<state*> EClOSURE = ENFA::ECLOSE(ETransition);

                vector<state*> EClOSUREINPUT;
                for(map<string,state*>::const_iterator it3=it2->second->states.begin(); it3 != it2->second->states.end(); it3++){
                    if ((*it3->first.begin())==(*ENFA::eps.begin())){
                        EClOSUREINPUT = ENFA::ECLOSE2(nextState,it2->first);
                    }
                }
                EClOSURE.insert(EClOSURE.end(),EClOSUREINPUT.begin(),EClOSUREINPUT.end());

                if(!EClOSURE.empty()){
                    ENFA::Etransitions(EClOSURE);
                }
                for(vector<state*>::const_iterator it3=EClOSURE.begin(); it3!=EClOSURE.end();it3++){
                    if (find(states2[make_pair((*it2->first.begin()),name)].begin(),states2[make_pair((*it2->first.begin()),name)].end(),(*it3)->name)==states2[make_pair((*it2->first.begin()),name)].end()){
                        states2[make_pair((*it2->first.begin()),name)].push_back((*it3)->name);
                    }
                }
            }
        }
    }
    return states2;
}

DFA* ENFA::toDFA() &{
    ENFAT t;
    t.load(getJson());
    DFA* dfa = t.toDFA();

    return dfa;
}

void ENFA::printStats()const&{
    cout << "no_of_states="+ to_string(ENFA::states.size())<<endl;
    map<string,int> transit;
    map<int,int> degree;
    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        if(degree.find((*it)->states.size()) == degree.end()){
            degree[(*it)->states.size()]=1;
        } else {
            degree[(*it)->states.size()]+=1;
        }
        for(map<string,state*>::const_iterator it2=(*it)->states.begin(); it2 != (*it)->states.end(); it2++){
            if((*it)->states.find((*it2).first) == (*it)->states.end()){
                transit[(*it2).first.substr(0,1)]=1;
            } else {
                transit[(*it2).first.substr(0,1)]+=1;
            }
        }
    }
    for(map<string,int>::const_iterator it=transit.begin(); it!=transit.end(); it++){
        if ((*(*it).first.begin())==(*ENFA::eps.begin())){
            cout << "no_of_transitions[" + ENFA::eps + "]=" + to_string((*it).second)<<endl;
        }
    }
    for(map<string,int>::const_iterator it=transit.begin(); it!=transit.end(); it++){
        if ((*(*it).first.begin()) != (*ENFA::eps.begin())){
            cout << "no_of_transitions[" + (*it).first + "]=" + to_string((*it).second)<<endl;
        }
    }
    for(map<int,int>::const_iterator it=degree.begin(); it!=degree.end(); it++){
        cout << "degree[" + to_string((*it).first) + "]=" + to_string((*it).second)<<endl;
    }
}

void ENFA::concatenate(const ENFA& enfa) &{
    states.insert(states.end(),enfa.states.begin(),enfa.states.end());
    state* temp=*endstates.begin();
    for(state* s: endstates){
        s->accepting= false;
    }
    endstates=enfa.endstates;
    alphabet.insert(enfa.alphabet.begin(),enfa.alphabet.end());
    enfa.startingState->starting= false;
    temp->addTransitionFunction(eps,enfa.startingState);
}

ENFA::ENFA() {};

void ENFA::load(const json &j) {
    if (j.value("type","string") == "ENFA"){
        ENFA::eps=j["eps"];
        vector<string> a=j["alphabet"];
        for(vector<string>::const_iterator it=a.begin();it!=a.end();it++){
            ENFA::alphabet.insert((*it));
        }
        vector<json> b =j["states"];
        long unsigned int count=0;
        while(count!=b.size()){
            json d1 =b[count];
            state* q(new state());
            q->name=d1["name"];
            q->starting=d1["starting"];
            q->accepting=d1["accepting"];
            if (q->starting){
                ENFA::startingState = q;
            }
            if (q->accepting){
                ENFA::endstates.push_back(q);
            }
            ENFA::states.push_back(q);
            count++;
        }
        vector<json> d =j["transitions"];
        long unsigned int count2=0;
        while(count2!=d.size()){
            json d1 =d[count2];
            for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end();it++){
                if (d1["from"] == (*it)->name){
                    for(vector<state*>::const_iterator it2=ENFA::states.begin(); it2!=ENFA::states.end();it2++){
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
json ENFA::getJson() const {
    json j;
    j["type"] = "ENFA";
    j["alphabet"]=ENFA::alphabet;
    vector<json> States;
    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        States.push_back(temp);
    }
    j["states"]=States;
    vector<json> transitions;

    set<string> alphabet2 = alphabet;
    alphabet2.insert("*");

    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        for(set<string>::const_iterator it2=alphabet2.begin(); it2!=alphabet2.end(); it2++){
            int count = -1;
            while (true){
                json temp;

                string key;
                if (count == -1){
                    key = (*it2);
                }else{
                    key = (*it2)+ to_string(count);
                }

                if ((*it)->states.find(key) != (*it)->states.end()){
                    temp["from"]=(*it)->name;
                    temp["input"] = (*it2);
                    temp["to"]=(*it)->states[key]->name;
                    transitions.push_back(temp);
                    count += 1;
                }else{
                    break;
                }
            }
        }
    }
    j["transitions"]=transitions;
    j["eps"] = "*";
    return j;
}

json ENFA::getJsonNfa() const {
    json j;
    j["type"] = "NFA";
    j["alphabet"]=ENFA::alphabet;

    vector<json> transitions;
    map<string, vector<json>> t_map;

    map<string, pair<string, string>> link_skipper;

    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        for(set<string>::const_iterator it2=alphabet.begin(); it2!=alphabet.end(); it2++){
            int count = -1;
            while (true){
                json temp;

                string key;
                if (count == -1){
                    key = (*it2);
                }else{
                    key = (*it2)+ to_string(count);
                }

                if ((*it)->states.find(key) != (*it)->states.end()){

                    temp["from"]=(*it)->name;
                    temp["input"] = (*it2);
                    temp["to"]=(*it)->states[key]->name;
                    //transitions.push_back(temp);
                    t_map[(*it)->states[key]->name].push_back(temp);
                    link_skipper[(*it)->states[key]->name] = make_pair((*it2), (*it)->name);
                    count += 1;
                }else{
                    break;
                }
            }
        }
    }

    for(auto &state: this->states){
        int count = -1;
        while (true){
            json temp;

            string key;
            if (count == -1){
                key = "*";
            }else{
                key = "*"+ to_string(count);
            }

            if (state->states.find(key) != state->states.end()){

                pair<string, string> before = link_skipper.at(state->name);
                string target_name = state->states[key]->name;

                temp["from"]=before.second;
                temp["input"] = before.first;
                temp["to"]=target_name;

                transitions.push_back(temp);

                t_map[state->name].clear();

                count += 1;
            }else{
                break;
            }
        }
    }

    for (auto &entry: t_map){
        for (auto &t: entry.second){
            transitions.push_back(t);
        }
    }

    vector<json> nodes;
    for(auto &k: this->states){
        if (link_skipper.find(k->name) == link_skipper.end() || !t_map.at(k->name).empty()){
            json temp;
            temp["name"]=k->name;
            temp["starting"]=k->starting;
            temp["accepting"]=k->accepting;
            nodes.push_back(temp);
        }
    }
    j["states"]=nodes;

    j["transitions"]=transitions;
    return j;
}

ENFA::~ENFA() {
    if (deletable){
        for(auto s: states){
            delete s;
        }
    }
}

ENFA::ENFA(state*startingState, const vector<state*> &states, const set<string> &alphabet,
           const vector<state*> &endstates, const string &eps) : startingState(startingState), states(states),
                                                                  alphabet(alphabet), endstates(endstates), eps(eps) {}
