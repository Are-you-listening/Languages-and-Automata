//
// Created by tibov on 14/05/23.
//

#include "SongWidget.h"
#include "cmath"
SongWidget::SongWidget(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Song *song) :
x(x), y(y), width(width), height(height), song(song) {

}

void SongWidget::draw(Display *display, Window window, GC graphics_content, int offset,  unsigned int min_height, unsigned int max_height) {
    XSetForeground(display,graphics_content, (70 << 16)+(70 << 8)+205);

    pair<unsigned int, unsigned int> p = getPositionY(offset, min_height, max_height);
    unsigned int using_y = p.first;
    unsigned int using_height = p.second;

    draw_stack.push({x, using_y, width, using_height});

    XFillRectangle(display, window, graphics_content, x, using_y, width, using_height);

    XFontStruct* font_info = XLoadQueryFont(display, "-Sony-Fixed-Medium-R-Normal--24-170-100-100-C-120-JISX0201.1976-0");
    XSetForeground(display,graphics_content, 0);
    XSetFont(display, graphics_content, font_info->fid);
    XFlush(display);
    if (height == using_height){
        XDrawString(display, window, graphics_content, x+10, using_y+40, &song->getTitle()[0], min(18, (int) song->getTitle().size()));
    }



}

unsigned int SongWidget::getX() const {
    return x;
}

unsigned int SongWidget::getY() const {
    return y;
}

pair<unsigned int, unsigned int>
SongWidget::getPositionY(int offset, unsigned int min_height, unsigned int max_height) {
    int using_height;
    unsigned int using_y = y+offset;
    if (y+offset+height > max_height){
        using_height = max_height-y-offset;
    }else if (offset < 0){
        using_y = (int) min_height;
        using_height = (int) height + offset;
        if (using_height < 0){
            return make_pair(0, 0);
        }
    }else{
        using_height = height;
    }
    return make_pair(using_y, using_height);
}

bool SongWidget::isClicked(unsigned int mouse_x, unsigned int mouse_y, int offset, unsigned int min_height, unsigned int max_height) {
    pair<unsigned int, unsigned int> p = getPositionY(offset, min_height, max_height);
    unsigned int using_y = p.first;
    unsigned int using_height = p.second;
    bool in_x = x <= mouse_x && mouse_x <= x+width;
    bool in_y = using_y <= mouse_y && mouse_y <= using_y+using_height;
    return in_x && in_y;
}

void SongWidget::setPosMouse(unsigned int x, unsigned int y) {
    SongWidget::old_x = SongWidget::x;
    SongWidget::old_y = SongWidget::y;

    SongWidget::x = x-width/2;
    SongWidget::y = y-height/2;
}

void SongWidget::draw(Display *display, Window window, GC graphics_content, bool keep) {
    if(!keep){
        clear(display, window);
    }
    XSetForeground(display,graphics_content, (70 << 16)+(70 << 8)+205);
    draw_stack.push({x, y, width, height});
    XFillRectangle(display, window, graphics_content, x, y, width, height);


}

void SongWidget::setPos(unsigned int x, unsigned int y) {
    SongWidget::x = x;
    SongWidget::y = y;
}

void SongWidget::clear(Display *display, Window window) {
    while (!draw_stack.empty()){
        vector<unsigned int> v = draw_stack.top();
        draw_stack.pop();
        XClearArea(display, window, v[0], v[1], v[2], v[3], 0);
    }

}

Song *SongWidget::getSong() const {
    return song;
}

