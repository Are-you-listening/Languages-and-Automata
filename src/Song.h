//
// Created by watson on 4/28/23.
//

#ifndef TOG_SONG_H
#define TOG_SONG_H

#include <string>

#include "DesignByContract.h"
#include "Utils.h"
#include "midiparser/MidiParser.h"
#include "midiparser/SongExporter.h"
#include "RE.h"
#include "Note.h"

using namespace std;

/**
 * Object which contains all the data of a Song.
 */
class Song {
private:
    /**
     * \brief Title of the song
     */
    string title;

    /**
     * \brief To keep track of the logs (pushed_back into by various functions)
     */
    vector<string> logs;

    Song* fInitCheck;

    /**
     * \brief Map sorted by the timestamp of the song and a bool deciding if these Notes are on/off. The timestamp is manually set to a accuracy of 1ms (0,001s)
     */
    map<pair<unsigned int, bool>, vector<Note*>> note_map; //{TimeStamp, on/off} {Note*} Timestamp=0,001s

    /**
     * \brief Set the biggest vector to the second argument
     * @param t
     * @return
     */
    [[nodiscard]] pair<vector<RE>,vector<RE>> sort(const pair<vector<RE>,vector<RE>> &t) const;

    /**
     * \brief Convert Regex's to minimized DFA's
     * @param s
     * @param complement, bool deciding if the DFA complement needs to be taken
     * @param reverse, bool deciding if the DFA reverse needs to be taken
     * @return
     */
    vector<DFA> convert(vector<RE> &s, bool complement, bool reverse) const;

    /**
     * \brief Check the similarity in order
     * \REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \REQUIRE(d.size()<=s.size(), "Indices should be always valid");
     * \ENSURE(succeeded, "Operation did not work properly");
     * @return
     */
    double checkTibo(vector<DFA> &d, vector<RE> &s) const;

    /**
     * \brief Cross check the similarity
     * \REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \ENSURE(succeeded, "Operation did not work properly");
     * @return
     */
    double checkKars(vector<DFA> &d, vector<RE> &s) const;

    /**
     * \brief Cross check the similarity but more accuractly?
     * \REQUIRE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \ENSURE(succeeded, "Operation did not work properly");
     * @param d
     * @param s
     * @return
     */
    double checkKarsAnas(vector<DFA> &d, vector<RE> &s) const;

    /**
     * \brief Run all checks on 1 sort of Regex
     * \ENSURE(succes, "Percentage must be between 0 and 1");
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param pair<vector<RE>,vector<RE>> &toCheck
     * @param complement, in case the complements of the 2 songs needs to be checked
     * @param reverse, in case the complements of the 2 songs needs to be checked
     * @return
     */
     vector<double> similar(pair<vector<RE>,vector<RE>> &toCheck,bool complement, bool reverse) const;

    /**
     * \brief Magimathical Formula Anas
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     */
    double magimathical(vector<vector<double>> &results) const;

public:
    /**
    * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    * \brief Checks if an item is properly initialised
     * @return succes, bool deciding the succes of the operation
    */
    bool ProperlyInitialized() const;

    /**
     * \brief Constructor
     * @param noteMap
     */
    Song(const map<pair<unsigned int, bool>, vector<Note *>> &noteMap);

    /**
     * \brief Empty Constructor
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     */
    Song();

    /**
     * \brief Parse a .mid file and load it's contents into a newly created object.
     * @param path , the path to the .mid file
     * \REQUIRE(FileExists(path) , "Given file not found");
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     */
    Song(const string& path);

    /**
     * \brief Overloader Constructor (Creates a new object) Including new usages of memory!
     * \REQUIRE(a.ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * \ENSURE(ProperlyInitialized(), "Constructor must end in properly initialised state!");
     * @param a
     * @return
     */
    Song& operator=(const Song &a);
    
    /**
     * \brief Destructor to free all used memory
     */
    ~Song();

    /**
     * \brief Convert a Song into a vector of Sub-Regex's
     * @param time_stamp , bool deciding if this Note-parameter should be used
     * @param note_on , bool deciding if this Note-parameter should be used
     * @param instrument , bool deciding if this Note-parameter should be used
     * @param note , bool deciding if this Note-parameter should be used
     * @param velocity , bool deciding if this Note-parameter should be used
     * @param pattern, int deciding the complexixity of the constructed Regex
     * @return vector<RE> rex
     */
    [[nodiscard]] vector<RE> toRegex(bool time_stamp, bool note_on, bool instrument, bool note_b, bool velocity, int pattern, bool rounder) const;

    /**
     * \brief Count the amount of notes (per type) of a Song
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @return map<int,unsigned int> map[Note]=occurences
     */
    [[nodiscard]] map<int,unsigned int> countNotes() const;

    /**
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(succes, "Percentage must be between 0 and 1");
     * \brief Run a similarity Check on this Song and a given Song ('song').
     * @param song
     * @param complement, in case the complements of the 2 songs needs to be checked
     * @param reverse, in case the complements of the 2 songs needs to be checked
     * @return percentage, the percentage the Songs are the same
     */
    [[nodiscard]] double similarity(const Song &song,bool complement, bool reverse);

    /**
     * \brief Check the occurences of the Notes and its similarity towards the notes of this object
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(succeed, "Percentage must be between 0 and 1");
     * @return
     */
    [[nodiscard]] double noteCountSimilarity(const Song &s);
    
    /**
     * \brief Check for 100% Equality of Songs
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param rhs
     * @return
     */
    bool operator==(const Song &rhs);

    /**
     * \brief Check if songs are not the same
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @param rhs
     * @return
     */
    bool operator!=(const Song &rhs);

    /**
     * \brief Export the retrieved data by a Song's operations into a .txt file
     * \ENSURE(FileExists(title+"_report.txt"),"No log file has been created");
     * \REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     */
    void Export() const;

    /**
     * \brief Get the title of an object
     * \REQUIRE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * @return
     */
    const string &getTitle() const;
};

#endif //TOG_SONG_H
