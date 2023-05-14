#include <iostream>
#include <algorithm>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
#include "Genre.h"

#include "NFA.h"

int main() {
    /*
    Song song("midi_files/world-1-birabuto-4-.mid");
    RE r = song.toRegex(0, 0, 1, 1, 0, 1)[0];
    ENFA e = r.toENFA();
    json j= e.getJsonNfa();
    NFA n(j);*/

    /*
    Song* song = new Song("midi_files/world-1-birabuto-remix-.mid");
    Song* song2 = new Song("midi_files/world-1-birabuto-4-.mid");
    //song->switchConsoleOutput();
    //song->similarity(*song2, 0, 0);

    Song* song3 = new Song("midi_files/metal_crusher.mid");
    Song* song4 = new Song("midi_files/metal_crusher.mid");
    song3->setTitle("metal_crusher");

    Genre k = Genre(song3, song4, {1,1,1,1,1},"Test");

    k.switchConsoleOutput();
    k.output();
    k.addGenre(song4);
    song3.switchConsoleOutput();*/



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
    
    vector<pair<string,string>> doubleComparison;
    ifstream Filelist("filelist.txt");
    ifstream Filelist2("filelist.txt");
    string c;
    string c3;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song(c2+c,1);
        while(getline(Filelist2,c3)){
            if(find(doubleComparison.begin(), doubleComparison.end(), make_pair(c,c3))!=doubleComparison.end()){
                continue;
            } else {
                doubleComparison.emplace_back(c,c3);
            }
            Song* song2 = new Song(c2+c3,0);
            vector<int> V={1,1,1,1,0,1};
            Genre genre = Genre(song,song2,V,c+"_compare_"+c3,1);
            DFA genreDFA=genre.getProductAutomata();
            Song generated=Song(genreDFA,V,0);
            string path="midi_output/"+c+"_compare_"+c3;
            generated.save(path);
            cout << "compare " << c << " with " << c3 << endl;
            double matchprecentage = song->similarity(*song2,0,0);
            if (matchprecentage<25){
                cout << "\033[1;31m match precentage: \033[0m" << "\033[1;31m" << matchprecentage << "\033[0m" <<endl; //TODO dit zou gecombineerd moeten worden met de log;
            } else if (matchprecentage<50){
                cout << "\033[1;33m match precentage: \033[0m" << "\033[1;33m" << matchprecentage << "\033[0m" <<endl;
            } else if (matchprecentage<90){
                cout << "\033[1;32m match precentage: \033[0m" << "\033[1;32m" << matchprecentage << "\033[0m" <<endl;
            } else {
                cout << "\033[1;42m\033[1;1m\033[1;35m match precentage: \033[0m" << "\033[1;42m\033[1;1m\033[1;35m" << matchprecentage << "\033[0m" <<endl;
            }
            delete song2;
            if(c==c3&&matchprecentage!=1){
                cerr << "\033[1;31midentical files should result in 100% match\033[0m" <<endl;
            }
            cout << endl;
        }
        delete song;
    }
    Filelist.close(); //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
    return 0;
}
