//
// Created by watson on 4/28/23.
//

#include "Genre.h"

double Genre::addGenre(const Song *&s) {
    return 0;
}

vector<DFA> Genre::toProductAutomata() const {

    //vector<RE> REs = s->toRegex(param[0],param[0],param[0],param[0],param[0],-1,param[0]); //Set pattern to -1 so we can generate 1 big Regex
    //vector<RE> REk = k->toRegex(param[0],param[0],param[0],param[0],param[0],-1,param[0]);

    return vector<DFA>();
}

bool Genre::inGenre(const Song *&s) {
    return false;
}

Genre::Genre(const map<double, vector<Song *>> &members, double limit, const vector<int> &param) : members(members),
                                                                                                   limit(limit),
                                                                                                   param(param) {}

Genre::Genre(Song *&s, Song *&k, const vector<int> &params) {
    param = params;

    pair<vector<RE> , vector<RE>> toCheck = {s->toRegex(param[0],param[0],param[0],param[0],param[0],param[0],param[0]) , k->toRegex(param[0],param[0],param[0],param[0],param[0],param[0],param[0])};
    vector<double> temp = s->similar( toCheck , 0 ,0); //Run Similarity Check

    double result = 0;
    for(double &m: temp){
        result+=m;
    }

    //Set Data
    limit = result/temp.size();
    members[limit]={s,k};
}