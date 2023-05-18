//
// Created by emill on 10/05/2023.
//

#include <queue>
#include "WNFA.h"

WNFA::WNFA(const string &filename) {
    ifstream input(filename);
    json j;
    input >> j;
    type = j["type"];
    set<string> temp = j["alphabet"];
    alfabet.clear();
    for (auto item : temp){
        string symbol = item;
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
        weightedNode *state1 = getWeightedState(transition["from"]).first;
        weightedNode *state2 = getWeightedState(transition["to"]).first;
        string inputsymbol = transition["input"];
        double weight = transition["weight"];
        for (auto symbol: inputsymbol) {
            state1->addconnection(state2, symbol, weight);
        }
    }
}

WNFA::WNFA() {}

void WNFA::addState(string name, bool start, bool endState) {
    weightedNode* n = new weightedNode(name);
    states[name] = n;
    if (start){
        startState = n;
    }
    if (endState){
        endStates[name] = n;
    }
}

pair<weightedNode *, bool> WNFA::getWeightedState(string name) const{
    auto result = states.find(name);
    if (result == states.end()){
        return make_pair(nullptr, false);
    }
    return make_pair(result->second, true);
}

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
}

void WNFA::print() {
    json Jout;
    Jout["type"] = type;
    for (auto symbol : alfabet){
        Jout["alphabet"].push_back(symbol);
    }
    for (auto state: states) {  // add all the states
        json temp;
        temp["name"] = state.second->getName();
        temp["starting"] = isStartState(state.second->getName());
        temp["accepting"] = isEndState(state.second->getName());
        Jout["states"].push_back(temp);
    }

    for (auto state: states) {  // add all the transitions
        for (const auto& transition: state.second->getweightedconnections()) {
            for (auto symbol: get<1>(transition)) {
                json temp;
                temp["from"] = state.second->getName();
                temp["to"] = get<0>(transition)->getName();
                temp["input"] = string(1, symbol);
                temp["weight"] = get<2>(transition);
                Jout["transitions"].push_back(temp);
            }
        }
    }
    cout << setw(4) << Jout << endl;
}

pair<Node*, bool> WNFA::getState(string name) {
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

pair<string, double> WNFA::WSSC_helper(const string &currentstate, const char &input) {     // currentstate is van de vorm {a, b} met a en b staten in de NFA
    vector<string> temp;
    double largestweight = 0.0;
    for (const auto& state : splitString(currentstate)){                                                     // splits de string op in een verzameling van staten
        for (auto connection : getWeightedState(state).first->getweightedconnections()){
            if (get<1>(connection).find(input) != get<1>(connection).end()){                                       // als er een verbinding bestaat voor symbool 'input'
                if (find(temp.begin(), temp.end(), get<0>(connection)->getName()) == temp.end()){           // check of we al eens een verbinding naar de nieuwe staat hebben gemaakt
                    temp.push_back(get<0>(connection)->getName());
                }
                
                largestweight = max(largestweight, get<2>(connection));
            }
        }
    }
    string result = "{";
    sort(temp.begin(), temp.end());
    for(const auto& part : temp){
        result += part;
        if (part != temp.back()){
            result += ",";
        }
    }
    result += "}";
    return make_pair(result, largestweight);
}

WDFA WNFA::toWDFA() {
    WDFA result;
    result.alfabet = alfabet;
    string startstate = "{" + startState->getName() + "}";
    queue<string> toProcess;
    toProcess.push(startstate);
    
    if (endStates.find(startState->getName()) == endStates.end()){
        result.addState(startstate, true, false);
    } else {
        result.addState(startstate, true, true);
    }
    
    while(not toProcess.empty()){
        string processing = toProcess.front();
        for (const string& symbol_str : alfabet){
            const char& symbol = symbol_str[0];
            pair<string, double> otherstate = WSSC_helper(processing, symbol);
            string temp = otherstate.first;
            double weight = otherstate.second;
            
            if (! result.getState(temp).second){
                toProcess.push(temp);
                
                bool accepting = false;
                
                vector<string> split = splitString(temp);
                
                for (const auto& state : split){
                    if (endStates.find(state) != endStates.end()){
                        accepting = true;
                    }
                }
                result.addState(temp, false, accepting);
            }
            result.getState(processing).first->addconnection(result.getState(temp).first, symbol, weight);
        }
        toProcess.pop();
    }
    return result;
}
