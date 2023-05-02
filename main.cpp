#include <iostream>
#include "midiparser/MidiParser.h"
#include <algorithm>
#include "src/DFA.h"
#include "src/ENFA.h"
#include "src/RE.h"
int main() {

    DFA temp("JSONWORK.json");
    string s = temp.ToRe();
    RE RE(s, '*');
    ENFA n = RE.toENFA();
    DFA d = n.toDFA();
    cout << (d == temp) << endl;
    /*
    DFA temp("JSONWORK.json");
    DFA b = temp.minimize();
    bool bd = temp==b;
    cout << bd << endl;*/

    /*
    ifstream Filelist("filelist.txt");
    string c;
    string c2="midi_files/";
    while(getline(Filelist,c)){
        MidiParser m(c2+c);
        map<pair<unsigned int, bool>, set<Note*>> b = m.getNoteMap(); //TODO cascades_a_rag_(1904)_(nc)smythe.mid, morton_159s_mr_jelly_lord_(1924)_(nc)smythe.mid crashen het systeem,
        cout << c+ " " +to_string(b.size()) + " bytes"<< endl; //TODO deze files zijn geschreven in rag time. kan zijn dat dit hierdoor komt, ik heb die voor nu uit de filelist gelaten.
    } //TODO mischien moeten wij vanaf nu de afpsraak maken dat alle files geopend worden met ios::binairy
    Filelist.close();
     */
    return 0;
}
