//
// Created by anass on 1-5-2023.
//

#include "RE.h"

struct number{
    unsigned int count=0;
};
number tempcounter;

RE::RE(const string& re, const char epsilon) {
    RE::re=re;
    RE::epsilon=epsilon;
}

ENFA * RE::plus(ENFA& enfa1, ENFA& enfa2) {
    ENFA* enfa=new ENFA();
    state* temp1=new state();
    state* temp2=new state();
    temp1->starting= true;
    temp1->accepting= false;
    temp1->name= to_string(tempcounter.count);
    tempcounter.count++;
    enfa->startingState=temp1;
    vector<state*> endstates={temp1};
    enfa->endstates=endstates;
    enfa->eps=RE::epsilon;
    vector<state*> states={temp1};
    enfa->states=states;
    state* temp=(*enfa->endstates.begin());
    enfa->concatenate(enfa1);

    enfa->states.insert(enfa->states.end(),enfa2.states.begin(),enfa2.states.end());
    enfa->endstates.push_back((*enfa2.endstates.begin()));
    enfa->alphabet.insert(enfa2.alphabet.begin(),enfa2.alphabet.end());
    enfa2.startingState->starting= false;
    temp->addTransitionFunction(RE::epsilon,enfa2.startingState);

    temp2->starting= false;
    temp2->accepting=true;
    temp2->name=to_string(tempcounter.count);
    tempcounter.count++;
    enfa->endstates[0]->accepting= false;
    enfa->endstates[1]->accepting= false;
    enfa->endstates[0]->addTransitionFunction(RE::epsilon,temp2);
    enfa->endstates[1]->addTransitionFunction(RE::epsilon,temp2);
    endstates={temp2};
    enfa->endstates=endstates;
    enfa1.startingState->starting= false;
    enfa->states.push_back(temp2);
    return enfa;
}

ENFA * RE::onechar(const string& c) {
    ENFA* enfa=new ENFA();
    state* temp1=new state();
    state* temp2=new state();
    temp1->starting= true;
    temp1->accepting= false;
    temp1->name=to_string(tempcounter.count);
    tempcounter.count++;
    temp2->starting= false;
    temp2->accepting=true;
    temp2->name=to_string(tempcounter.count);
    tempcounter.count++;
    temp1->addTransitionFunction(c,temp2);
    enfa->startingState=temp1;
    vector<state*> endstates={temp2};
    enfa->endstates=endstates;
    enfa->eps=RE::epsilon;
    vector<state*> states={temp1,temp2};
    enfa->states=states;
    set<string> alpahabet={c};
    enfa->alphabet=alpahabet;
    return enfa;
}

void RE::kleene(ENFA& enfa) {
    state* temp1=new state();
    state* temp2=new state();
    temp1->starting= true;
    temp1->accepting= false;
    temp1->name=to_string(tempcounter.count);
    tempcounter.count++;
    temp2->starting= false;
    temp2->accepting=true;
    temp2->name=to_string(tempcounter.count);
    tempcounter.count++;
    temp1->addTransitionFunction(RE::epsilon,enfa.startingState);
    temp1->addTransitionFunction(RE::epsilon,temp2);
    enfa.endstates[0]->accepting= false;
    enfa.endstates[0]->addTransitionFunction(RE::epsilon,temp2);
    enfa.endstates[0]->addTransitionFunction(RE::epsilon,enfa.startingState);
    vector<state*> endstates={temp2};
    enfa.endstates=(endstates);
    enfa.startingState->starting= false;
    enfa.startingState=temp1;
    enfa.states.push_back(temp1);
    enfa.states.push_back(temp2);

}

void RE::concatenation(ENFA& enfa1, ENFA& enfa2)& {
    enfa1.concatenate(enfa2);
}

ENFA RE::toENFA()& {
    ENFA* enfa= nullptr;
    int count=0;
    for(string::const_iterator it=re.begin(); it!=re.end(); it++,count++){
        if ((*it)=='+'){
            RE regex(re.substr(count+1),(*RE::epsilon.begin()));
            ENFA temp= regex.toENFA();
            enfa = RE::plus(*enfa,temp);
            break;
        } else if ((*it)=='('){
            string temp_string = "";
            it++;
            count++;
            int count2=1;
            while (count2!=0){
                if ((*it)=='('){
                    count2++;
                } else if ((*it)==')'){
                    count2--;
                    if(count2==0){
                        break;
                    }
                }
                temp_string.push_back((*it));
                it++;
                count++;
            }
            if ((*(re.begin()+count+1))=='*'){
                RE regex(temp_string,(*RE::epsilon.begin()));
                ENFA temp= regex.toENFA();
                RE::kleene(temp);
                if (enfa== nullptr){
                    ENFA* enfa_temp=new ENFA();
                    enfa_temp->endstates=temp.endstates;
                    enfa_temp->states=temp.states;
                    enfa_temp->startingState=temp.startingState;
                    enfa_temp->alphabet=temp.alphabet;
                    enfa_temp->eps=temp.eps;
                    enfa=enfa_temp;
                } else {
                    enfa->concatenate(temp);
                }
            } else {
                RE regex(temp_string,(*RE::epsilon.begin()));
                ENFA temp= regex.toENFA();
                if (enfa== nullptr){
                    ENFA* enfa_temp=new ENFA();
                    enfa_temp->endstates=temp.endstates;
                    enfa_temp->states=temp.states;
                    enfa_temp->startingState=temp.startingState;
                    enfa_temp->alphabet=temp.alphabet;
                    enfa_temp->eps=temp.eps;
                    enfa=enfa_temp;
                } else {
                    enfa->concatenate(temp);
                }
            }
        } else if(enfa== nullptr) {
            string s= &(*it);
            enfa=RE::onechar(s.substr(0,1));
        } else if((*it)=='*') {
            continue;
        } else {
            if ((*it)=='*' || (*it)==')'){
                break;
            }
            if ((*(re.begin()+count+1))=='*'){
                string s= &(*it);
                ENFA temp;
                temp= *RE::onechar(s.substr(0,1));
                RE::kleene(temp);
                enfa->concatenate(temp);
                it++;
                count++;
            } else {
                string s= &(*it);
                enfa->concatenate(*RE::onechar(s.substr(0,1)));
            }
        }
        if ((*it)=='\000'){
            break;
        }
    }
    return *enfa;
}