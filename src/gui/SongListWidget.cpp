//
// Created by tibov on 14/05/23.
//

#include "SongListWidget.h"
#include "cmath"
SongListWidget::SongListWidget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, const vector<Song*>& songs_in): x(x), y(y), width(width), height(height) {
    for (int i=0; i<songs_in.size(); i++){
        SongWidget* w = new SongWidget(x+10, y+10, width-20, 80, songs_in[i]);
        songs.push_back(w);
    }


}

void SongListWidget::doScrolled(unsigned int mouse_x, unsigned int mouse_y, bool up) {
    if (inWidget(mouse_x, mouse_y)){
        if (!up && index >= 0){

            index_offset -= change_index;
            if (index_offset < 0 && index-1 > 0){
                index -= ceil(abs(index_offset+1)/80.0);

                while (index_offset < 0){
                    index_offset = index_offset + 80;
                }
            }


        }else if (up && index < 10){

            index_offset += change_index;
            if (index_offset > 80){
                index += floor(index_offset/80.0);
                index_offset = index_offset % 80;
            }
        }
    }

}

bool SongListWidget::inWidget(unsigned int mouse_x, unsigned int mouse_y) {
    bool in_x = x <= mouse_x && mouse_x <= x+width;
    bool in_y = y <= mouse_y && mouse_y <= y+height;
    return in_x && in_y;
}

void SongListWidget::draw(Display *display, Window window, GC graphics_content) {

    XSetForeground(display,graphics_content, 255);
    XFillRectangle(display, window, graphics_content, x, y, width, height);
    drawSongs(display, window, graphics_content);
}

void SongListWidget::drawSongs(Display *display, Window window, GC graphics_content) {
    for (int i=0; i< songs.size(); i++){
        SongWidget* song_widget = songs[i];
        int offset = index*80+index_offset+ i*100;
        bool skip = (int) song_widget->getY()+offset+(int)y < 10 || (int) song_widget->getY()+offset >= (int) height+(int)y;
        if (!skip){
            song_widget->draw(display, window, graphics_content, offset, y+10, height+y-10);
        }

    }

}

SongWidget *SongListWidget::select(unsigned int mouse_x, unsigned int mouse_y) {
    if (inWidget(mouse_x, mouse_y)){
        for (int i=0; i< songs.size(); i++){
            SongWidget* song_widget = songs[i];
            int offset = index*80+index_offset+ i*100;
            if (song_widget->isClicked(mouse_x, mouse_y, offset, y+10, height+y-10)){

                songs.erase(next(songs.begin(), i));
                return song_widget;
            }
        }
    }
    return nullptr;
}

void SongListWidget::addSong(SongWidget *song_widget) {

    song_widget->setPos(x+10, y+10);
    songs.push_back(song_widget);

}

vector<SongWidget *> SongListWidget::getSongs() {
    return songs;
}

vector<Song *> SongListWidget::getAllSongs() {
    vector<Song *> out;
    for (auto s_w: songs){
        out.push_back(s_w->getSong());
    }

    return out;
}
