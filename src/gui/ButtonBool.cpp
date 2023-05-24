//
// Created by tibov on 18/05/23.
//

#include "ButtonBool.h"

void ButtonBool::click() {
    on = !on;
}

ButtonBool::ButtonBool(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool on): Button(x, y, width, height), on(on) {}

void ButtonBool::draw(Display *display, Window window, GC graphics_content) {
    if (on){
        XSetForeground(display,graphics_content, (255 << 8));
    }else{
        XSetForeground(display,graphics_content, (255 << 16));
    }
    XFillRectangle(display, window, graphics_content, getX(), getY(), getWidth(), getHeight());
}

bool ButtonBool::isOn() {
    return on;
}
