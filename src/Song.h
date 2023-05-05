//
// Created by watson on 4/28/23.
//

#ifndef TOG_SONG_H
#define TOG_SONG_H

#include <string>

#include "DesignByContract.h"
#include "midiparser/MidiParser.h"
#include "RE.h"

using namespace std;

/**
 * Object which contains all the data of a Song.
 */
class Song {
private:
    Song* fInitCheck;

    /**
     * \brief Map sorted by the timestamp of the song and a bool deciding if these Notes are on/off. The timestamp is manually set to a accuracy of 1ms (0,001s)
     */
    map<pair<unsigned int, bool>, vector<Note*>> note_map; //{TimeStamp, on/off} {Note*} Timestamp=0,001s

public:

    /**
     * \brief Destructor to free all used memory
     */
    ~Song();

    /**
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(succes, "Percentage must be between 0 and 100");
     * \brief Run a similarity Check on this Song and a given Song ('song').
     * @param song
     * @return percentage, the percentage the Songs are the same
     */
    int similarity(Song &song) const;

    /**
    * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    * \brief Checks if an item is properly initialised
     * @return succes, bool deciding the succes of the operation
    */
    bool ProperlyInitialized() const;

    /**
     * \brief Parse a .mid file and load the Note-data into this Song-Object.
     * @param path , the path to the .mid file
     */
    void parse(const string& path);

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
    vector<RE> toRegex(bool time_stamp, bool note_on, bool instrument, bool note_b, bool velocity, int pattern) const;
};


#endif //TOG_SONG_H
