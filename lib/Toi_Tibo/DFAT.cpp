//
// Created by tibov on 25/02/23.
//

#include "DFAT.h"
#include <algorithm>
#include <fstream>

#include <iomanip>

DFAT::DFAT(const string &path) {
    /**
     * Constructor
     * */
    std::ifstream f(path);

    nlohmann::json data = nlohmann::json::parse(f);

    load(data);

}

bool DFAT::accepts(const string &s) {
    string current = start_state;
    for (char c: s){
        if (find(alfabet.begin(), alfabet.end(), c) == alfabet.end()){
            return false;
        }
        string new_state = transition_map[current][c];
        current = new_state;
    }
    if (find(end_states.begin(), end_states.end(), current) != end_states.end()){
        return true;
    }else{
        return false;
    }

}

void DFAT::print() const{
    nlohmann::json data = nlohmann::json::parse(R"({})");
    data["type"] = "DFA";
    for (auto a: alfabet){
        string s;
        s += a;
        data["alphabet"].push_back( s);
    }
    int index = 0;
    for (auto s : states){
        bool start = false;
        bool end = false;
        if (start_state == s){
            start = true;
        }

        if (find(end_states.begin(), end_states.end(), s) != end_states.end()){
            end = true;
        }

        data["states"][index]["name"] = s;
        data["states"][index]["starting"] = start;
        data["states"][index]["accepting"] = end;
        index += 1;
    }

    index = 0;
    for (auto t : transition_map){
        string state = t.first;
        for (char a: alfabet){
            string target = t.second.at(a);

            string s;
            s += a;

            data["transitions"][index]["from"] = state;
            data["transitions"][index]["to"] = target;
            data["transitions"][index]["input"] = s;
            index += 1;
        }

    }


    cout << setw(4) << data << endl;

}

void DFAT::make_TFA() {
    /**
     * maakt een table filling map die overal een boll heeft indien 2 states distinguishable zijn
     * */
    set<string>::iterator it_1, it_2, temp;
    for (it_1 = states.begin(); it_1 != states.end(); it_1++){
        temp = it_1;
        temp++;
        /**
         * Basisgeval indien een state een eindstaat is en een andere niet -> distinguishable
         */
        for (it_2 = temp; it_2 != states.end(); it_2++){
            if (is_end_state(*it_1) xor is_end_state(*it_2)){

                /**
                 * voeg toe dat deze combinatie distinguishable is
                 * ook roepen we disqualified op om recursief andere combinaties mogelijk te markeren als distinguishable
                 * */
                set_table_filling_map(*it_1, *it_2, true, table_filling_map);
                disqualified(*it_1, *it_2, transition_map, table_filling_map, states);


            }
        }
    }

}

bool DFAT::is_end_state(const string &state) {
    /**
     * geeft een bool terug die weergeeft dat het opgevraagde item een eindstaat is
     * */
    return find(end_states.begin(), end_states.end(), state) != end_states.end();
}

void DFAT::disqualified(const string &state1, const string &state2, const map<string , map<char, string>>& trans_map, map<string, map<string, bool>>& tfa_map, const set<string>& p_states) {
    /**
     * recursief markeren van combinaties als distinguishable
     * */

    for (char a: alfabet){


        /**
         * bewaar een set met alle states die naar state1 en state2 gaan volgens char a
         * ga nadien alle mogelijke combinaties af en bewaar ze als distinguishable
         * */
        set<string> s1;
        set<string> s2;
        for (const string &state: p_states){
            if (trans_map.at(state).at(a) == state1){
                s1.insert(state);
            }

            if (trans_map.at(state).at(a) == state2){
                s2.insert(state);
            }
        }

        for (const string &s1t: s1){
            for (const string &s2t: s2){
                if (!in_table_filling_map(s1t, s2t, true, tfa_map)){
                    /**
                     * bewaar en zichzelf oproepen recursief
                     * */
                    set_table_filling_map(s1t, s2t, true, tfa_map);
                    disqualified(s1t, s2t, trans_map, tfa_map, p_states);
                }

            }
        }

    }

}

void DFAT::set_table_filling_map(const string &state1, const string &state2, bool b, map<string, map<string, bool>>& tfa_map) {
    /**
     * Bewaar state in TFA-map
     * */
    if(tfa_map.find(state1) != tfa_map.end()) {
        map<string, bool> m = tfa_map.at(state1);
        m.insert({state2, b});
        tfa_map[state1] = m;
    }else if(tfa_map.find(state2) != tfa_map.end()){
        map<string, bool> m = tfa_map.at(state2);
        m.insert({state1, b});
        tfa_map[state2] = m;
    }else{
        map<string, bool> m;
        m.insert({state2, b});
        tfa_map.insert({state1, m});
    }
}

bool DFAT::in_table_filling_map(const string &state1, const string &state2, bool b, const map<string, map<string, bool>>& tfa_map) {
    /**
     * check dat er een state1, state2 combinatie bestaat met als waarde bool b
     * */
    if(tfa_map.find(state1) != tfa_map.end()) {
        map<string, bool> m = tfa_map.at(state1);
        if (m.find(state2) != m.end()){
            bool bm = m.at(state2);
            return bm == b;
        }
    }
    if(tfa_map.find(state2) != tfa_map.end()){
        map<string, bool> m = tfa_map.at(state2);
        if (m.find(state1) != m.end()){
            bool bm = m.at(state1);
            return bm == b;
        }
    }
    return false;
}

void DFAT::printTable() {
    /**
     * Print de TFA tabel uit
     */
    set<string>::iterator it;
    set<string>::iterator it2;
    int index = 0;
    for (it = states.begin(); it != states.end(); it++){
        if (index > 0){
            cout << *it;
        }

        for (it2 = states.begin(); it2 != it; it2++){

            if (in_table_filling_map(*it, *it2, true, table_filling_map)){
                cout << "\tX";
            }else{
                cout << "\t-";
            }

        }
        index += 1;
        cout << endl;
    }
    cout << "\t";
    for (it = states.begin(); it != states.end(); it++){
        it2 = it;
        it2++;
        if (it2 != states.end()){
            cout << "" << *it << "\t";
        }

    }
    cout << endl;

}

DFAT DFAT::minimize() {
    /**
     * Maak een geminimaliseerde DFAT
     * */

    /**
     * Hier wordt eerste een vector van verzamelingen gemaakt (equivalentie verzameling)
     * */
    vector<set<string>> equivalence;

    set<string>::iterator it;
    set<string>::iterator it2;
    for (it = states.begin(); it != states.end(); it++){

        for (it2 = states.begin(); it2 != it; it2++){

            if (!in_table_filling_map(*it, *it2, true, table_filling_map)){

                /**
                 * voegt toe aan equivalentie relatie (indien meer dan 2)
                 * */
                bool b1 = get_eq(*it, equivalence).second;
                bool b2 = get_eq(*it2, equivalence).second;

                if (b1 || b2){
                    set<string> eq_1 = get_eq(*it, equivalence).first;
                    add_eq(*it2, eq_1, equivalence);

                    set<string> eq_2 = get_eq(*it2, equivalence).first;
                    add_eq(*it, eq_2, equivalence);

                }else{
                    equivalence.push_back({*it, *it2});
                }

            }
        }
    }

    /**
     * De states dat een singleton zijn, worden hier toegevoegd aan de equivalentieverzameling
     * */
    for (auto state: states){
        bool found = get_eq(state, equivalence).second;

        if (!found){
            set<string> temp = {state};
            equivalence.push_back(temp);
        }

    }

    /**
     * Check elke state 1 voor 1, (lazy linking)
     * */
    set<string> unchecked;
    set<string> checked;
    map<string, map<char, string>> new_transition_map;

    unchecked.insert(start_state);

    while(!unchecked.empty()){

        set<string> new_unchecked;
        for (const string& state: unchecked){
            if (find(checked.begin(), checked.end(), state) != checked.end()){
                continue;
            }else{
                checked.insert(state);
            }

            map<char, string> m = transition_map.at(state);

            string from = set_to_string(get_eq(state, equivalence).first);

            for (char a: alfabet){
                string target = m.at(a);

                string to = set_to_string(get_eq(target, equivalence).first);

                new_transition_map[from].insert({a, to});

                new_unchecked.insert(target);

            }

        }

        unchecked = new_unchecked;

    }

    /**
     * initialiseer de nieuwe DFAT
     * */
    set<string> new_states;
    for (auto state: checked){
        new_states.insert(set_to_string(get_eq(state, equivalence).first));
    }

    set<string> new_end_states;

    for (string end_state: end_states){
        new_end_states.insert(set_to_string(get_eq(end_state, equivalence).first));
    }


    string new_start_state = set_to_string(get_eq(start_state, equivalence).first);

    DFAT mindfa(alfabet, new_states, new_transition_map, new_start_state, new_end_states);
    return mindfa;



}

string DFAT::set_to_string(const set<string> &s) const {
    /**
     * Convert set naar juiste format string
     * */
    string output;
    output += "{";
    for (const string& t: s){
        output += t + ", ";
    }
    output = output.substr(0, output.size()-2);
    output += "}";
    return output;
}

DFAT::DFAT(const set<char> &alfabet, const set<string> &states, const map<string, map<char, string>> &transition_map,
           const string &start_state, const set<string> &end_states): alfabet(alfabet), states(states), transition_map(transition_map), start_state(start_state), end_states(end_states) {
    make_TFA();
}

bool DFAT::operator==(const DFAT &d) {
    /**
     * controleer dat de DFAT's equivalent zijn
     * */
    if (alfabet != d.getAlfabet()){
        return false;
    }

    /**
     * maak 1 grote TFA
     * */
    map<string , map<char, string>> compare_map;
    map<string, map<string, bool>> compare_tfa_map;
    set<string> all_states;
    set<string> all_end_states;
    compare_map.insert(transition_map.begin(), transition_map.end());
    compare_map.insert(d.getTransitionMap().begin(), d.getTransitionMap().end());


    all_states.insert(states.begin(), states.end());
    all_states.insert(d.getStates().begin(), d.getStates().end());

    all_end_states.insert(end_states.begin(), end_states.end());
    all_end_states.insert(d.getEndStates().begin(), d.getEndStates().end());


    set<string>::iterator it_1, it_2, temp;
    for (it_1 = all_states.begin(); it_1 != all_states.end(); it_1++){

        temp = it_1;
        temp++;

        for (it_2 = temp; it_2 != all_states.end(); it_2++){
            if (find(all_end_states.begin(), all_end_states.end(), *it_1) != all_end_states.end() xor find(all_end_states.begin(), all_end_states.end(), *it_2) != all_end_states.end()){
                set_table_filling_map(*it_1, *it_2, true, compare_tfa_map);
                disqualified(*it_1, *it_2, compare_map, compare_tfa_map, all_states);


            }
        }
    }

    printTable(all_states, compare_tfa_map);

    /**
     * check dat de eindstaten distinguishable zijn
     * */
    return !in_table_filling_map(start_state, d.getStartState(), true, compare_tfa_map);
}

const map<string, map<char, string>> &DFAT::getTransitionMap() const {
    return transition_map;
}

const set<string> &DFAT::getStates() const {
    return states;
}

const string &DFAT::getStartState() const {
    return start_state;
}

const set<char> &DFAT::getAlfabet() const {
    return alfabet;
}

const set<string> &DFAT::getEndStates() const {
    return end_states;
}

pair<set<string>, bool> DFAT::get_eq(const string &state, const vector<set<string>> &equiv_vect) {

    for (auto &v: equiv_vect){
        if (find(v.begin(), v.end(), state) != v.end()){

            return make_pair(v, true);
        }
    }

    return make_pair(set<string>(), false);
}

void DFAT::add_eq(const string &state, const set<string> &eq, vector<set<string>>& equiv_vect) {
    for (auto &v: equiv_vect){
        if (eq == v){
            v.insert(state);
        }
    }
}

void DFAT::printTable(const set<string> &p_states, const map<string, map<string, bool>> &tfa_map) {
    set<string>::iterator it;
    set<string>::iterator it2;
    int index = 0;
    for (it = p_states.begin(); it != p_states.end(); it++){
        if (index > 0){
            cout << *it;
        }

        for (it2 = p_states.begin(); it2 != it; it2++){

            if (in_table_filling_map(*it, *it2, true, tfa_map)){
                cout << "\tX";
            }else{
                cout << "\t-";
            }

        }
        index += 1;
        cout << endl;
    }
    cout << "\t";
    for (it = p_states.begin(); it != p_states.end(); it++){
        it2 = it;
        it2++;
        if (it2 != p_states.end()){
            cout << "" << *it << "\t";
        }

    }
    cout << endl;

}

DFAT::DFAT() {

}

void DFAT::load(const nlohmann::json &data) {
    for (string s: data["alphabet"].get<std::set<string>>()){
        alfabet.insert(s[0]);
    }

    int state_size = data["states"].size();
    for (int i = 0; i<state_size; i++){
        string state_name = data["states"][i]["name"].get<std::string>();
        states.insert(state_name);
        bool starting = data["states"][i]["starting"].get<bool>();
        if (starting){
            start_state = state_name;
        }

        bool accepting = data["states"][i]["accepting"].get<bool>();
        if (accepting){
            end_states.insert(state_name);
        }

    }
    int transition_size = data["transitions"].size();
    for (int i = 0; i < transition_size; i++){
        string from = data["transitions"][i]["from"].get<std::string>();
        string to = data["transitions"][i]["to"].get<std::string>();
        char input = data["transitions"][i]["input"].get<std::string>()[0];
        transition_map[from].insert({input, to});
    }
    make_TFA();
}

nlohmann::json DFAT::getJson() const {
    nlohmann::json data = nlohmann::json::parse(R"({})");
    data["type"] = "DFA";
    for (auto a: alfabet){
        string s;
        s += a;
        data["alphabet"].push_back( s);
    }
    int index = 0;
    for (auto s : states){
        bool start = false;
        bool end = false;
        if (start_state == s){
            start = true;
        }

        if (find(end_states.begin(), end_states.end(), s) != end_states.end()){
            end = true;
        }

        data["states"][index]["name"] = s;
        data["states"][index]["starting"] = start;
        data["states"][index]["accepting"] = end;
        index += 1;
    }

    index = 0;
    for (auto t : transition_map){
        string state = t.first;
        for (char a: alfabet){
            string target = t.second.at(a);

            string s;
            s += a;

            data["transitions"][index]["from"] = state;
            data["transitions"][index]["to"] = target;
            data["transitions"][index]["input"] = s;
            index += 1;
        }

    }
    return data;
}

