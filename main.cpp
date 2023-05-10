#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Toi_Emil/WNFA.h"
#include "NFA.h"

int main() {
    /*
    Song song("midi_files/world-1-birabuto-4-.mid");
    RE r = song.toRegex(0, 0, 1, 1, 0, 1)[0];
    ENFA e = r.toENFA();
    json j= e.getJsonNfa();
    NFA n(j);*/

    Song song3("midi_files/world-1-birabuto-4-.mid");
    song3.save("out.mid");
    /*
    Song song2("midi_broken/world-1-birabuto-remix-.mid");
    song.similarity(song2, 0, 0);


    Song song3("midi_files/metal_crusher.mid");
    Song song4("midi_files/metal_crusher.mid");
    song3.setTitle("metal_crusher");
    song3.switchConsoleOutput();

    cout << song3.similarity(song4,0,0) << endl;
    song3.output();
     */


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
    
    ifstream Filelist("filelist.txt");
    ifstream Filelist2("filelist.txt");
    string c;
    string c3;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song(c2+c);
        while(getline(Filelist2,c3)){
            Song* song2 = new Song(c2+c3);
            cout << "compare " << c << " with " << c3 << endl;
            double matchprecentage = song->similarity(*song2,0,0);
            if (matchprecentage<25){
                cout << "\032match precentage: \032" << "\031" << matchprecentage << "\031" <<endl;
            } else if (matchprecentage<50){
                cout << "\032match precentage: \032" << "\033" << matchprecentage << "\033" <<endl;
            } else if (matchprecentage<90){
                cout << "\032match precentage: \032" << "\032" << matchprecentage << "\032" <<endl;
            } else {
                cout << "\032match precentage: \032" << R"(")" << matchprecentage << R"(")" <<endl;
            }
        }
        delete song;
    }
    Filelist.close(); //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
    
    return 0;
}
