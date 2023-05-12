//
// Created by tibov on 29/04/23.
//

#ifndef MIDIPARSER_NOTE_H
#define MIDIPARSER_NOTE_H

#include <iostream>
#include <string>

#include "Utils.h"
#include "DesignByContract.h"

using namespace std;

class Note {
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
    [[nodiscard]] string getRE(int r_time_stamp, int r_note_on, int r_instrument, int r_note, int r_velocity, int octaaf) const;

    int getNoteValue() const;

    unsigned int getInstrument() const;

    unsigned int getTimeStamp() const;

    int getVelocity() const;

    bool isNoteOn() const;

private:
    unsigned int instrument;
    int note_value=0;
    unsigned int time_stamp;
    int velocity;
    bool note_on;

    string RoundInstrument(int r_instrument) const;
    string RoundNote(int r_note, int r_octaaf) const;
    string RoundTime_stamp(int r_time_stamp) const;
    string RoundNote_on(int r_note_on) const;
    string RoundVelocity(int r_velocity) const;
};
#endif //MIDIPARSER_NOTE_H
