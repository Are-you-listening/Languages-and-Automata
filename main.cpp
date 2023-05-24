#include <iostream>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "Genre.h"
#include "Utils.h"

int main() {
    vector<int> V = {1, 1, 1, 1, 1, 1};

    Song* c0 = new Song("midi_files/c0.mid",1);
    Song* c1 = new Song("midi_files/c1.mid",1);
    Song* c2 = new Song("midi_files/c2.mid",1);
    Song* c3 = new Song("midi_files/c3.mid",1);
    Song* c4 = new Song("midi_files/c4.mid",1);
    Song* c5 = new Song("midi_files/c5.mid",1);
    Song* c6 = new Song("midi_files/c6.mid",1);
    Song* c7 = new Song("midi_files/c7.mid",1);
    Song* c8 = new Song("midi_files/c8.mid",1);
    Song* c9 = new Song("midi_files/c9.mid",1);
    Song* c10 = new Song("midi_files/c10.mid",1);
    Song* HarryPotter = new Song("midi_files/HarryPotter.mid",1);
    Song* HarryPotterPrologue = new Song("midi_files/HarryPotterPrologue(3).mid",1);
    Song* CoolBeat = new Song("midi_files/09_-_Cool_Beat.mid",1);
    Song* GameCube = new Song("midi_files/Game_Cube_intro.mid",1);

    c0->similarity(c1,0,0);
    c0->similarity(c2,0,0);
    c0->similarity(c3,0,0);
    c0->similarity(c4,0,0);
    c0->similarity(c5,0,0);
    c0->similarity(c6,0,0);
    c0->similarity(c7,0,0);
    c0->similarity(c8,0,0);
    c0->similarity(c9,0,0);
    c0->similarity(c10,0,0);

    c4->similarity(c5,0,0);

    Genre* HenryHerry = new Genre(HarryPotter,HarryPotterPrologue,V,"HenryHerry",0.70,0,0);

    Song* product = new Song(HenryHerry->getProductAutomata(),V,1);
    product->save("midi_output/Product.mid");

    cout << HenryHerry->inGenre(c0) << endl;

    delete c0;
    delete c1;
    delete c2;
    delete c3;
    delete c4;
    delete c5;
    delete c6;
    delete c7;
    delete c8;
    delete c9;
    delete c10;
    delete HarryPotter;
    delete HarryPotterPrologue;
    delete CoolBeat;
    delete GameCube;
    delete HenryHerry;



    /*vector<pair<string,string>> doubleComparison;
    ifstream Filelist("demo.txt");
    ofstream errorfile("errors.txt");
    string c;
    string c2="midi_files/";
    vector<Song*> songs;

    while(getline(Filelist,c)){
        Song* song = new Song(c2+c,true);
        song->setTitle(c);
        ifstream Filelist2("demo.txt");
        string c3;
        while(getline(Filelist2,c3)){
            if(find(doubleComparison.begin(), doubleComparison.end(), make_pair(c,c3))!=doubleComparison.end()){
                continue;
            } else {
                doubleComparison.emplace_back(c,c3);
            }
            Song* song2 = new Song(c2+c3,false);
            song2->setTitle(c3);
            try {
                vector<int> V = {1, 1, 1, 1, 1, -1};
                Genre genre = Genre(song, song2, V, c + "_compare_" + c3, 0.70, 1, 0);
                DFA* genreDFA = genre.getProductAutomata();
                Song* generated = new Song(genreDFA, V, true);
                generated->save("midi_output/" + c + "_compare_" + c3);
                song->similarity(song2, false, false);
                song->output();
                genre.output();
                generated->output();
                delete generated;
            } catch (...){
                errorfile << c << c3 << "\n";
            }
            delete song2;
        }
        delete song;
        Filelist2.close();
    }
    Filelist.close();
    errorfile.close();*/
    return 0;
}
