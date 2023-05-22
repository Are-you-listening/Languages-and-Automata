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
        for(const auto &k: a){
            DFA::alphabet.insert(k);
        }
        vector<json> b =j["states"];
        int count=0;
        while(count!=b.size()){
            json d1 =b[count];
            auto q(new state(d1["name"],d1["starting"],d1["accepting"] ));
            if (q->starting){
                DFA::startingState = q;
            }
            if (q->accepting){
                DFA::endstates.push_back(q);
            }
            DFA::states[q->name]=q;
            count++;
        }
        vector<json> d =j["transitions"];
        int count2=0;
        while(count2!=d.size()){
            json d1 =d[count2];
            for(const auto &it: states){
                if (d1["from"] == it.first){
                    for(const auto &it2: states){
                        if (d1["to"]==it2.first){
                            it.second->addTransitionFunction(d1["input"],it2.second);
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
    vector<json> States;
    for(const auto &it : DFA::states){
        json temp;
        temp["name"]=it.first;
        temp["starting"]=it.second->starting;
        temp["accepting"]=it.second->accepting;
        States.push_back(temp);
    }
    j["states"]=States;
    vector<json> transitions;
    for(const auto &it : DFA::states){
        for(const auto &it2 : DFA::alphabet){
            json temp;
            temp["from"]=it.first;
            temp["input"] = it2;
            temp["to"]=it.second->states[it2]->name;
            transitions.push_back(temp);
        }
    }
    j["transitions"]=transitions;
    cout << setw(4) << j <<endl;
}

DFA::DFA() = default;

state*DFA::getStartingState() const {
    return startingState;
}

void DFA::setStartingState(state* StartingState) {
    DFA::startingState = StartingState;
}

const set<string> &DFA::getAlphabet() const {
    return alphabet;
}

void DFA::setAlphabet(const set<string> &Alphabet) {
    DFA::alphabet = Alphabet;
}

void DFA::setEndstates(const vector<state*> &Endstates) {
    DFA::endstates = Endstates;
}

void state::addTransitionFunction(string c, state* q){
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
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

state* state::getComplement() const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    auto new_state(new state( name, starting, !accepting )) ;
    new_state->states = states; //TODO Dit is memory wise best wel een probleem
    return new_state;
}

void state::addTransitionFunctionENFA(const string &c, state*q) {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    //string* ptr = &c;
    //string cCopy = *ptr;
    statesENFA[c].insert(q);
}

state::state() {fInitCheck=this;}

state::state(string name,bool starting, bool accepting) : name(std::move(name)), starting(starting),
                                                                 accepting(accepting) {fInitCheck=this;}

bool state::ProperlyInitialized() const {
    return this->fInitCheck==this;
}

DFA::DFA(DFA* dfa1, DFA* dfa2, bool c) {
    //Merge the alphabets
    MergeAlpabets(dfa1,dfa2);
    alphabet=dfa1->alphabet;
    alphabet.insert(dfa2->alphabet.begin(),dfa2->alphabet.end());

    //Set Starting State
    auto startstate(new state("{"+dfa1->startingState->name+","+dfa2->startingState->name+"}" , true, false));
    //Set Accepting true/false according (Depending on union/intersection)
    if(dfa1->startingState->accepting && dfa2->startingState->accepting && c){
        startstate->accepting= true;
    } else if( (dfa1->startingState->accepting || dfa2->startingState->accepting) && !c) {
        startstate->accepting=true;
    } else{
        startstate->accepting = false;
    }
    startingState=startstate;
    states[startstate->name]=startstate;

    queue<tuple<state*, state*, state*>> todo;
    todo.push({make_tuple(startstate, dfa1->startingState, dfa2->startingState)});

    while(!todo.empty()) {
        auto tup = todo.front();
        todo.pop();
        for (const auto &it2: alphabet) {
            state *state1 = get<1>(tup);
            state *state2 = get<2>(tup);

            state* new1 = state1->states[it2];
            state* new2 = state2->states[it2];

            string name = "{" + new1->name + "," + new2->name + "}";
            auto temp(new state(name, false, false));

            if (states.find(temp->name) == states.end()) {
                todo.emplace( temp, new1 , new2 );

                if ((state1->states[it2]->accepting && state2->states[it2]->accepting && c) ||
                    (state1->states[it2]->accepting || state2->states[it2]->accepting && !c)) {
                    temp->accepting = true;
                    endstates.push_back(temp);
                }

                states[temp->name] = temp;
                get<0>(tup)->addTransitionFunction(it2, temp);
            } else {
                state* temp2 = states.find(temp->name)->second;
                delete temp;
                get<0>(tup)->addTransitionFunction(it2, temp2);
                continue;
            }
        }
    }
}

DFA* DFA::minimize() const {
    json data = getJson();
    DFAT temp;

    temp.load(data);
    temp = temp.minimize();

    DFA* final =  new DFA();
    final->load(temp.getJson());
    return final;
}

bool DFA::operator==(const DFA &d) const {
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
    vector<json> States;
    for(const auto &it: this->states){
        json temp;

        temp["name"]=it.first;
        temp["starting"]=it.second->starting;
        temp["accepting"]=it.second->accepting;
        States.push_back(temp);
    }
    j["states"]=States;
    vector<json> transitions;
    for(const auto &it: this->states){
        for(const auto &it2: alphabet){
            json temp;
            temp["from"]=it.first;
            temp["input"] = it2;
            temp["to"]=it.second->states[it2]->name;
            transitions.push_back(temp);
        }
    }
    j["transitions"]=transitions;
    return j;
}

string DFA::ToRe() const{
    json data = getJson();
    DFAE temp;
    temp.load(data);
    REE r = temp.toREE();
    return r.getRegex();
}

DFA* DFA::complement() {
    vector<state*> new_states;
    for (const auto &s: states){
        state* new_state = s.second->getComplement();
        new_states.push_back(new_state);
    }
    json j = getJson();

    vector<json> States;
    for(const auto &it : new_states){
        json temp;
        temp["name"]=it->name;
        temp["starting"]=it->starting;
        temp["accepting"]=it->accepting;
        States.push_back(temp);
    }

    j["states"]=States;
    DFA* d =  new DFA();
    d->load(j);
    return d;
}

ENFA DFA::reverse() {
    vector<state*> new_states;
    map<string, set<pair<state*, string>>> reverse_link_map;
    map<string, state*> name_to_state_map;
    for (const auto &it: states){
        state* s = it.second;
        auto new_state(new state());
        new_state->name = s->name;
        new_state->accepting = s->starting;
        new_state->starting = false;

        for (const auto &entry: s->states){
            string transition_char = entry.first;
            state* target = entry.second;
            reverse_link_map[target->name].insert(make_pair(new_state, transition_char));
            name_to_state_map[new_state->name] = new_state;
        }
        new_states.push_back(new_state);
    }

    for (const auto &entry: reverse_link_map){
        state* state_go = name_to_state_map.at(entry.first);
        for (auto &p: entry.second){
            state* state_target = p.first;
            string transition_char = p.second;

            state_go->addTransitionFunctionENFA(transition_char, state_target);
        }
    }

    auto new_start(new state());
    new_start->name = "reversed_start";
    new_start->starting = true;
    new_start->accepting = false;
    for (const auto &s: states){
        if (s.second->accepting){
            state* target = name_to_state_map.at(s.first);
            new_start->addTransitionFunctionENFA("*", target);
        }
    }

    new_states.push_back(new_start);

    vector<json> States;
    for(const auto &it: new_states){
        json temp;
        temp["name"]=it->name;
        temp["starting"]=it->starting;
        temp["accepting"]=it->accepting;
        States.push_back(temp);
    }
    json j = getJson();
    j["states"] = States;

    set<string> alphabet2 = alphabet;
    alphabet2.insert("*");

    vector<json> transitions;
    for(const auto &it: new_states){
        for(const auto &it2: alphabet2){

            if (it->statesENFA.find(it2) != it->statesENFA.end()){
                for(auto target: it->statesENFA.at(it2)){
                    json temp;
                    temp["from"]=it->name;
                    temp["input"] = it2;
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

void DFA::load( set<string> &alfa, map<string,state*> &States, state*start_state,
                vector<state*> &end_states) {
    alphabet = alfa;
    states = States;
    startingState = start_state;
    endstates = end_states;
}

void DFA::AddState(state*k) {
    states[k->name]=k;
    if(k->accepting){
        endstates.push_back(k);
    }

    if(k->starting){
        startingState=k;
    }
}

vector<vector<DFA*>> DFA::split(int split_size) {
    state* current_start;
    set<state*> o;
    set<state*> e;
    set<state*> l = {};
    set<state*> new_l = {getStartingState()};
    vector<vector<DFA*>> d_list;

    do {
        l = new_l;
        vector<DFA*> d_sub_list;
        new_l.clear();

        do {
            set<string> names;
            current_start = *l.begin();
            l.erase(current_start);
            o.clear();
            e.clear();
            inRange(split_size, o, new_l, e, current_start);
            map<state *, state *> state_link;
            for (auto a: o) {
                auto w = new state();
                w->name = a->name;
                state_link[a] = w;
            }
            for (auto a: e) {
                state_link[a] = new state;
            }


            map<string,state *> out;
            vector<state *> end;
            state *out_start;

            o.insert(current_start);
            for (auto s: o) {
                /*
                if (names.find(s->name) != names.end()) {
                    continue;
                }*/
                state *temp = state_link[s];
                if (s == current_start) {
                    temp->starting = true;
                    out_start = temp;
                } else {
                    temp->starting = false;
                }
                temp->name = s->name;
                temp->accepting = false;

                for (const auto &transities: s->states) {
                    auto t_state = transities.second;
                    if (o.find(t_state) != o.end() || e.find(t_state) != e.end()) {
                        if (state_link.find(t_state) == state_link.end()){
                            cout << "he" << endl;
                        }
                        temp->addTransitionFunction(transities.first, state_link[t_state]);
                    }
                }
                names.insert(temp->name);
                out[temp->name]=temp;
            }
            for (auto s: e) {
                if (names.find(s->name) != names.end()) {
                    continue;
                }

                state *temp = state_link[s];
                if (s == current_start) {
                    temp->starting = true;
                    out_start = temp;
                } else {
                    temp->starting = false;
                }
                temp->name = s->name;
                temp->accepting = true;

                for (const auto &transities: s->states) {
                    auto t_state = transities.second;
                    if (o.find(t_state) != o.end() || e.find(t_state) != e.end()) {
                        if (state_link.find(t_state) == state_link.end()){
                            cout << "he" << endl;
                        }
                        temp->addTransitionFunction(transities.first, state_link[t_state]);
                    }
                }
                names.insert(temp->name);
                out[temp->name]=temp;
                end.push_back(temp);
            }
            DFA *d = new DFA();
            d->setAlphabet(alphabet);
            d->setStates(out);
            d->setStartingState(out_start);
            d->setEndstates(end);
            d_sub_list.push_back(d);

        } while (!l.empty());
        d_list.push_back(d_sub_list);
    }while(!new_l.empty());
    return d_list;
}

void DFA::inRange(int range, set<state*> &out,set<state*>& last, set<state*>& end, state*current) {

    if (range == -1){
        if (current->name != "({},{})"){
            last.insert(current);
        }

        return;
    }else if(range == 0){
        if (current->name != "({},{})"){
            end.insert(current);
            last.insert(current);
        }else{
            out.insert(current);
        }
        return;
    }else{
        out.insert(current);
    }

    for(const auto &target: current->states){
        auto s_target = target.second;
        if (out.find(s_target) == out.end()){
            inRange(range-1, out, last, end, s_target);
        }

    }
}

DFA::~DFA() {
    for(auto &k:states){
        delete k.second;
    }
}

const map<string, state *> &DFA::getStates() const {
    return states;
}

void DFA::setStates(const map<string, state *> &States) {
    DFA::states = States;
}

void DFA::ExtendAlphabet(set<string> &k) {
    for(auto &s: k){
        this->alphabet.insert(s);
    }
}