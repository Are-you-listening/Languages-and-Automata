#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Toi_Emil/WNFA.h"

int main() {
    //Song song("midi_files/world-1-birabuto-4-.mid");
    //Song song2("midi_broken/world-1-birabuto-remix-.mid");
    //song.similarity(song2);

    Song song3("midi_files/metal_crusher.mid");
    Song song4("midi_files/metal_crusher.mid");
    cout << song3.similarity(song4,0,0) << endl;
    song3.setTitle("metal_crusher");
    song3.output();

    //Song song5("midi_files/Metal_Crusher2.mid");
    //cout << song3.similarity(song5,0,0) << endl;
    //song3.output();

    /*
    RE r("00(0+1+2+3+4+5+6+7)i0", '*');
    ENFA e = r.toENFA();
    DFA d = e.toDFA();
    cout << d.accepts("000i0") << endl;*/
    
    /*
    Song song3;
    Song song4;
    song3.parse("midi_files/quick-man-s-stage-4-.mid");
    song4.parse("midi_files/MM9_Tornadoman.mid");
    song3.similarity(song4);
     */
    
    /*
    WNFA w("tiboEnv/WNFA_balance_test.json");
    double weight =  w.weightedaccepts("abcd");
     */

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
        vector<DFA> t;
        for(vector<RE>::const_iterator it=regexlist.begin(); it!=regexlist.end(); it++){
            cout << (*it).re << endl;

        }
        delete song;
    }
    Filelist.close(); //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
     */
    return 0;
}
