//
// Created by watson on 2/28/23.
//

#ifndef INC_1_TRANSITIEFUNCTIE_H
#define INC_1_TRANSITIEFUNCTIE_H

#include <vector>
#include <string>

using namespace std;

class transitiefunctie {
private:
    vector<pair<pair<string,char>,string>> function; //Bevat de informatie om een functie te simuleren //Houdt de paden bij {{start,input}finished state}

public:
    //Constructor
    transitiefunctie(vector<pair<pair<string, char>, string>> &function);
    transitiefunctie();

    //Plaats een nieuwe functie
    void setFunction(vector<pair<pair<string, char>, string>> function);

    const vector<pair<pair<string, char>, string>> &getFunction() const;

    //Extended transition function
    string transition(const string &toestand, const string &x) const;

    //Doet de transitie voor 1 char
    string simple_transition(const string &toestand, const char &a) const;

    //Voeg single transition toe
    void add(pair<pair<string, char>, string> x);

    void changeStateName(string &state, string &newName);

    void clear();
};


#endif //INC_1_TRANSITIEFUNCTIE_H
