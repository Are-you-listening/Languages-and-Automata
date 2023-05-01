#include <iostream>
#include "midiparser/MidiParser.h"
#include <algorithm>
int main() {
    ifstream Filelist("filelist.txt");
    string c;
    string c2="midi_files/";
    while(getline(Filelist,c)){
        MidiParser m(c2+c);
        map<pair<unsigned int, bool>, set<Note*>> b = m.getNoteMap();
        cout << c+ " " +to_string(b.size()) + " bytes"<< endl;
    }
    Filelist.close();
    return 0;
}
