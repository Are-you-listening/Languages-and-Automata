//
// Created by watson on 4/28/23.
//

#include "Song.h"

bool Song::ProperlyInitialized() const {
    if(fInitCheck==this){
        return true;
    }
    return false;
}

int Song::similarity(Song &song) const {
    REQUIRE( ProperlyInitialized(), "constructor must end in properlyInitialized state");

    int percentage = 5;

    bool succes = false;
    if(percentage<=100 && percentage>=0){
        succes== true;
    }

    ENSURE(succes, "Percentage must be between 0 and 100");
    return percentage;
}
