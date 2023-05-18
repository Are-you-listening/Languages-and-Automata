//
// Created by tibov on 17/05/23.
//

#include "ButtonCompare.h"

ButtonCompare::ButtonCompare(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                             SongListWidget* compare, SongListWidgetSingle* song_box
                             ): Button(x, y, width, height), compare(compare), song_box(song_box) {

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
        double pct = song->similarity(*songs[0], 0, 0);
        cout << pct << endl;
    }

}
