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
    vector<pair<string,string>> doubleComparison; // TODO enfa alpahbet bezit soms over incomplete sequence
    ifstream Filelist("filelistAnas.txt"); // hier moet je filelist met je naam komen
    string c;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song(c2+c,1);
        song->setTitle(c);
        ifstream Filelist2("filelist.txt"); // dit ongewijzigd laten aub
        string c3;
        while(getline(Filelist2,c3)){
            if(find(doubleComparison.begin(), doubleComparison.end(), make_pair(c,c3))!=doubleComparison.end()){
                continue;
            } else {
                doubleComparison.emplace_back(c,c3);
            }
            Song* song2 = new Song(c2+c3,0);
            song2->setTitle(c3);
            vector<int> V={1,1,1,1,1,-1};
            /*
            Genre genre = Genre(song,song2,V,c+"_compare_"+c3,1,0);
            DFA* genreDFA=genre.getProductAutomata();
            Song generated=Song(genreDFA,V,1);
            string path="midi_output/"+c+"_compare_"+c3;
            generated.save(path);
             */
            song->similarity(song2,0,0);
            delete song2;
        }
        delete song;
    }
    Filelist.close();
    return 0;
}
