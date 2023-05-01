#include <iostream>
#include "midiparser/MidiParser.h"
int main() {
    MidiParser m("midi_files/t14.mid");
    map<pair<unsigned int, bool>, set<Note*>> b = m.getNoteMap();
    cout << b.size() << endl;
    return 0;
}
