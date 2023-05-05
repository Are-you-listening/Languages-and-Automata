//
// Created by tibov on 29/04/23.
//

#ifndef MIDIPARSER_NOTE_H
#define MIDIPARSER_NOTE_H
#include "iostream"
#include <string>

using namespace std;

class Note {
private:
    int channel;
    int note_value;
    unsigned int time_stamp;
    int velocity;
    bool note_on;

public:
    /**
     * \brief Simple Constructer
     * @param time_stamp
     * @param note_on
     * @param channel
     * @param note
     * @param velocity
     */
    Note(unsigned int time_stamp, bool note_on, int channel, int note, int velocity, unsigned int instrument);

    /**
     * \brief Convert a NoteObject to a Regex given by the selected inputparamaters (if @param=true, add to regex)
     * @param r_time_stamp
     * @param r_note_on
     * @param r_channel
     * @param r_note
     * @param r_velocity
     * @return
     */
    [[nodiscard]] string getRE(bool r_time_stamp, bool r_note_on, bool r_channel, bool r_note, bool r_velocity) const;
};


#endif //MIDIPARSER_NOTE_H
