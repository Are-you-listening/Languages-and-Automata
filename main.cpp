#include <iostream>
#include "Song.h"
#include <algorithm>
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"

int main() {

    Song song;
    Song song2;
    song.parse("midi_files/world-1-birabuto-4-.mid");
    song2.parse("midi_files/world-1-birabuto-remix-.mid");
    song.similarity(song2);

    /*
    ifstream Filelist("filelist.txt");
    string c;
    string c3;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song();
        song->parse(c2+c);
        cout << c << endl;
        vector<RE> regexlist= song->toRegex(1,1,1,1,1,7);
        for(vector<RE>::const_iterator it=regexlist.begin(); it!=regexlist.end(); it++){
            cout << (*it).re << endl;
        }
        delete song;
    }
    Filelist.close(); //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
     */
    return 0;
}
