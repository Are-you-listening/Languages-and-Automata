//
// Created by tibov on 29/04/23.
//

#ifndef MIDIPARSER_NOTE_H
#define MIDIPARSER_NOTE_H

#include <iostream>
#include <string>
#include <cmath>

#include "Utils.h"
#include "DesignByContract.h"

using namespace std;

class Note {
private:

    /**
     * \brief The instrument playing this note
     */
    unsigned int instrument;

    /**
     * \brief The value of the note (A,B,C)
     */
    int note_value=0;

    /**
     * \brief Moment of playing
     */
    unsigned int time_stamp;

    /**
     * \brief "Music guys knows"
     */
    int velocity;

    /**
     * \brief Duration of the node been played
     */
    int duration=-1;

    /**
     * \brief Round the used instrument to it's general (Electric Piano = Piano)
     * @param r_instrument
     * @return
     */
    [[nodiscard]] string RoundInstrument(int r_instrument) const;

    /**
     * \brief Round to the notes close by
     * @param r_note
     * @param r_octaaf
     * @return
     */
    [[nodiscard]] string RoundNote(int r_note, int r_octaaf) const;

    /**
     * \brief Round the time stamp
     * @param r_time_stamp
     * @return
     */
    [[nodiscard]] string RoundTime_stamp(int r_time_stamp) const;

    /**
     * \brief Round the duration
     * @param r_duration
     * @return
     */
    [[nodiscard]] string RoundDuration(int r_duration) const;

    /**
     * \brief Round the velocity
     * @param r_velocity
     * @return
     */
    [[nodiscard]] string RoundVelocity(int r_velocity) const;

public:
    const double time_split = 1000;
    /**
     * \brief Simple Constructor
     * @param time_stamp
     * @param duration
     * @param channel
     * @param note
     * @param velocity
     */
    Note(unsigned int time_stamp, int duration, int note, int velocity, unsigned int instrument);

    /**
     * \brief Constructor to build Object Copy
     * @param n
     */
    Note(const Note &n);

    /**
     * \brief Convert a NoteObject to a Regex given by the selected inputparamaters (if @param=true, add to regex)
     * @param r_time_stamp
     * @param r_duration
     * @param r_channel
     * @param r_note
     * @param r_velocity
     * @return
     */
    [[nodiscard]] string getRE(int r_time_stamp, int r_duration, int r_instrument, int r_note, int r_velocity) const;

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] int getNoteValue() const;

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] unsigned int getInstrument() const;

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] unsigned int getTimeStamp() const;

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] int getVelocity() const;

    /**
     * \brief Simple getter
     * @return
     */
    [[nodiscard]] int getDuration() const;

    /**
     * \brief Simple setter
     * @return
     */
    void setDuration(int Duration);
};
#endif //MIDIPARSER_NOTE_H
