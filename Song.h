//
// Created by watson on 4/28/23.
//

#ifndef TOG_SONG_H
#define TOG_SONG_H

#include "lib/DesignByContract.h"

/**
 * Object which contains all the data of a Song.
 */
class Song {
private:
    Song* fInitCheck;

public:
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
};


#endif //TOG_SONG_H
