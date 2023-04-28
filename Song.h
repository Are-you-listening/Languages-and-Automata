//
// Created by watson on 4/28/23.
//

#ifndef TOG_SONG_H
#define TOG_SONG_H

#include "lib/DesignByContract.h"

class Song {
private:
    Song* fInitCheck;

public:
    /**
     * \REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");
     * \ENSURE(succes, "Percentage must be between 0 and 100");
     * \brief Run a similarity Check on this and a given song (song).
     * @param song
     * @return percentage, the percentage the Songs are the same
     */
    int similarity(Song &song) const;

    /**
    * \ENSURE ( ProperlyInitialized(), "constructor must end in properlyInitialized state");
    * \brief Checks if an item is properly initialised
    */
    bool ProperlyInitialized() const;
};


#endif //TOG_SONG_H
