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
        for(vector<string>::const_iterator it=a.begin();it!=a.end();it++){
            ENFA::alphabet.insert((*it));
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
    int count=0;
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
    vector<json> states;
    for(vector<state*>::const_iterator it=ENFA::states.begin(); it!=ENFA::states.end(); it++){
        json temp;
        temp["name"]=(*it)->name;
        temp["starting"]=(*it)->starting;
        temp["accepting"]=(*it)->accepting;
        states.push_back(temp);
    }
    j["states"]=states;
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

vector<state *> ENFA::ECLOSE(state *Etransition)const&{
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

vector<state *> ENFA::ECLOSE2(state *Etransition, string input)const&{
    vector<state *> EStates;
    for (map<string, state *>::const_iterator it = Etransition->states.begin(); it != Etransition->states.end(); it++) {
        if ((*it->first.begin()) == (*ENFA::eps.begin())) {
            vector<state *> temp = ECLOSE2(Etransition->states[ENFA::eps], input);
            EStates.insert(EStates.end(), temp.begin(), temp.end());
        } else {
            if (Etransition->states.find(input) != Etransition->states.end()) {
                if (find(EStates.begin(), EStates.end(), Etransition->states[input]) == EStates.end()) {
                    EStates.push_back(Etransition->states[input]);
                }
            }
        }
        for (int i = 0; Etransition->states.find(input + to_string(i)) != Etransition->states.end(); i++) {
            state *temp = Etransition->states[input + to_string(i)];
            if (find(EStates.begin(), EStates.end(), temp) == EStates.end()) {
                EStates.push_back(temp);
            }
        }
    }
    return EStates;
}

void ENFA::Etransitions(vector<state*> &states)const&{
    for(vector<state*>::const_iterator it=states.begin(); it!=states.end(); it++){
        bool a=false;
        vector<state*> states2= ECLOSE((*it));
        for(vector<state*>::const_iterator it2=states2.begin(); it2!=states2.end(); it2++){
            if(find(states.begin(),states.end(),(*it2))==states.end()){
                states.push_back((*it2));
                a=true;
            }
        }
        if (a){
            Etransitions(states);
            it=states.begin();
        }
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

DFA ENFA::toDFA() &{
    map<char,string> dict;
    int count=0;
    for(vector<state*>::const_iterator state=ENFA::states.begin(); state!=ENFA::states.end(); state++, count++){
        char temp=count;
        while (temp=='{' || temp==',' || temp=='}'){
            temp=count;
            count++;
        }
        dict[temp]=(*state)->name;
        (*state)->name=temp;
    }
    DFA dfa = ENFA::toDFA2();
    vector<state*> tempSTATES = dfa.getStates();
    for(vector<state*>::const_iterator state=tempSTATES.begin(); state!=tempSTATES.end(); state++){
        vector<string> names;
        if ((*state)->name=="{}"){
            continue;
        }
        for(string::const_iterator name=(*state)->name.begin(); name!=(*state)->name.end(); name++){
            if ((*name)=='{' || (*name)==',' || (*name)=='}'){
                continue;
            }
            names.push_back(dict[(*name)]);
        }
        std::sort(names.begin(), names.end());
        string s="{";
        for(vector<string>::const_iterator name=names.begin(); name!=names.end();){
            s+=*name;
            name++;
            if(name!=names.end()){
                s+=",";
            } else {
                s+="}";
            }
        }
        (*state)->name=s;
    }
    return dfa;
}

DFA ENFA::toDFA2() &{
    DFA* dfa=new DFA();
    vector<map<pair<char,string>, vector<string>>> ENFAtoDFA;
    vector<vector<map<pair<char,string>, vector<string>>>> ENFAtoDFADONE;
    vector<string> notExistingStates;
    vector<state*> ECLOSURE = ENFA::ECLOSE(startingState);
    if(find(ECLOSURE.begin(),ECLOSURE.end(),startingState)==ECLOSURE.end()){
        ECLOSURE.push_back(startingState);
    }
    ENFA::Etransitions(ECLOSURE);
    string startingstatename;
    for (vector<state*>::const_iterator state=ECLOSURE.begin(); state!=ECLOSURE.end(); state++){
        startingstatename+=(*state)->name;
    }
    for (vector<state*>::const_iterator state=ECLOSURE.begin(); state!=ECLOSURE.end(); state++){
        map<pair<char,string>, vector<string>> states2 = ENFA::possibleStates((*state),startingstatename);
        ENFAtoDFA.push_back(states2);
    }
    vector<string> lazyEval;
    state* emptyset = new state();
    emptyset->name = "{}";
    emptyset->accepting = false;
    emptyset->starting = false;

    map<pair<char,string>,vector<string>> ToBeCombined;
    for(vector<map<pair<char,string>, vector<string>>>::iterator it=ENFAtoDFA.begin(); it!=ENFAtoDFA.end(); it++){
        for(map<pair<char,string>, vector<string>>::iterator it2=it->begin(); it2!=it->end(); it2++){
            if(ToBeCombined.find(make_pair(it2->first.first,it2->first.second))==ToBeCombined.end()){
                ToBeCombined[make_pair(it2->first.first,it2->first.second)]=it2->second;
            } else {
                for (vector<string>::iterator it3=it2->second.begin(); it3!=it2->second.end(); it3++){
                    if (find(ToBeCombined[make_pair(it2->first.first,it2->first.second)].begin(),ToBeCombined[make_pair(it2->first.first,it2->first.second)].end(),(*it3))==ToBeCombined[make_pair(it2->first.first,it2->first.second)].end()){
                        ToBeCombined[make_pair(it2->first.first,it2->first.second)].push_back((*it3));
                    }
                }
            }
        }
    }
    for(vector<map<pair<char,string>, vector<string>>>::iterator it=ENFAtoDFA.begin(); it!=ENFAtoDFA.end(); it++){
        for(map<pair<char,string>, vector<string>>::iterator it2=it->begin(); it2!=it->end(); it2++){
            (*it)[it2->first]=ToBeCombined[make_pair(it2->first.first,it2->first.second)];
        }
    }

    ENFAtoDFADONE.push_back(ENFAtoDFA);
    int count = 0;
    while(count!=ENFAtoDFADONE.size()){
        for(vector<map<pair<char,string>, vector<string>>>::iterator it=ENFAtoDFADONE[count].begin(); it!=ENFAtoDFADONE[count].end(); it++){
            vector<map<pair<char,string>, vector<string>>> ENFAtoDFA2;
            for(map<pair<char,string>, vector<string>>::iterator it2=it->begin(); it2!=it->end(); it2++){
                string tempStateName;
                for(vector<string>::iterator it3=it2->second.begin(); it3!=it2->second.end(); it3++){
                    tempStateName+=(*it3);
                }
                sort(tempStateName.begin(), tempStateName.end());
                if(find(lazyEval.begin(),lazyEval.end(),tempStateName) == lazyEval.end()) {
                    for(vector<string>::iterator it3=it2->second.begin(); it3!=it2->second.end(); it3++){
                        state* tempState;
                        for(vector<state*>::iterator it4=ENFA::states.begin(); it4!=ENFA::states.end(); it4++){
                            if ((*it4)->name==(*it3)){
                                tempState=(*it4);
                            }
                        }
                        map<pair<char,string>, vector<string>> temp = ENFA::possibleStates(tempState,tempStateName);
                        if(temp.empty()){
                            notExistingStates.push_back(tempStateName);
                        }
                        ENFAtoDFA2.push_back(temp);
                    }
                }
                lazyEval.push_back(tempStateName);
            }
            map<pair<char,string>,vector<string>> ToBeCombined2;
            for(vector<map<pair<char,string>, vector<string>>>::iterator it=ENFAtoDFA2.begin(); it!=ENFAtoDFA2.end(); it++){
                for(map<pair<char,string>, vector<string>>::iterator it2=it->begin(); it2!=it->end(); it2++){
                    if(ToBeCombined2.find(make_pair(it2->first.first,it2->first.second))==ToBeCombined2.end()){
                        ToBeCombined2[make_pair(it2->first.first,it2->first.second)]=it2->second;
                    } else {
                        for (vector<string>::iterator it3=it2->second.begin(); it3!=it2->second.end(); it3++){
                            if (find(ToBeCombined2[make_pair(it2->first.first,it2->first.second)].begin(),ToBeCombined2[make_pair(it2->first.first,it2->first.second)].end(),(*it3))==ToBeCombined2[make_pair(it2->first.first,it2->first.second)].end()){
                                ToBeCombined2[make_pair(it2->first.first,it2->first.second)].push_back((*it3));
                            }
                        }
                    }
                }
            }
            for(vector<map<pair<char,string>, vector<string>>>::iterator it=ENFAtoDFA2.begin(); it!=ENFAtoDFA2.end(); it++){
                for(map<pair<char,string>, vector<string>>::iterator it2=it->begin(); it2!=it->end(); it2++){
                    (*it)[it2->first]=ToBeCombined2[make_pair(it2->first.first,it2->first.second)];
                }
            }
            ENFAtoDFADONE.push_back(ENFAtoDFA2);
        }
        count++;
    }
    vector<string> endstates;
    vector<state*> dfaStates;
    vector<state*> dfaEndStates;
    vector<string> dfaStateNames;
    for(vector<string>::const_iterator state=notExistingStates.begin(); state!=notExistingStates.end(); state++){
        bool a= true;
        for(vector<vector<map<pair<char,string>, vector<string>>>>::const_iterator it=ENFAtoDFADONE.begin(); it!=ENFAtoDFADONE.end(); it++){
            for (vector<map<pair<char,string>, vector<string>>>::const_iterator it2=it->begin(); it2!=it->end(); it2++){
                for(map<pair<char,string>, vector<string>>::const_iterator it3=it2->begin(); it3!=it2->end(); it3++){
                    if(it3->first.second==(*state)){
                        a= false;
                    }
                }
            }
        }
        if (a){
            for(set<string>::const_iterator char1=ENFA::alphabet.begin(); char1!=ENFA::alphabet.end(); char1++){
                vector<string> v={"{}"};
                ENFAtoDFADONE[0][0][make_pair((*(*char1).begin()),(*state))]=v;
                if (find(dfaStates.begin(),dfaStates.end(),emptyset)==dfaStates.end()){
                    dfaStates.push_back(emptyset);
                }
            }
        }
    }
    for(vector<state*>::const_iterator it=ENFA::endstates.begin(); it!=ENFA::endstates.end(); it++){
        endstates.push_back((*it)->name);
    }
    for(vector<vector<map<pair<char,string>, vector<string>>>>::const_iterator it=ENFAtoDFADONE.begin(); it!=ENFAtoDFADONE.end(); it++){
        for (vector<map<pair<char,string>, vector<string>>>::const_iterator it2=it->begin(); it2!=it->end(); it2++){
            for(map<pair<char,string>, vector<string>>::const_iterator it3=it2->begin(); it3!=it2->end(); it3++){
                string name="{";
                for(string::const_iterator it4=it3->first.second.begin(); it4!=it3->first.second.end(); it4++){
                    if(it4==it3->first.second.end()-1){
                        string temp_string(1,(*it4));
                        name+=temp_string;
                    } else {
                        string temp_string(1,(*it4));
                        name+=temp_string+",";
                    }
                }
                name+="}";
                if(find(dfaStateNames.begin(),dfaStateNames.end(),name)==dfaStateNames.end()){
                    state* temp=new state();
                    temp->name = name;
                    for(vector<string>::const_iterator it4=endstates.begin(); it4!=endstates.end(); it4++) {
                        if (it3->first.second.find((*it4)) != string::npos) {
                            temp->accepting = true;
                            if(find(dfaEndStates.begin(),dfaEndStates.end(),temp)==dfaEndStates.end()){
                                dfaEndStates.push_back(temp);
                            }
                        }
                    }
                    string startingname="{";
                    for(string::const_iterator it4=startingstatename.begin(); it4!=startingstatename.end(); it4++){
                        if(it4==startingstatename.end()-1){
                            string temp_string(1,(*it4));
                            startingname+=temp_string;
                        } else {
                            string temp_string(1,(*it4));
                            startingname+=temp_string+",";
                        }
                    }
                    startingname+="}";
                    if (temp->name == startingname){
                        temp->starting = true;
                        dfa->setStartingState(temp);
                    }
                    dfaStateNames.push_back(temp->name);
                    dfaStates.push_back(temp);
                }
            }
        }
    }
    for(set<string>::const_iterator char1=ENFA::alphabet.begin(); char1!=ENFA::alphabet.end(); char1++){
        string temp{(*char1)};
        emptyset->addTransitionFunction(temp,emptyset);
    }
    for(vector<vector<map<pair<char,string>, vector<string>>>>::const_iterator it=ENFAtoDFADONE.begin(); it!=ENFAtoDFADONE.end(); it++){
        for (vector<map<pair<char,string>, vector<string>>>::const_iterator it2=it->begin(); it2!=it->end(); it2++){
            for (map<pair<char,string>, vector<string>>::const_iterator it3=it2->begin(); it3!=it2->end(); it3++){
                string name="{";
                for(string::const_iterator it4=it3->first.second.begin(); it4!=it3->first.second.end(); it4++){
                    if(it4==it3->first.second.end()-1){
                        string temp_string(1,(*it4));
                        name+=temp_string;
                    } else {
                        string temp_string(1,(*it4));
                        name+=temp_string+",";
                    }
                }
                name+="}";

                string name2;
                for(vector<string>::const_iterator it4=it3->second.begin(); it4!=it3->second.end(); it4++){
                    name2+=(*it4);
                }
                std::sort(name2.begin(), name2.end());

                string name3="{";
                for(string::const_iterator it4=name2.begin(); it4!=name2.end(); it4++){
                    if(it4==name2.end()-1){
                        string temp_string(1,(*it4));
                        name3+=temp_string;
                    } else {
                        string temp_string(1,(*it4));
                        name3+=temp_string+",";
                    }
                }
                name3+="}";
                for(vector<state*>::const_iterator it5=dfaStates.begin(); it5!=dfaStates.end(); it5++){
                    if((*it5)->name == name){
                        for(vector<state*>::const_iterator it6=dfaStates.begin(); it6!=dfaStates.end(); it6++){
                            if((*it6)->name==name3){
                                string temp_string(1,it3->first.first);
                                if ((*it5)->states.find(temp_string) == (*it5)->states.end()){
                                    (*it5)->addTransitionFunction(temp_string,(*it6));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    for(vector<state*>::const_iterator state=dfaStates.begin(); state!=dfaStates.end(); state++){
        for(set<string>::const_iterator char1=ENFA::alphabet.begin(); char1!=ENFA::alphabet.end(); char1++){
            if((*state)->states.find((*char1)) == (*state)->states.end()){
                (*state)->addTransitionFunction((*char1),emptyset);
            }
        }
    }
    dfa->setAlphabet(ENFA::alphabet);
    dfa->setStates(dfaStates);
    dfa->setEndstates(dfaEndStates);
    return *dfa;
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
    ENFA::states.insert(ENFA::states.end(),enfa.states.begin(),enfa.states.end());
    state* temp=(*ENFA::endstates.begin());
    for(vector<state*>::iterator it=ENFA::endstates.begin(); it!=ENFA::endstates.end(); it++){
        (*it)->accepting= false;
    }
    ENFA::endstates=enfa.endstates;
    ENFA::alphabet.insert(enfa.alphabet.begin(),enfa.alphabet.end());
    enfa.startingState->starting= false;
    temp->addTransitionFunction(ENFA::eps,enfa.startingState);
}



ENFA::ENFA() {

}

void ENFA::load(const json &j) {
    if (j.value("type","string") == "ENFA"){
        ENFA::eps=j["eps"];
        vector<string> a=j["alphabet"];
        for(vector<string>::const_iterator it=a.begin();it!=a.end();it++){
            ENFA::alphabet.insert((*it));
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
