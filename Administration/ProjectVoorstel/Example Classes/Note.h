//
// Created by watson on 4/17/23.
//

#ifndef TOG_NOTE_H
#define TOG_NOTE_H


#include "../../../lib/DesignByContract.h"

class Note {
    const int TimeStamp;
    const int Speed;
    const char Letter;

    Note* fInit;

    bool InitCheck();

public:
    /**
     * \brief Simple Constructor
     * REQUIRE()
     * ENSURE
     * @param TimeStamp
     * @param Speed
     * @param Letter
     */

    Note(int TimeStamp, int Speed, int Letter);

};


#endif //TOG_NOTE_H
