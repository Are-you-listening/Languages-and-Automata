//
// Created by tibov on 17/05/23.
//

#include "ButtonCompare.h"
#include "Genre.h"
ButtonCompare::ButtonCompare(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                             SongListWidget* compare, SongListWidgetSingle* song_box, ButtonBool* comp, ButtonBool* reverse, PctScreen* pcts
                             ): Button(x, y, width, height), compare(compare), song_box(song_box), complement_button(comp), reverse_button(reverse), pcts(pcts) {

}

void ButtonCompare::click() {
    Song* song = song_box->getSong();
    vector<Song*> songs = compare->getAllSongs();

    if (song == nullptr){
        if (songs.size() >= 2){
            Genre g = Genre(songs[0], songs[1], {1,1,1,1,1,-1}, "GUI", 0, 0);
            for(long unsigned int i =2; i<songs.size(); i++){
                g.addGenre(songs[i]);
            }

            DFA* genreDFA = g.getProductAutomata();
            vector<int> v = {1,1,1,1,1,-1};
            Song* new_song = new Song(genreDFA, v, false);
            SongWidget* new_widget = new SongWidget(getX()+10, getY()+10, getWidth()-20, 80, new_song);
            song_box->addSong(new_widget);

            new_song->save("out.mid");
        }
        return;
    }

    if (songs.empty()){
        return;
    }

    if(songs.size() == 1){
        double pct = song->similarity(songs[0], complement_button->isOn(), reverse_button->isOn());
        double pct2 = songs[0]->similarity(song, complement_button->isOn(), reverse_button->isOn());
        pcts->setPct((pct+pct2)/2);
    }else{
        Genre g = Genre(songs[0], songs[1], {0,1,0,1,0,-1}, "GUI", 0, 0);
        for(long unsigned int i =2; i<songs.size(); i++){
            g.addGenre(songs[i]);
        }
        double pct = g.similarity(song);
        pcts->setPct(pct*100);

    }

}
