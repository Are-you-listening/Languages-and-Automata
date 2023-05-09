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

Genre::Genre(const map<double, vector<Song *>> &members, double limit, const vector<bool> &param) : members(members),
                                                                                                    limit(limit),
                                                                                                    param(param) {}

bool Genre::inGenre(const Song *&s) {
    return false;
}

Genre::Genre(const Song *&s, const Song *&k, vector<bool> &param) {

}
