//
// Created by anass on 1-5-2023.
//

#include "DFA.h"

DFA::DFA(const string& c) {
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

DFA::DFA() {

}

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
    while (states.find(cCopy)!=states.end()){
        cCopy = *ptr;
        cCopy+=to_string(count);
        count++;
    }
    state::states[cCopy]=q;
}

DFA::DFA(DFA& dfa1, DFA& dfa2, bool c) {
    bool a= true;
    state* startstate = new state();
    startstate->starting= true;
    DFA::alphabet=dfa1.alphabet;
    DFA::alphabet.insert(dfa2.alphabet.begin(),dfa2.alphabet.end());
    startstate->name="("+dfa1.startingState->name+","+dfa2.startingState->name+")";
    if(dfa1.startingState->accepting && dfa2.startingState->accepting){
        startstate->accepting= true;
    }
    DFA::states.push_back(startstate);
    DFA::startingState=startstate;
    while (a){
        a=false;
        vector<state*> TOBEADDED;
        vector<state*> TOBEADDEDEND;
        for(vector<state*>::const_iterator it=DFA::states.begin(); it!=DFA::states.end(); it++){
            for(set<string>::const_iterator it2=DFA::alphabet.begin(); it2!=DFA::alphabet.end(); it2++){
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
                state* temp;
                for(vector<state*>::const_iterator it3=dfa1.states.begin(); it3!=dfa1.states.end(); it3++){
                    if ((*it3)->name==state1_string){
                        state1 = (*it3);
                    }
                }
                for(vector<state*>::const_iterator it3=dfa2.states.begin(); it3!=dfa2.states.end(); it3++){
                    if ((*it3)->name==state2_string){
                        state2 = (*it3);
                    }
                }
                bool b2=false;
                string name="("+state1->states[(*it2)]->name+","+state2->states[(*it2)]->name+")";
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
                        bool b3=true;
                        for(vector<state*>::const_iterator it3=TOBEADDEDEND.begin(); it3!=TOBEADDEDEND.end(); it3++){
                            if (name==(*it3)->name){
                                b3= false;
                            }
                        }
                        if(b3){
                            TOBEADDEDEND.push_back(temp);
                        }
                    }
                    bool b4=true;
                    for(vector<state*>::const_iterator it3=TOBEADDED.begin(); it3!=TOBEADDED.end(); it3++){
                        if (name==(*it3)->name){
                            b4= false;
                        }
                    }
                    if(b4){
                        TOBEADDED.push_back(temp);
                    }
                }
                if ((*it)->states.find((*it2))==(*it)->states.end()){
                    (*it)->addTransitionFunction((*it2),temp);
                }
            }
        }
        for(vector<state*>::const_iterator it=TOBEADDED.begin(); it!=TOBEADDED.end(); it++){
            if (find(DFA::states.begin(),DFA::states.end(),(*it))==DFA::states.end()){
                DFA::states.push_back((*it));
                a=true;
            }
        }
        for(vector<state*>::const_iterator it=TOBEADDEDEND.begin(); it!=TOBEADDEDEND.end(); it++){
            if (find(DFA::endstates.begin(),DFA::endstates.end(),(*it))==DFA::endstates.end()) {
                DFA::endstates.push_back((*it));
            }
        }
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

