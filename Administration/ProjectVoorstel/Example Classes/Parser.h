//
// Created by watson on 4/17/23.
//

#ifndef TOG_PARSER_H
#define TOG_PARSER_H

#include "Song.h"


class Parser {

    /**
     * REQUIRE(FilExists() )
     * @param file : .mid file to read from
     * @return .mid file in Song format
     */
    Song* Parse(fstream &file) const;

};


#endif //TOG_PARSER_H
