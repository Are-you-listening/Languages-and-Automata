//
// Created by watson on 4/28/23.
//

#include "Genre.h"

double Genre::addGenre(const Song *&s) {
    if(inGenre(s)){
        members[limit].push_back(s);
    }
    return 0;
}

DFA Genre::toProductAutomata() const {
    vector<DFA> temp;

    //Loop over each Song
    for(auto it = members.begin(); it!=members.end(); it++){
        for(const Song* s: it->second){
            vector<RE> t = s->toRegex(param[0],param[0],param[0],param[0],param[0],-1); //Set pattern to -1 so we can generate 1 big Regex
            ENFA a = t[0].toENFA();
            DFA m = a.toDFA();
            m = m.minimize();
            if(temp.size()==0){
                temp.push_back(m);
            } else{
                temp[0] = DFA(temp[0], m , 0); //Make or extend ProductAutomata
            }
        }
    }
    return temp[0];
}

bool Genre::inGenre(const Song *&s) {
    DFA m = toProductAutomata();
    vector<RE> st = s->toRegex(param[0],param[0],param[0],param[0],param[0],param[0]);

    return m.accepts(st[0].re);
}

Genre::Genre(const map<double, vector<const Song *>> &members, double limit, const vector<int> &param) : members(members),
                                                                                                   limit(limit),
                                                                                                   param(param) {}

Genre::Genre(Song *&s, Song *&k, const vector<int> &params) {
    param = params;

    pair<vector<RE> , vector<RE>> toCheck = {s->toRegex(param[0],param[0],param[0],param[0],param[0],param[0]) , k->toRegex(param[0],param[0],param[0],param[0],param[0],param[0])};
    vector<double> temp = s->similar( toCheck , 0 ,0); //Run Similarity Check

    double result = 0;
    for(double &m: temp){
        result+=m;
    }

    //Set Data
    limit = result/temp.size();
    members[limit]={s,k};
}