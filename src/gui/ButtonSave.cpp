//
// Created by tibov on 23/05/23.
//

#include "ButtonSave.h"

ButtonSave::ButtonSave(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                       SongListWidgetSingle *song_box): Button(x, y, width, height), song_box(song_box) {}

void ButtonSave::click() {
    Song* song = song_box->getSong();
    if (song == nullptr){
        return;
    }
    song->save("midi_output/GUI_song.mid");
}

void ButtonSave::draw(Display *display, Window window, GC graphics_content) {
    XSetForeground(display,graphics_content, (100 << 16)+255);
    XFillRectangle(display, window, graphics_content, getX(), getY(), getWidth(), getHeight());
}
