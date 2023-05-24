//
// Created by tibov on 17/05/23.
//

#ifndef TOG_BUTTON_H
#define TOG_BUTTON_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include "stack"
#ifdef __unix__
#include <cstdio>
#include <cstdlib>
#endif

class Button {
public:
    Button(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    bool isClicked(unsigned int mouse_x, unsigned int mouse_y);
    virtual void click() = 0;
    virtual void draw(Display* display, Window window, GC graphics_content);
private:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
protected:
    unsigned int getX() const;

    unsigned int getY() const;

    unsigned int getWidth() const;

    unsigned int getHeight() const;
};

#endif //TOG_BUTTON_H
