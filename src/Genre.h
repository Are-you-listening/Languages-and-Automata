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
     * \brief Toggle the Console Output
     */
     bool console = false;

    /**
     * \brief The name of this genre
     */
    string name;

    /**
     * \brief Keep a self pointer for ProperlyInitialised
     */
     const Genre* fInitCheck;

    /**
     * \brief Keep track of the logs
     */
    vector<string> logs;

    /**
     * \brief Contains the current members of this Genre
     */
    vector<Song*> members;

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
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @return Datatype containing the sub DFA's in productautomata
     */
    DFA toProductAutomata();

public:
    /**
    * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    * \brief Checks if an item is properly initialised
     * @return succes, bool deciding the succes of the operation
    */
    bool ProperlyInitialized() const;

    /**
     * \brief In case the match % <= limit, add to this Genre
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param s
     * @return
     */
    void addGenre(Song* &s);

    /**
     * \brief Check if a given Song is part of this Genre
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param s
     * @return
     */
    bool inGenre(Song* &s);

    /**
     * \brief Simple constructor
     * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param members
     * @param limit
     * @param param
     */
    Genre(const vector<Song *> &members, double limit, const vector<int> &param, string &name);

    /**
     * \brief Create a Genre based on 2 Song's and a given set of paramaters.
     * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param s
     * @param k
     * @param param
     */
    Genre(Song* &s, Song* &k, const vector<int> &params);

    /**
     * \brief Output results to a .txt file
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(FileExists(title+"_report.txt"),"No log file has been created");
     * @return
     */
    void output() const;

    /**
     * \brief Simple getter
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @return
     */
    const string &getName() const;

    /**
     * \brief Simple setter
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(Genre::name == name , "Setter didn't work properly");
     * @param name
     */
    void setName(const string &name);

    /**
     * \brief Turn the console output on/off
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     */
    void switchConsoleOutput();
};

#endif //TOG_GENRE_H
