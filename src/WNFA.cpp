//
// Created by emill on 10/05/2023.
//

#include "WNFA.h"
#include "WDFA.h"

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

WNFA::WNFA() {type = "WNFA";}

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


double WNFA::weightedaccepts(string input) const {
    double result = 0.0;
    map<string, pair<double,weightedNode*> > currentStates;
    currentStates[startState->getName()]={0.0,startState};

    for (const char &symbol : input) { //Loop on every character of the string
        for (auto &node: currentStates) {
            auto nextStates = node.second.second->accepts(symbol); //Get the reachable states with this symbol

            if (nextStates.empty()) { //No transitions
                node.second.first -= 1.0; //Subtract values
            } else { //The state accepted this symbol
                for (const auto &state: nextStates) { //For each state we can reach with the symbol
                    result += nextStates[0].first;
                    currentStates[state.second->getName()] = {state.first + node.second.first,state.second}; // map[ "0" ] = {1.0 + 1.0 , *"0"}
                }
            }
        }
    }

    //Add all results together
    for(const auto &k: currentStates){
        result+=k.second.first;
    }

    return result/( (double) currentStates.size() );
}

void WNFA::print() const{
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

bool WNFA::isStartState(string name) const {
    if (getState(name).first == startState){
        return true;
    } else {
        return false;
    }
}

bool WNFA::isEndState(string name) const{
    if (endStates.find(name) != endStates.end()){
        return true;
    }
    return false;
}

map<string,weightedNode*> WNFA::splitString(const string &str) const {
    map<string,weightedNode*> tokens;
    string token;
    string filteredString;
    for (char character : str){
        if (character != '{' and character != '}'){
            filteredString += character;
        }
    }

    istringstream tokenStream(filteredString);
    while (getline(tokenStream, token, ',')) {
        tokens[token]= getState(token).first;
    }
    return tokens;
}

pair< map<string,weightedNode*> , double> WNFA::WSSC_helper(const map<string,weightedNode*> &currentstates, const char &input) const {
    map<string,weightedNode*> to;
    double largestweight = 0.0;
    for (auto &node : currentstates) { // Loop over elke staat
        auto connections = node.second->accepts(input); //Find reachable states from node

        for (auto &state: connections) {
            to[state.second->getName()] = state.second; //We should simply be able to overwrite the state with ittself if it already exists
            largestweight = max(largestweight, state.first); //Change weight to the biggest
        }
    }
    return make_pair(to, largestweight);
}

const map<string, weightedNode *> &WNFA::getStates() const {
    return states;
}

void WNFA::setStates(const map<string, weightedNode *> &states) {
    WNFA::states = states;
}

weightedNode *WNFA::getStartState() const {
    return startState;
}

void WNFA::setStartState(weightedNode *startState) {
    WNFA::startState = startState;
}

const map<string, weightedNode *> &WNFA::getEndStates() const {
    return endStates;
}

void WNFA::setEndStates(const map<string, weightedNode *> &endStates) {
    WNFA::endStates = endStates;
}

const string &WNFA::getType() const {
    return type;
}

void WNFA::setType(const string &type) {
    WNFA::type = type;
}

const vector<string> &WNFA::getAlfabet() const {
    return alfabet;
}

void WNFA::setAlfabet(const vector<string> &alfabet) {
    WNFA::alfabet = alfabet;
}

WDFA WNFA::toWDFA() {
    WDFA result;

    result.alfabet=alfabet;
    map<string,weightedNode*> startstate;
    startstate[startState->getName()] = startState;

    set< string > toProcess;
    toProcess.insert("{" + startState->getName() + "}"); //Add Start State for lazy evaluation
    result.addState("{" + startState->getName() + "}", true, true); //Add start state to DFA

    while(!toProcess.empty()){ //While there are states to proces
        string processing_str = *toProcess.begin();
        map<string,weightedNode*> processing = splitString(processing_str) ; //current state
        toProcess.erase(processing_str);

        for (const string& symbol_str : alfabet){
            const char& symbol = symbol_str[0];
            auto otherstate = WSSC_helper(processing , symbol);
            string temp = NodesToString(otherstate.first);
            double weight = otherstate.second;

            if(temp == "{}"){
                continue;
            }

            if ( result.states.find(temp) == result.states.end() ){ //Als de huidige state nog niet in de WDFA zit

                if(toProcess.find(temp)==toProcess.end()){
                    toProcess.insert(temp); //Add to process
                }

                result.addState(temp, false, true); //Add the new state (Every state in the WNFA is an accepting state)
            }

            (result.getState(processing_str).first)->addconnection(result.getState(temp).first, symbol, weight); //Add a connection from State:Processing to the newly created State:temp
        }
    }
    return result;
}