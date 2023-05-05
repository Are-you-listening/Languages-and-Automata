#include <iostream>
#include "Song.h"
#include <algorithm>
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"

int main() {
    ifstream Filelist("filelist.txt");
    string c;
    string c2="midi_files/";
    vector<Song*> songs;
    while(getline(Filelist,c)){
        Song* song = new Song();
        song->parse(c2+c);
        cout << c << endl;
    }
    Filelist.close(); //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
    return 0;
}
