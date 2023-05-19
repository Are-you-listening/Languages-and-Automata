#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Genre.h"

#include "NFA.h"
#include "gui/Gui.h"
int main() {

    /*
    Gui g;
    g.start();
     */
    /*
    Song* s = new Song("midi_files/HarryPotter.mid", 0);
    Song* s2 = new Song("midi_files/HarryPotter.mid", 0);
     s->similarity(*s2, 0, 0);
     */

    /*
    Song s("midi_files/c0.mid" , true);
    Song k("midi_files/c1.mid" , true);
    s.similarity(k,false, false);
    cout << ":";*/

    Song* s = new Song("midi_files/HarryPotter.mid", 0);
    Song* s2 = new Song("midi_files/HarryPotter.mid", 0);

    //vector<int> V={1,1,1,1,0,-1};
    //Genre genre = Genre(s,s2,V,"_compare_",0, 0);
    //DFA genreDFA=genre.getProductAutomata();
    RE r = s->toRegex(1, 1, 1, 1, 1 , -1)[0];
    ENFA e = r.toENFA();
    DFA d = e.toDFA();
    vector<int> v = {1,1,1,1,1,-1};
    Song* s3 = new Song(d, v, 0);
    s3->save("out.mid");
    Song* s4 = new Song("out.mid", 0);
    s4->save("out.mid");


    /*
    Song* s = new Song("midi_files/HarryPotter.mid", 0);
    Song* s2 = new Song("midi_files/t2.mid", 0);
    RE r = s->toRegex(0, 0, 0, 1, 1, -1)[0];
    RE r3 = s2->toRegex(0, 0, 0, 1, 1, -1)[0];
    RE r2 = s->toRegex(0, 0, 0, 1, 1, -1)[0];
    ENFA e = r.toENFA();
    ENFA e3 = r3.toENFA();
    DFA d = e.toDFA();
    DFA d3 = e3.toDFA();


    vector<int> V={0,0,0,1,1,-1};
    cout << d3.getStates().size() << " " << d.getStates().size() << endl;
    cout << d.minimize().getStates().size() << endl;
    cout << "b" << endl;
    Genre genre = Genre(s,s2,V,"_compare_",0);
    DFA genreDFA=genre.getProductAutomata();
    cout << genreDFA.accepts(r2.re) << endl;
    cout << genreDFA.getStates().size() << endl;
     */
    //cout << d2.getStates().size() << endl;
    //cout << d2.accepts(r2.re) << endl;
    /*
    vector<pair<string,string>> doubleComparison; // TODO enfa alpahbet bezit soms over incomplete sequence
    ifstream Filelist("filelist2nd.txt"); 
    string c;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song(c2+c,1);
        song->setTitle(c);
        ifstream Filelist2("filelist.txt");
        string c3;
        while(getline(Filelist2,c3)){
            if(find(doubleComparison.begin(), doubleComparison.end(), make_pair(c,c3))!=doubleComparison.end()){
                continue;
            } else {
                doubleComparison.emplace_back(c,c3);
            }
            Song* song2 = new Song(c2+c3,0);
            song2->setTitle(c3);
            vector<int> V={1,1,1,1,0,1};
            Genre genre = Genre(song,song2,V,c+"_compare_"+c3,1);
            DFA genreDFA=genre.getProductAutomata();
            Song generated=Song(genreDFA,V,1);
            string path="midi_output/"+c+"_compare_"+c3;
            generated.save(path);
            song->similarity(*song2,0,0);
            delete song2;
        }
        delete song;
    }
    Filelist.close();
     */
    return 0;
}
