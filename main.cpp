#include <iostream>
#include "midiparser/MidiParser.h"
int main() {
    MidiParser m("midi_files/t1.mid");
    map<pair<unsigned int, bool>, set<Note*>> b = m.getNoteMap();

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
