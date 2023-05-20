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

    if (song == nullptr){
        return;
    }

    vector<Song*> songs = compare->getAllSongs();
    if (songs.empty()){
        return;
    }


    if(songs.size() == 1){
        double pct = song->similarity(*songs[0], complement_button->isOn(), reverse_button->isOn());
        double pct2 = songs[0]->similarity(*song, complement_button->isOn(), reverse_button->isOn());
        pcts->setPct((pct+pct2)/2);
    }else{
        Genre g = Genre(songs[0], songs[1], {0,1,0,1,0,-1}, "GUI", 0, 0);
        for(int i =2; i<songs.size(); i++){
            g.addGenre(songs[i]);
        }
    }

}
