//
// Created by watson on 4/28/23.
//

#include "Genre.h"

double Genre::addGenre(const Song *&s) {
    return 0;
}

vector<DFA> Genre::toProductAutomata() const {
    return vector<DFA>();
}

bool Genre::inGenre(const Song *&s) {
    return false;
}

Genre::Genre(const map<double, vector<Song *>> &members, double limit, const vector<int> &param) : members(members),
                                                                                                   limit(limit),
                                                                                                   param(param) {}

Genre::Genre(const Song *&s, const Song *&k, const vector<int> &param) {

}