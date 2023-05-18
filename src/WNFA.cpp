//
// Created by emill on 10/05/2023.
//

#include "WNFA.h"

WNFA::WNFA(const string &filename) {
    ifstream input(filename);
    json j;
    input >> j;
    type = j["type"];
    set<string> temp = j["alphabet"];
    alfabet.clear();
    for (const auto& item : temp){
        const string& symbol = item;
        alfabet.push_back(symbol);
    }
    set<json> newstates = j["states"];
    for (auto state : newstates){
        string tempname = state["name"];
        bool tempstart = state["starting"];
        bool tempend = state["accepting"];
        addState(tempname, tempstart, tempend);
    }
    

    set<json> newtransitions = j["transitions"];
    for (auto transition : newtransitions) {
        weightedNode *state1 = states.find(transition["from"])->second;
        weightedNode *state2 = states.find(transition["to"])->second;
        string inputsymbol = transition["input"];
        double weight = transition["weight"];
        for (auto symbol: inputsymbol) {
            state1->addconnection(state2, symbol, weight);
        }
    }
}

WNFA::WNFA() {}

void WNFA::addState(const string& name, bool start, bool endState) {
    auto* n = new weightedNode(name);
    states[name] = n;
    if (start){
        startState = n;
    }
    if (endState){
        endStates[name] = n;
    }
}

/*
double WNFA::weightedaccepts(string input) {
    set<pair<double, weightedNode*>> currentstates = {make_pair(0.0, this->startState)};

    for (const char& symbol : input){
        if (std::find(alfabet.begin(), alfabet.end(), string(1, symbol)) == alfabet.end()){
            cerr << "Symbol " << symbol << " not in alphabet" << endl;
            return -1.0;
        }
        
        set<pair<double, weightedNode*>> tempstates;
        for (const pair<double, weightedNode*>& state : currentstates){
            for (const auto& connection : state.second->getweightedconnections()){
                if (get<1>(connection).find(symbol) != get<1>(connection).end()){
                    tempstates.emplace(state.first + get<2>(connection), get<0>(connection));
                }
            }
        }
        currentstates = tempstates;
    }

    double largestpath = 0.0;
    for (const pair<double, weightedNode*>& state : currentstates){
        largestpath = max(largestpath, state.first);
    }
    return largestpath/input.size();
}*/

void WNFA::print() {
    json Jout;
    Jout["type"] = type;
    for (auto &symbol : alfabet){
        Jout["alphabet"].push_back(symbol);
    }
    for (auto &state: states) {  // add all the states
        json temp;
        temp["name"] = state.second->getName();
        temp["starting"] = isStartState(state.second->getName());
        temp["accepting"] = isEndState(state.second->getName());
        Jout["states"].push_back(temp);
    }

    for (auto &state: states) {// add all the transitions
        const map<char,vector<pair< double,weightedNode* >> > transitions = state.second->getweightedconnections();
        for (auto &k: transitions) { // {char, {pair(1.0 , State) , pair(1.0 , State)} }
            for(auto s: k.second){
                json temp;
                temp["from"] = state.second->getName();
                temp["to"] = s.second->getName();
                temp["input"] = string(1, k.first);
                temp["weight"] = s.first;
                Jout["transitions"].push_back(temp);
            }
        }
    }
    cout << setw(4) << Jout << endl;
}

pair<weightedNode*, bool> WNFA::getState(string name) const {
    auto result = states.find(name);
    if (result == states.end()){
        return make_pair(nullptr, false);
    }
    return make_pair(result->second, true);
}

bool WNFA::isStartState(string name) {
    if (getState(name).first == startState){
        return true;
    } else {
        return false;
    }
}

bool WNFA::isEndState(string name) {
    if (endStates.find(name) != endStates.end()){
        return true;
    }
    return false;
}

vector<string> WNFA::splitString(const string &str) {
    vector<string> tokens;
    string token;
    string filteredString;
    for (char character : str){
        if (character != '{' and character != '}'){
            filteredString += character;
        }
    }

    istringstream tokenStream(filteredString);
    while (getline(tokenStream, token, ',')) {
        tokens.push_back(token);
    }
    return tokens;
}

pair< map<string,weightedNode*> , double> WNFA::WSSC_helper(const map<string,weightedNode*> &currentstates, const char &input) {     // currentstate is van de vorm {a, b} met a en b staten in de NFA
    map<string,weightedNode*> to;
    double largestweight = 0.0;
    for (auto &node : currentstates) { // Loop over elke staat
        auto connections = node.second->accepts(input); //Find reachable states from node

        //cout << connections.empty() << endl;

        //if (!connections.empty()) { // als er een verbinding bestaat voor symbool 'input'
            for (auto &state: connections) {
                to[state.second->getName()]=state.second; //We should simply be able to overwrite the state with ittself if it already exists
                largestweight = max(largestweight, state.first); //Change weight to the biggest
            }
        //}
    }

    //sort(to.begin(), to.end()); //TODO is de sort hier nodig? Map sort automatisch?

    return make_pair(to, largestweight);
}

WDFA WNFA::toWDFA() {
    WDFA result;

    //cout << this->states.size() << endl;

    result.alfabet = alfabet;
    map<string,weightedNode*> startstate;
    startstate[startState->getName()] = startState;

    queue< map<string,weightedNode*> > toProcess;
    toProcess.push(startstate); //Add Start State for lazy evaluation
    result.addState("{" + startState->getName() + "}", true, true); //Add start state to DFA
    
    while(!toProcess.empty()){ //While there are states to proces
        //cout << toProcess.size() << endl;

        map<string,weightedNode*> processing = toProcess.front(); //current state
        toProcess.pop();

        //cout << toProcess.size() << endl;


        string processing_str = NodesToString(processing);

        for (const string& symbol_str : alfabet){
            const char& symbol = symbol_str[0];

            auto otherstate = WSSC_helper(processing , symbol);
            if(otherstate.first.empty()){
                continue;
            }
            string temp = NodesToString(otherstate.first);


            //cout << temp << endl;
            double weight = otherstate.second;

            //Maak van de queue een map
            if ( result.states.find(temp) == result.states.end() ){ //Als de huidige state nog niet in de WDFA zit
                toProcess.push(otherstate.first); //Add to process

                result.addState(temp, false, true); //Add the new state (Every state in the WNFA is an accepting state)
            }

            (result.getState(processing_str).first)->addconnection(result.getState(temp).first, symbol, weight); //Add a connection from State:Processing to the newly created State:temp
        }
        cout << toProcess.size() << endl;
    }
    return result;
}
