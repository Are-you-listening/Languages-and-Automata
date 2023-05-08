#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Toi_Emil/WNFA.h"

int main() {

    /*
    Song song; 
    Song song2;
    song.parse("midi_files/world-1-birabuto-4-.mid");
    song2.parse("midi_broken/world-1-birabuto-remix-.mid");
    song.similarity(song2);
    */
    
    Song song;
    Song song2;
    song.parse("midi_files/metal_crusher.mid");
    song2.parse("midi_files/Metal_Crusher2.mid");
    song.similarity(song2);

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
