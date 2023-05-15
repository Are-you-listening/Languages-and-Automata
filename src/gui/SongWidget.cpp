//
// Created by tibov on 14/05/23.
//

#include "SongWidget.h"

SongWidget::SongWidget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Song *song) :
x(x), y(y), width(width), height(height), song(song) {}

void SongWidget::draw(Display *display, Window window, GC graphics_content, int offset,  unsigned int min_height, unsigned int max_height) {
    cout << "off " << offset << endl;
    XSetForeground(display,graphics_content, 900000);

    unsigned int using_height;
    unsigned int using_y = y;
    if (y+offset+height > max_height){
        using_height = max_height-y-offset;
    }else if (offset < 0){
        using_y = min_height-1*offset;
        using_height = height-(min_height-x-offset);
        cout << "using " << using_height << endl;
    }else{
        using_height = height;
    }

    XFillRectangle(display, window, graphics_content, x, using_y+offset, width, using_height);
}

unsigned int SongWidget::getX() const {
    return x;
}

unsigned int SongWidget::getY() const {
    return y;
}
