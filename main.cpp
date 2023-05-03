#include <iostream>
#include "Song.h"
#include <algorithm>
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"

int main() {
    /*
    Song* song = new Song();
    song->parse("midi_files/morton_159s_mr_jelly_lord_(1924)_(nc)smythe.mid");*/

    ifstream Filelist("filelist.txt");
    string c;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song();
        song->parse(c2+c); //TODO cascades_a_rag_(1904)_(nc)smythe.mid, morton_159s_mr_jelly_lord_(1924)_(nc)smythe.mid crashen het systeem,
        cout << c << endl; //TODO deze files zijn geschreven in rag time. kan zijn dat dit hierdoor komt, ik heb die voor nu uit de filelist gelaten.
    } //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
    Filelist.close();

    return 0;
}
