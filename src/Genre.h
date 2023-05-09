//
// Created by watson on 4/28/23.
//

#ifndef TOG_GENRE_H
#define TOG_GENRE_H

#include <map>

#include "Song.h"
#include "DFA.h"

class Genre {
private:
    /**
     * \brief Contains the current members of this Genre sorted by its match %
     */
    map<double,vector<Song*>> members;

    /**
     * \brief The minimum % of Similarity a newly given Song has to have with the Genre to join it
     */
    double limit;

    /**
     * \brief The paramaters (for Similarity) this genre is build on
     */
    vector<int> param;

    /**
     * \brief Generate a product automata for the current Genre's member-sub-regexes
     * @return Datatype containing the sub DFA's in productautomata
     */
    vector<DFA> toProductAutomata() const;

public:
    /**
     * \brief In case the match % <= limit, add to this Genre
     * @param s
     * @return match %
     */
    double addGenre(const Song* &s);

    /**
     * \brief Check if a given Song is part of this Genre
     * @param s
     * @return
     */
    bool inGenre(const Song* &s);

    /**
     * \brief Simple constructor
     * @param members
     * @param limit
     * @param param
     */
    Genre(const map<double, vector<Song *>> &members, double limit, const vector<int> &param);

    /**
     * \brief Create a Genre based on 2 Song's and a given set of paramaters.
     */
    Genre(const Song* &s, const Song* &k, const vector<int> &param);
};

#endif //TOG_GENRE_H
