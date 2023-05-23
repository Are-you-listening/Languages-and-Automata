#include <iostream>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "Genre.h"
#include "Utils.h"

int main() {
    Song* k = new Song("midi_files/c0.mid", true);
    Song* s = new Song("midi_files/c1.mid", true);

    s->similarity(k,0,0);
    //s->save("kees");

    vector<int> V = {1, 1, 1, 1, 1, -1};
    Genre genre = Genre(k, s, V, "test", 1, 1);
    DFA *genreDFA = genre.getProductAutomata();
    Song* generated = new Song(genreDFA, V, true);
    //generated->save("");

    //s->similarity(generated,0,0);

    delete k;
    delete s;

    /*vector<pair<string,string>> doubleComparison;
    ifstream Filelist("filelistAnas.txt"); // hier moet je filelist met je naam komen
    ofstream errorfile("errors.txt");
    string c;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song(c2+c,true);
        song->setTitle(c);
        ifstream Filelist2("filelist3.txt"); // dit ongewijzigd laten a.u.b.
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
                Genre genre = Genre(song, song2, V, c + "_compare_" + c3, 1, 0);
                DFA *genreDFA = genre.getProductAutomata();
                Song generated = Song(genreDFA, V, true);
                generated.save("midi_output/" + c + "_compare_" + c3);
                song->similarity(song2, false, false);
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
