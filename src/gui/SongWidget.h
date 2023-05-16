//
// Created by tibov on 14/05/23.
//

#ifndef TOG_SONGWIDGET_H
#define TOG_SONGWIDGET_H
#include "Song.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#ifdef __unix__
#include <cstdio>
#include <cstdlib>
#endif

class SongWidget {
public:
    SongWidget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Song *song);
    void draw(Display* display, Window window, GC graphics_content, int offset, unsigned int min_height, unsigned int max_height);
    void draw(Display* display, Window window, GC graphics_content);
    pair<unsigned int, unsigned int> getPositionY(int offset, unsigned int min_height, unsigned int max_height);
    bool isClicked(unsigned int mouse_x, unsigned int mouse_y, int offset, unsigned int min_height, unsigned int max_height);

    unsigned int getX() const;

    unsigned int getY() const;
    void setPos(unsigned int x, unsigned int y);

public:

private:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    Song* song;

};


#endif //TOG_SONGWIDGET_H