#include <iostream>

#include "Song.h"
#include "src/DFA.h"
#include "src/ENFA.h"
#include "Genre.h"
#include "Utils.h"
#include "gui/Gui.h"
int main() {

    Gui g;
    g.start();

    /*
    Song* s = new Song("midi_files/c0.mid", 0);
    Song* s2 = new Song("midi_files/c1.mid", 0);

    Genre g = Genre(s, s2, {1, 1, 1, 1, 1, -1}, "s" ,0, 1);
    vector<int> v = {1, 1, 1, 1, 1, -1};
    Song* s3 = new Song(g.getProductAutomata(), v, 0);
    s3->save("midi_output/out.mid");
     */

    /*
    vector<pair<string,string>> doubleComparison;
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
    errorfile.close();
     */
    return 0;
}
