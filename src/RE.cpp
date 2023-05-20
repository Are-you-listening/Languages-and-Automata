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

ENFA RE::plus(ENFA& enfa1, ENFA& enfa2) {
    ENFA enfa=ENFA();
    state* temp1(new state(to_string(tempcounter.count) , true, false));
    tempcounter.count++;
    state* temp2(new state(to_string(tempcounter.count) , false , true));
    tempcounter.count++;

    enfa.startingState=temp1;
    enfa.endstates={temp1};
    enfa.eps=RE::epsilon;
    enfa.states={temp1};
    state* temp=(*enfa.endstates.begin());
    enfa.concatenate(enfa1);

    enfa.states.insert(enfa.states.end(),enfa2.states.begin(),enfa2.states.end());
    enfa.endstates.push_back((*enfa2.endstates.begin()));
    enfa.alphabet.insert(enfa2.alphabet.begin(),enfa2.alphabet.end());
    enfa2.startingState->starting= false;
    temp->addTransitionFunction(RE::epsilon,enfa2.startingState);

    enfa.endstates[0]->accepting= false;
    enfa.endstates[1]->accepting= false;
    enfa.endstates[0]->addTransitionFunction(RE::epsilon,temp2);
    enfa.endstates[1]->addTransitionFunction(RE::epsilon,temp2);

    enfa.endstates={temp2};
    enfa1.startingState->starting= false;
    enfa.states.push_back(temp2);
    return enfa;
}

ENFA RE::onechar(const string& c) {
    state* temp1(new state(to_string(tempcounter.count) , true, false));
    tempcounter.count++;
    state* temp2(new state(to_string(tempcounter.count) , false , true));
    tempcounter.count++;
    temp1->addTransitionFunction(c,temp2);

    return ENFA(temp1 ,{temp1,temp2} , {c} , {temp2}, epsilon);
}

void RE::kleene(ENFA &enfa) {
    state* temp1(new state());
    state* temp2(new state());
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

void RE::concatenation(ENFA &enfa1, ENFA &enfa2) {
    enfa1.concatenate(enfa2);
}

ENFA RE::toENFA()& {
    ENFA enfa;
    enfa.startingState = nullptr;
    int count=0;
    for(string::const_iterator it=re.begin(); it!=re.end(); it++,count++){
        if ((*it)=='+'){
            RE regex(re.substr(count+1),(*RE::epsilon.begin()));
            ENFA temp= regex.toENFA();
            temp.deletable = false;
            enfa = RE::plus(enfa,temp);
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
                temp.deletable = false;
                RE::kleene(temp);
                if (enfa.startingState == nullptr){
                    enfa=temp;
                } else {
                    enfa.concatenate(temp);
                }
            } else {
                RE regex(temp_string,(*RE::epsilon.begin()));
                ENFA temp= regex.toENFA();
                temp.deletable = false;
                if (enfa.startingState == nullptr){
                    enfa=temp;
                } else {
                    enfa.concatenate(temp);
                }
            }
        } else if(enfa.startingState == nullptr) {
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
                ENFA temp= RE::onechar(s.substr(0,1));
                kleene(temp);
                enfa.concatenate(temp);
                it++;
                count++;
            } else {
                string s= &(*it);
                enfa.concatenate(onechar(s.substr(0,1)));
            }
        }
        if ((*it)=='\000'){
            break;
        }
    }

    enfa.deletable = true;
    return enfa;
}

RE::~RE() {

}
