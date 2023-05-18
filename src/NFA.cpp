//
// Created by watson on 4/12/23.
//

#include "NFA.h"

NFA::NFA(const string &inputfile) {
    // inlezen uit file
    ifstream input(inputfile);
    json j;
    input >> j;

    //Parse Alphabet
    Alphabet = j["alphabet"].get<std::vector<string>>();

    //Parse States
    auto states = j["states"];
    for(int i = 0 ; i<states.size() ; i++){
        string toestand = states[i]["name"];
        bool start = states[i]["starting"];
        bool accept = states[i]["accepting"];

        State* temp = new State(toestand,start,accept);
        Q.insert({toestand, temp});
    }

    //Parse Delta
    auto transitions = j["transitions"];
    for(int i = 0 ; i<transitions.size() ; i++) {
        string from = transitions[i]["from"];
        string to = transitions[i]["to"];
        string in = transitions[i]["input"];
        char input = in[0];

        Q.find(from)->second->AddTransition(input, to );
    }
}

NFA::~NFA() {
    for(auto it = Q.begin(); it != Q.end() ; it++){
        delete it->second;
    }
}

DFA NFA::toDFA() const {
    map<string , vector<pair<char,string>> > DFA_Q; // collect alle states //statename, {inputsymbol, reachablestates in stringform} // bv: ["{q1,q2"} , { c, {q3,q4} }]
    queue<string> states_todo;
    map<string,int> counts; //Keeps how many times a string been evaluated

    //Find Start State and add it to Q
    State* start;
    string startname="";
    for(auto itr = this->Q.begin(); itr!=this->Q.end() ; itr++){
        if(itr->second->getStarting()){
            State* start = itr->second;
            vector<string> temp = {itr->first};
            string name = NameConvert( temp );
            startname = name;
            DFA_Q.insert( {name , FindReachableStates(name)  } ); //Insert Start
            //DFA_Q.insert( { "{}" , FindReachableStates("{}") } ); //Insert Empty State
            states_todo.push(name);
            counts.insert({name,0});
            break;
        }
    }

    //Lazy Evaluation from start State
    while(!states_todo.empty()){
        vector<pair<char,string>> options = DFA_Q.find(states_todo.front())->second;
        for(int i = 0 ; i<options.size() ; i++) {
            string name = options[i].second;
            if(DFA_Q.find(name)!=DFA_Q.end()){
                if(counts.find(name)->second>0){
                    break;
                }else{
                    counts.find(name)->second++;
                }
            }
            DFA_Q.insert({name, FindReachableStates(name)});
            states_todo.push(name);
            counts.insert( {name,0});
        }
        states_todo.pop();
    }

    //Create Object
    nlohmann::json dfa;
    dfa["type"] = "DFA";
    dfa["alphabet"] = Alphabet;

    //Set states
    int count = 0;
    int count2 = 0;
    for(auto &j: DFA_Q){

        //Check Starting & Accepting Bools
        bool starting = false;
        bool accepting = false;
        vector<State*> temp = StringToState(j.first);
        for(State* k: temp){
            if(k->getAnEnd()){
                accepting = true;
            }
        }
        if(j.first==startname){
            starting = true;
        }

        //Set Data
        string name = j.first;
        dfa["states"][count2]["name"] = name;
        dfa["states"][count2]["starting"] = starting;
        dfa["states"][count2]["accepting"] = accepting;


        vector<pair<char,string>> transitions = j.second;
        //Set transitions (voor elke state)
        for(int i = 0; i<transitions.size() ; i++){
            string temp = j.first;

            dfa["transitions"][count]["from"]= SortName(temp);
            string z = SortName(transitions[i].second);

            dfa["transitions"][count]["to"]= SortName(transitions[i].second);
            temp ="";
            temp += transitions[i].first;
            dfa["transitions"][count]["input"]= temp;
            count++;

        }
        count2++;
    }

    //Output the file
    /*
    std::ofstream file("DFA.json");
    file << dfa;
    file.close();
     */

    return DFA(dfa);
}

vector<State *> NFA::StringToState(const string &a) const {
    string tempstring;
    vector<State*> states;

    for(char k:a){
        if( (k=='{') ){
            continue;
        }else if( (k==',') || (k=='}') ){
            states.push_back(Q.find(tempstring)->second);
            tempstring = ""; //Reset temp
        } else{
            tempstring+=k;
        }
    }

    return states;
}

vector<pair<char, string>> NFA::FindReachableStates(const string &start) const {
    vector<State*> current = StringToState(start); //Alle Staten
    vector<pair<char, string>> ReachableStates;

    for(State* state: current){ //Voor elke staat
        for(string s: Alphabet){ //Collect alle reachable states bij deze input
            char k = s[0]; //Conversion to char
            vector<string> Reachable = state->DoTransition(k);
            string temp = NameConvert(Reachable); //Bereikbare staten in string vorm
            pair<char,string> bereikbaar = {k,temp}; //In Pair vorm
            bool check = false;

            for(int i = 0; i<ReachableStates.size(); i++){
                auto s = ReachableStates[i];
                if(s.first==k){

                    //Sort & Merge
                    ReachableStates[i].second=SortNames(temp,s.second);
                    check = true;
                }
            }

            if(!check){
                ReachableStates.push_back(bereikbaar);
            }
        }
    }
    return ReachableStates;
}

bool NFA::Found(vector<pair<string, vector<pair<char, string>>>> &processed, string &name) const {
    for(auto &z: processed){
        if(z.first==name){
            return true;
        }
    }
    return false;
}

string NFA::SortNames(string &a, string &b) const {
    vector<State*> aNaked = StringToState(a);
    vector<State*> bNaked = StringToState(b);
    vector<State*> Full = MergeVectors(aNaked,bNaked);
    vector<string> temp;
    for(auto k: Full){
        temp.push_back(k->getName());
    }
    std::sort(temp.begin(), temp.end());
    string newName = NameConvert(temp);
    return newName;
}

string NFA::SortName(string &a) const {
    vector<State*> Full = StringToState(a);
    vector<string> temp;
    for(auto k: Full){
        temp.push_back(k->getName());
    }
    std::sort(temp.begin(), temp.end());
    string newName = NameConvert(temp);
    return newName;
}

NFA::NFA(const json &j) {
    Alphabet = j["alphabet"].get<std::vector<string>>();

    //Parse States
    auto states = j["states"];
    for(int i = 0 ; i<states.size() ; i++){
        string toestand = states[i]["name"];
        bool start = states[i]["starting"];
        bool accept = states[i]["accepting"];

        State* temp = new State(toestand,start,accept);
        Q.insert({toestand, temp});
    }

    //Parse Delta
    auto transitions = j["transitions"];
    for(int i = 0 ; i<transitions.size() ; i++) {
        string from = transitions[i]["from"];
        string to = transitions[i]["to"];
        string in = transitions[i]["input"];
        char input = in[0];

        Q.find(from)->second->AddTransition(input, to );
    }

}

WNFA NFA::toWNFA(){

    WNFA result = WNFA();
    result.alfabet = Alphabet;
    State* start_state;
    // maak de staten van de WNFA aan
    for (pair<string, State*> state_pair : Q){
        result.addState(state_pair.first, state_pair.second->getStarting(), state_pair.second->getAnEnd());
        if (state_pair.second->getStarting()){

            start_state = state_pair.second;
        }
    }

    vector<weightedNode*> v = {result.startState};
    adaptDistance(v, start_state, (int) Q.size()-1, 0, -0.2, result);

    // voeg de transities uit de NFA toe aan de WNFA
    for (pair<string, weightedNode*> state : result.states){
        for (string temp : result.alfabet){
            char symbol = temp[0];
            for (const auto& transition : Q.find(state.second->getName())->second->DoTransition(symbol)){
                state.second->addconnection(result.getState(transition).first, symbol, 1);
            }
        }
    }

    // voeg nieuwe transities toe die enkel in de WNFA aanwezig zijn
    /*
    for (string temp : result.alfabet) {
        char symbol = temp[0];

        result.startState->addconnection(result.startState, symbol, 0);

        for (weightedNode *endstate: result.endStates) {
            endstate->addconnection(endstate, symbol, 0);
        }

        for (weightedNode *firststate: result.states) {
            for (weightedNode *secondstate: result.states) {
                firststate->addconnection(secondstate, symbol, 0);
            }
        }
    }*/
    return result;
}

void NFA::adaptDistance(vector<weightedNode*>& original, State* s, int distance, int index, double weight, const WNFA& result){
    map<const char, vector<string>> transition_map = s->getTransition();
    for (auto entry: transition_map){
        vector<State*> current_states;
        for (auto s: entry.second){
            State* st = Q.find(s)->second;

            weightedNode* w = (result.getState(st->getName())).first;

                for (int i=0; i < original.size(); i++){
                    weightedNode* o = original[i];
                    if (index-i > 0){
                        o->addconnection(w, entry.first, (index-i)*weight+1);
                    }

                    o->addconnection(o, entry.first, 0);

                }

                original.push_back(w);
                //cout << w->name << " "<< distance << endl;

            if (distance > 1){
                adaptDistance(original, st, distance - 1, index+1, weight, result);
                //adaptDistance(result.getWeightedState(st->getName()).first, st, distance - 1, 0, weight, result);

            }
        }
    }
}
