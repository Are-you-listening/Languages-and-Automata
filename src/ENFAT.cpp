//
// Created by tibov on 21/03/23.
//

#include "ENFAT.h"

ENFAT::ENFAT(const string &path) {
    /**
     * Constructor
     * Maakt een e-NFA aan
     * */
    std::ifstream f(path);

    nlohmann::json data = nlohmann::json::parse(f);

    load(data);
}

set<string> ENFAT::Eclose(const string &state){
    /**
     * Default Eclose functie
     * */
    set<string> empty;
    return Eclose(state, empty);
}

set<string> ENFAT::Eclose(const string &state, const set<string>& found) {
    /**
     * echte Eclose functie, wordt gebruikt om bij te houden welke elementen we al in de verzameling hebben
     * zodat we een inf loop voorkomen
     *
     * Recursief maakt het een set van de eclose van de state
     * */
    set<string> output;
    output.insert(state);
    /**
     * indien de statt geen transities heeft
     * */
    map<string, set<string>>::iterator it = epsilon_transition_map.find(state);
    if (it == epsilon_transition_map.end()){
        return output;
    }

    /**
     * checkt welke staten we allemaal kunnen bereiken volgens een epsilon transitie
     * */
    set<string> new_found;
    new_found.insert(found.begin(), found.end());
    set<string> targets = it->second;
    new_found.insert(targets.begin(), targets.end());
    for (const string &s : targets){
        if (find(found.begin(), found.end(), s) == found.end()){
            set<string> rec_set = Eclose(s, new_found);
            output.insert(rec_set.begin(), rec_set.end());
        }

    }
    return output;
}

DFA ENFAT::toDFA() {
    /**
     * Vormt een DFA om naar e-NFA
     * */
    set<string> DFA_start_state;
    DFA_start_state = Eclose(start_state);
    set<set<string>> state_queue;
    //set<set<string>> finished ={};
    map<string, set<string>> finished;

    /**
     * maak een DFA-format transitiediagram
     * */
    map<string , map<char, string>> new_transition_map;
    state_queue.insert(DFA_start_state);
    while (!state_queue.empty()){
        set<string> current = *state_queue.begin();
        for (char a: alfabet){
            set<string> new_pos;
            for (const string &s: current){
                auto it = transition_map.find(s);
                if (it == transition_map.end()){
                    continue;
                }
                map<char, set<string>> temp = it->second;
                if (temp.find(a) == temp.end()){
                    continue;
                }
                set<string> targets = temp.at(a);
                for (const string& t: targets){
                    set<string> eq = Eclose(t);
                    new_pos.insert(eq.begin(), eq.end());
                }
            }

            string new_pos_string = set_to_string(new_pos);
            new_transition_map[set_to_string(current)][a] = new_pos_string;
            //if (find(finished.begin(), finished.end(), new_pos) == finished.end()){
            if (finished.find(new_pos_string) == finished.end()){
                state_queue.insert(new_pos);
            }
        }

        state_queue.erase(current);
        finished[set_to_string(current)] = current;

    }
    cout << "he" << endl;
    /**
     * check voor de nieuwe eindstaten
     * */
    set<string> end_states_string;
    for (auto fv: finished){
        auto f = fv.second;
        for (const string& end: end_states){
            if (find(f.begin(), f.end(), end) != f.end()){
                end_states_string.insert(set_to_string(f));
            }
        }
    }

    /**
     * convert set<string> naar string format
     * */

    set<string> states_string;
    for (auto fv: finished){
        auto f = fv.second;
        states_string.insert(set_to_string(f));
    }

    for (auto state :states_string){
        if (new_transition_map.find(state) == new_transition_map.end()){
            map<char, string> m;
            new_transition_map[state] = m;
        }

        for (char a: alfabet){
            if (new_transition_map.at(state).find(a) == new_transition_map.at(state).end()){
                new_transition_map[state][a] = state;
            }
        }
    }

    /**
     * maak nieuwe DFA aan
     * */
    string new_start_state = set_to_string(DFA_start_state);

    set<string> alfabet_parent;
    for (auto a: alfabet){
        string s;
        s += a;
        alfabet_parent.insert(s);
    }

    map<string, state*> state_linker;
    vector<state*> states_parent;
    vector<state*> end_parent;
    state* start_state_parent;
    for (auto s : states_string){
        bool start = false;
        bool end = false;

        state* temp_state = new state;

        if (new_start_state == s){
            start = true;
            start_state_parent = temp_state;
        }

        if (find(end_states_string.begin(), end_states_string.end(), s) != end_states_string.end()){
            end = true;
            end_parent.push_back(temp_state);
        }

        temp_state->name = s;
        temp_state->starting = start;
        temp_state->accepting = end;

        state_linker.insert({s, temp_state});
        states_parent.push_back(temp_state);
    }

    for (auto t : new_transition_map){
        string state_string = t.first;
        state* depart_state = state_linker.at(state_string);
        for (char a: alfabet){
            string target_string = t.second.at(a);
            state* arrive_state = state_linker.at(target_string);

            string s;
            s += a;
            depart_state->addTransitionFunction(s, arrive_state);
        }
    }

    DFA d;
    d.load(alfabet_parent, states_parent, start_state_parent, end_parent);
    return d;
}

string ENFAT::set_to_string(const set<string> &s) const {
    /**
     * Convert set naar juiste format string
     * */
    string output;
    output += "{";
    for (const string& t: s){
        output += t + ",";
    }
    if (!s.empty()){
        output = output.substr(0, output.size()-1);
    }

    output += "}";
    return output;
}

bool ENFAT::accepts(const string &s) {
    set<string> current_states;
    current_states = Eclose(start_state);
    for (char c: s){
        set<string> new_states;
        for (const auto& state: current_states){
            if (!(transition_map.find(state) != transition_map.end() && transition_map.at(state).find(c) != transition_map.at(state).end())){
                continue;
            }
            set<string> target_states = transition_map.at(state).at(c);
            set<string> target_states2;
            for (const auto& state: target_states){
                set<string> e_close_set = Eclose(state);
                target_states2.insert(e_close_set.begin(), e_close_set.end());
            }
            new_states.insert(target_states2.begin(), target_states2.end());
        }
        current_states = new_states;
    }

    for (const auto& state: current_states){
        if (find(end_states.begin(), end_states.end(), state) != end_states.end()){
            return true;
        }
    }

    return false;
}

void ENFAT::load(const json &data) {
    for (string s: data["alphabet"].get<std::set<string>>()){
        alfabet.insert(s[0]);
    }

    epsilon = data["eps"].get<std::string>()[0];

    /**
     * voegt epsilon nfa toe
     * */
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

    /**
     * stelt de transitie tabel op, aangezien er meerdere transities voor een character kunnen gebeuren is die een verzameling
     * */
    int transition_size = data["transitions"].size();
    for (int i = 0; i < transition_size; i++){
        string from = data["transitions"][i]["from"].get<std::string>();
        char input = data["transitions"][i]["input"].get<std::string>()[0];

        if(input == epsilon){
            set<string> to;
            if (epsilon_transition_map.find(from) != epsilon_transition_map.end()){
                to = epsilon_transition_map.at(from);
            }

            string temp_to = data["transitions"][i]["to"].get<std::string>();

            to.insert(temp_to);

            epsilon_transition_map[from] = to;
        }else{
            set<string> to;
            if (transition_map.find(from) != transition_map.end() && transition_map.at(from).find(input) != transition_map.at(from).end()){
                to = transition_map.at(from).at(input);
            }

            string temp_to = data["transitions"][i]["to"].get<std::string>();

            to.insert(temp_to);

            transition_map[from][input] = to;
        }

    }
}

ENFAT::ENFAT() {};