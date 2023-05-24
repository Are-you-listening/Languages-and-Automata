//
// Created by tibov on 17/05/23.
//

#include "Button.h"

Button::Button(unsigned int x, unsigned int y, unsigned int width, unsigned int height): x(x), y(y), width(width), height(height) {}

bool Button::isClicked(unsigned int mouse_x, unsigned int mouse_y) {
    bool in_x = x <= mouse_x && mouse_x <= x+width;
    bool in_y = y <= mouse_y && mouse_y <= y+height;
    return in_x && in_y;
}

void Button::draw(Display *display, Window window, GC graphics_content) {
    XSetForeground(display,graphics_content, (255 << 16) + (215 << 8) + 0);
    XFillRectangle(display, window, graphics_content, x, y, width, height);
}

unsigned int Button::getX() const {
    return x;
}

unsigned int Button::getY() const {
    return y;
}

unsigned int Button::getWidth() const {
    return width;
}

unsigned int Button::getHeight() const {
    return height;
}
