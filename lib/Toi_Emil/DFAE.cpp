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

bool comesbefore(string str1, string str2) {
    char firstChar1 = '\0';
    char firstChar2 = '\0';
    for (int i = 0; i < str1.size(); i++) {
        if (isalnum(str1[i])) {
            firstChar1 = str1[i];
            break;
        }
    }
    for (int i = 0; i < str2.size(); i++) {
        if (isalnum(str2[i])) {
            firstChar2 = str2[i];
            if (firstChar1 != firstChar2){
                break;
            }
        }
    }
    if (tolower(firstChar1) < tolower(firstChar2)) {
        return true;
    } else {
        return false;
    }
}

void DFAE::eliminateState(std::string eliminatedstate) {
    Node* todestroy = getState(eliminatedstate).first;
    for (Node* state : states) {
        if (state == todestroy) {
            continue;
        }
        list<pair<Node*, string>> connectionstoadd;
        list<pair<Node*, string>> connectionstoremove;
        for (const auto &regexconnection: state->regexconnections) {
            if (regexconnection.first == todestroy) {
                for (const auto& todestroyconnection: todestroy->regexconnections) {
                    string tempconnection = regexconnection.second;
                    if (todestroyconnection.first == todestroy) {
                        continue;
                    }


                    if (not todestroy->selfconnection.empty()) {
                        if (pluswithoutbrackets(tempconnection)){
                            tempconnection += ")";
                        }
                        tempconnection += todestroy->selfconnection + "*";
                    }

                    if (todestroyconnection.second.find('+') != todestroyconnection.second.npos){       // als er een + in staat moeten er haken komen
                        tempconnection += "(" + todestroyconnection.second + ")";
                    } else {
                        tempconnection += todestroyconnection.second;
                    }
                    if (tempconnection.find('(') == tempconnection.npos && tempconnection.find(')') != tempconnection.npos){
                        tempconnection = "(" + tempconnection;
                    } else if (pluswithoutbrackets(tempconnection)){
                        tempconnection = "(" + tempconnection + ")";
                    }

                    connectionstoadd.emplace_back(todestroyconnection.first, tempconnection);
                    connectionstoremove.emplace_back(regexconnection);
                }
            }
        }
        for (const auto & connection : connectionstoremove){
            state->regexconnections.remove(connection);
        }
        list<pair<Node*, string>> temp;
        for (auto& regexconnection : state->regexconnections){
            for (auto connection : connectionstoadd){
                if (regexconnection.first == connection.first){
                    if (comesbefore(regexconnection.second, connection.second)){
                        regexconnection.second += "+" + connection.second;
                    } else {
                        regexconnection.second = connection.second + "+" + regexconnection.second;
                    }
                    temp.push_back(connection);
                }
            }
        }
        for (auto connection : temp){
            connectionstoadd.remove(connection);
        }
        state->regexconnections.insert(state->regexconnections.end(), connectionstoadd.begin(), connectionstoadd.end());
    }
}



REE DFAE::toREE() {
    //copy the current DFAE and move the connections to regexconnections, before eliminating any states
    bool sorted;
    do {
        for (auto it = states.begin(); it != states.end().operator--(); it++) {
            auto next = it.operator++();
            it.operator--();
            if (comesbefore((*next)->getName(), (*it)->getName())) {
                sorted = false;
                auto *temp = *next;
                *next = *it;
                *it = temp;
            } else {
                sorted = true;
            }
        }
    } while (not sorted);
    do {
        for (auto it = endStates.begin(); it != endStates.end().operator--(); it++) {
            auto next = it.operator++();
            it.operator--();
            if (comesbefore((*next)->getName(), (*it)->getName())) {
                sorted = false;
                auto *temp = *next;
                *next = *it;
                *it = temp;
            } else {
                sorted = true;
            }
        }
    } while (not sorted);

    for (auto &state : states){
        for (const auto& connection : state->connections){
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
            state->regexconnections.emplace_back(connection.first, tempstring);
            if (connection.first == state){
                state->selfconnection = tempstring;
            }
        }
    }

    // for each endstate eliminate all other states except for the starting state
    // generate the regex for this dfa with 1 or 2 states (structure found in textbook)
    // connect all the regex's with '+'

    string finalregex;
    int count = 0;
    for (Node* endstate : endStates) {
        DFAE tempdfa(*this);
        for (Node *state: tempdfa.states) {
            if (state->getName() != startState->getName() && state->getName() != endstate->getName()) {
                tempdfa.eliminateState(state->getName());
            }
        }
        string start2start;
        string start2end;
        string end2end;
        string end2start;
        string regex;
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

            for (auto regex : strings){
                if (pluswithoutbrackets(*regex)){
                    *regex = "(" + *regex + ")";
                }
            }

            if (not start2start.empty() || (not start2end.empty() && not end2start.empty())){
                regex += "(";
            }

            if (not start2start.empty()){
                regex += start2start;
            }

            if (not start2start.empty() && (not start2end.empty() && not end2start.empty())){
                regex += "+";
            }

            if (not start2end.empty() && not end2start.empty()){
                regex += start2end;
                if (not end2end.empty()){
                    if (end2end[0] == '(' && *(end2end.end()-1) == ')'){
                        regex += "" + end2end + "*";
                    } else {
                        regex += "(" + end2end + ")*";
                    }
                }
                regex += end2start;
            }

            if (not start2start.empty() || (not start2end.empty() && not end2start.empty())){
                regex += ")*";
            }

            regex += start2end;     // if start2end is empty, the endstate is unreachable from the startstate, so we always add it

            // also check if the string already has parentheses?

            if (not end2end.empty()){
                if (end2end[0] == '(' && *(end2end.end()-1) == ')'){
                    regex += "" + end2end + "*";
                } else {
                    regex += "(" + end2end + ")*";
                }
            }

        } else {        // if start- and endstate are the same
            // there will only be one connection
            end2end = endstate->regexconnections.begin()->second;
            regex = end2end + "*";
        }
        if (count == 0){
            finalregex += regex;
        } else {
            finalregex += "+" + regex;
        }
        count ++;
    }
    return REE(finalregex);
}

DFAE::DFAE(DFAE &other) {
    // Create new nodes
    for (const auto& node : other.states) {
        Node* new_node = new Node(node->getName());
        states.push_back(new_node);
        if (other.isStartState(node->getName())) {
            startState = new_node;
        }
        if (other.isEndState(node->getName())) {
            endStates.push_back(new_node);
        }
    }

    // Create new connections
    for (const auto& node : other.states) {
        Node* new_node = getState(node->getName()).first;
        for (const auto& connection : node->regexconnections) {
            Node* target = getState(connection.first->getName()).first;
            new_node->regexconnections.emplace_back(target, connection.second);
        }
        if (not node->selfconnection.empty()) {
            new_node->selfconnection = node->selfconnection;
        }
    }
}

