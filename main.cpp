#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Genre.h"

#include "NFA.h"
int main() {
    Song* song1 = new Song("midi_files/amen-break-midi.mid", false);
    RE r = song1->toRegex(1, 3, 1, 2, 1, -1)[0];
    RE r2 = song1->toRegex(1, 1, 1, 1, 1, -1)[0];
    ENFA e = r.toENFA();
    DFA d = e.toDFA();
    cout << d.accepts(r2.re) << endl;
    cout << d.getStates().size() << endl;
    d = d.minimize();
    cout <<d.accepts(r2.re) << endl;

    /*
    vector<pair<string,string>> doubleComparison; // TODO enfa alpahbet bezit soms over incomplete sequence
    ifstream Filelist("filelist.txt"); 
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
