//
// Created by tibov on 14/05/23.
//

#ifndef TOG_SONGLISTWIDGET_H
#define TOG_SONGLISTWIDGET_H
#include "Song.h"
#include "SongWidget.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#ifdef __unix__
#include <cstdio>
#include <cstdlib>
#endif

class SongListWidget {
public:
    SongListWidget(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    void doScrolled(unsigned int mouse_x, unsigned int mouse_y, bool up);
    bool inWidget(unsigned int mouse_x, unsigned int mouse_y);
    void draw(Display* display, Window window, GC graphics_content);
    void drawSongs(Display* display, Window window, GC graphics_content);
    SongWidget* select(unsigned int mouse_x, unsigned int mouse_y);
    void addSong(SongWidget* song_widget);
private:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    int index = 0;
     int index_offset = 0;
    vector<SongWidget*> songs;
    int change_index = 40;
};


#endif //TOG_SONGLISTWIDGET_H
