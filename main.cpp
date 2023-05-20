#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Genre.h"
#include "Utils.h"

#include "NFA.h"
#include "gui/Gui.h"

int main() {
    vector<int> V = {1,1,1,1,1,-1};
    Song* s = new Song("midi_files/c0.mid" , false);

    ENFA az = s->toRegex(V[0],V[1],V[2],V[3],V[4],V[5])[0].toENFA();
    DFA aza = az.toDFA();

    Song* k = new Song("midi_files/c1.mid" , false);
    Genre m = Genre(s,k,V,"test",0,0);

    DFA a = m.getProductAutomata();
    Song* yaay = new Song(a,V,true);
    /*
    Song* k = new Song("midi_files/[ontiva.com] Tekken Tag Tournament - Hwoarang Theme-tiny.wav.mid" , false);
    RE r = k->toRegex(1, 1, 1, 1, 1, -1)[0];
    ENFA e = r.toENFA();
    NFA n(e.getJsonNfa());
    WNFA w = n.toWNFA();
    */


    /*
    string m = "(m+y)*+(e+y+m+i)s";
    const char e = 'e';
    RE r(m,e);
    ENFA k = r.toENFA();
    //ENFA s("enfa.json");
    //DFA s = k.toDFA();
    */
    /*Song* s = new Song("midi_files/c0.mid" , false);
    Song* k = new Song("midi_files/c1.mid" , false);
    Genre m = Genre(s,k,{1,1,1,1,0,1},"test",0,0);

    delete s;
    delete k;*/
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

            //Genre genre = Genre(song,song2,V,c+"_compare_"+c3,1,0);
            //DFA genreDFA=genre.getProductAutomata();
            //Song generated=Song(genreDFA,V,1);
            //string path="midi_output/"+c+"_compare_"+c3;
            //generated.save(path);

            //song->similarity(*song2,0,0);
            delete song2;
            break;
        }
        delete song;
        break;
    }
    Filelist.close();
    */
    return 0;
}
