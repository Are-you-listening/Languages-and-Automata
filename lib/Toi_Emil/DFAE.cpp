//
// Created by emil on 24/03/23.
//

#include <iostream>
#include "DFAE.h"
#include "automaat.h"
using namespace std;

DFAE::DFAE(const string &fileName) : automaat(fileName) {
    type = "DFAE";
}

DFAE::DFAE() {
    type = "DFAE";
}

/**
 * controleert of de gegeven string haakjes mist: a + b
 * als dit zo is dan kunnen er problemen opdagen bij het concateneren
 */

bool pluswithoutbrackets(const string &input){
    bool betweenbrackets = false;
    for (char symbol : input){
        if (symbol == '('){
            betweenbrackets = true;
            continue;
        } else if (symbol == ')'){
            betweenbrackets = false;
        } else if (symbol == '+' && not betweenbrackets){
            return true;
        }
    }
    return false;
}

void DFAE::eliminateState(const std::string &eliminatedstate) {
    Node* todestroy = states[eliminatedstate];

    set<pair<Node*, string>> todestroyconnections;

    for (const auto& todestroyconnection: todestroy->regexconnections) {
        stringstream tempconnection;
        if (todestroyconnection.first == todestroy) {
            continue;
        }

        if (not todestroy->selfconnection.empty()) {
            if (pluswithoutbrackets(tempconnection.str())) {
                tempconnection << ")";
            }
            tempconnection << todestroy->selfconnection << "*";
        }

        if (todestroyconnection.second.find('+') !=
            todestroyconnection.second.npos) {       // als er een + in staat moeten er haken komen
            tempconnection << "(" << todestroyconnection.second << ")";
        } else {
            tempconnection << todestroyconnection.second;
        }
        string tempconnection_str = tempconnection.str();
        if (tempconnection_str.find('(') == tempconnection_str.npos &&
            tempconnection_str.find(')') != tempconnection_str.npos) {
            stringstream temp;
            temp << "(" << tempconnection_str;
            tempconnection.clear();
            tempconnection << temp.str();
        } else if (pluswithoutbrackets(tempconnection_str)) {
            stringstream temp;
            temp << "(" << tempconnection_str << ")";
            tempconnection.clear();
            tempconnection << temp.str();
        }
        todestroyconnections.emplace(todestroyconnection.first, tempconnection.str());
    }

    //map<Node*, string> connectionstoremove;

    for (const auto& temp_pair : states) {
        Node* state = temp_pair.second;
        if (state == todestroy) {
            continue;
        }
        set<pair<Node*, string>> connectionstoadd;

        if (state->regexconnections.find(todestroy) != state->regexconnections.end()) {
            pair<Node*, string> tempregexpair = make_pair(todestroy, state->regexconnections[todestroy]);
            if (pluswithoutbrackets(tempregexpair.second)){
                tempregexpair.second = "(" + tempregexpair.second + ")";
            }

            state->regexconnections.erase(todestroy);
            for (const auto& connection : todestroyconnections) {
                if (state->regexconnections[connection.first].empty()) {
                    connectionstoadd.emplace(connection.first, tempregexpair.second + connection.second);
                } else {
                    connectionstoadd.emplace(connection.first, state->regexconnections[connection.first] + "+" + tempregexpair.second + connection.second);
                }
            }
        }

        for (auto add : connectionstoadd){
            state->regexconnections[add.first] = add.second;
        }
    }
}

REE DFAE::toREE() const {
    for (auto &state : states){

        map<Node*, set<char>> tempconnections;

        for (const char& symbol : alfabet){
            tempconnections[state.second->connections[symbol]].emplace(symbol);
        }

        for (const auto& connection : tempconnections){
            string tempstring;
            if (connection.second.size() == 1){
                tempstring = *connection.second.begin();

            } else if (connection.second.size() > 1){
                tempstring += *connection.second.begin();

                for (auto it = connection.second.begin().operator++(); it != connection.second.end(); it++){
                    tempstring += "+";
                    tempstring += *it;
                }

            }
            state.second->regexconnections[connection.first] = tempstring;
            if (connection.first == state.second){
                state.second->selfconnection = tempstring;
            }
        }

    }

    // for each endstate eliminate all other states except for the starting state
    // generate the regex for this dfa with 1 or 2 states (structure found in textbook)
    // connect all the regex's with '+'

    stringstream finalregex;
    int count = 0;
    for (Node* endstate : endStates) {
        DFAE tempdfa(*this);
        for (pair<string, Node*> temp: tempdfa.states) {
            Node* state = temp.second;
            if (state->getName() != startState->getName() && state->getName() != endstate->getName()) {
                tempdfa.eliminateState(state->getName());
            }
        }
        string start2start;
        string start2end;
        string end2end;
        string end2start;
        stringstream regex;
        if (endstate != startState){    // if start- and endstate are seperate
            for (const auto &startconnection: tempdfa.getState(startState->getName()).first->regexconnections) {
                if (startconnection.first == tempdfa.startState) {
                    start2start = startconnection.second;
                } else if (startconnection.first == tempdfa.getState(endstate->getName()).first) {
                    start2end = startconnection.second;
                }
            }
            for (const auto &endconnection: tempdfa.getState(endstate->getName()).first->regexconnections) {
                if (endconnection.first == tempdfa.startState) {
                    end2start = endconnection.second;
                } else if (endconnection.first == tempdfa.getState(endstate->getName()).first) {
                    end2end = endconnection.second;
                }
            }

            vector<string*> strings = {&start2start, &start2end, &end2start, &end2end};

            for (auto temp : strings){
                if (pluswithoutbrackets(*temp)){
                    *temp = "(" + *temp + ")";
                }
            }

            if (not start2start.empty() || (not start2end.empty() && not end2start.empty())){
                regex << "(";
            }

            if (not start2start.empty()){
                regex << start2start;
            }

            if (not start2start.empty() && (not start2end.empty() && not end2start.empty())){
                regex << "+";
            }

            if (not start2end.empty() && not end2start.empty()){
                regex << start2end;
                if (not end2end.empty()){
                    if (end2end[0] == '(' && *(end2end.end()-1) == ')'){
                        regex << "" << end2end << "*";
                    } else {
                        regex << "(" << end2end << ")*";
                    }
                }
                regex << end2start;
            }

            if (not start2start.empty() || (not start2end.empty() && not end2start.empty())){
                regex << ")*";
            }

            regex << start2end;     // if start2end is empty, the endstate is unreachable from the startstate, so we always add it

            // also check if the string already has parentheses?

            if (not end2end.empty()){
                if (end2end[0] == '(' && *(end2end.end()-1) == ')'){
                    regex << "" << end2end << "*";
                } else {
                    regex << "(" << end2end << ")*";
                }
            }

        } else {        // if start- and endstate are the same
            // there will only be one connection
            end2end = endstate->regexconnections.begin()->second;
            regex << end2end << "*";
        }
        if (count == 0){
            finalregex << regex.str();
        } else {
            finalregex << "+" << regex.str();
        }
        count ++;
    }
    for (const auto& state : states){
        state.second->regexconnections.clear();
    }
    string result = finalregex.str();
    return REE(result);
}

DFAE::DFAE(const DFAE &other) {
    // Create new nodes
    for (const auto& node : other.states) {
        Node* new_node = new Node(node.first);
        states[node.first] = new_node;
        if (other.isStartState(node.first)) {
            startState = new_node;
        }
        if (other.isEndState(node.first)) {
            endStates.emplace(new_node);
        }
    }

    // Create new connections
    for (const auto& node : other.states) {
        Node* new_node = getState(node.first).first;
        for (const auto& connection : node.second->regexconnections) {
            Node* target = getState(connection.first->getName()).first;
            new_node->regexconnections[target] = connection.second;
        }
        if (not node.second->selfconnection.empty()) {
            new_node->selfconnection = node.second->selfconnection;
        }
    }
}
