//
// Created by tibov on 29/04/23.
//

#ifndef MIDIPARSER_NOTE_H
#define MIDIPARSER_NOTE_H

#include <iostream>
#include <string>

#include <DesignByContract.h>

using namespace std;

class Note {
private:
    unsigned int instrument;
    int note_value=0;
    unsigned int time_stamp;
    int velocity;
    bool note_on;

    string RoundInstrument(bool round_instrument, bool r_instrument) const;
    string RoundNote(int note_value, bool r_note, bool round, int round_index, bool round_octave, int round_index2) const;

public:
    /**
     * \brief Simple Constructer
     * @param time_stamp
     * @param note_on
     * @param channel
     * @param note
     * @param velocity
     */
    Note(unsigned int time_stamp, bool note_on, int note, int velocity, unsigned int instrument);

    /**
     * \brief Constructor to build Object Copy
     * @param n
     */
    Note(const Note &n);

    /**
     * \brief Convert a NoteObject to a Regex given by the selected inputparamaters (if @param=true, add to regex)
     * @param r_time_stamp
     * @param r_note_on
     * @param r_channel
     * @param r_note
     * @param r_velocity
     * @return
     */
    [[nodiscard]] string getRE(bool r_time_stamp, bool r_note_on, bool r_instrument, bool r_note, bool r_velocity, bool rounder) const;

    int getNoteValue() const;
};
#endif //MIDIPARSER_NOTE_H
