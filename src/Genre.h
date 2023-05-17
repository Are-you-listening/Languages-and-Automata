//
// Created by watson on 4/28/23.
//

#ifndef TOG_GENRE_H
#define TOG_GENRE_H

#include "Song.h"
#include "DFA.h"

class Genre {
private:
    /**
     * \brief The current ProductAutomate build up from the existing members (pair.first = members.size)
     */
     pair<int,DFA> ProductAutomata;

    /**
     * \brief Toggle the Console Output
     */
     bool console;

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
     * \brief The paramaters (for Similarity) this genre is build on
     */
    vector<int> param;

    /**
     * \brief The minimum amount a new Song has to have to be added to the Genre
     */
    double limit;

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
      * brief Create a Genre based on 2 Song's and a given set of paramaters. The minimum match % (required for new songs) will be initialised on the Similarity % between k & s
      * \REQUIRE(params.size()==6, "Params doesn't has all the paramaters");
      * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
      * @param s
      * @param k
      * @param params
      * @param name
      * @param console
      */
    Genre(Song* &s, Song* &k, const vector<int> &params, const string &name, bool console);

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

    /**
     * \brief Retrieve the current ProductAutomata
     * @return
     */
    DFA getProductAutomata() const;
};

#endif //TOG_GENRE_H
