//
// Created by tibov on 18/05/23.
//

#ifndef TOG_PCTSCREEN_H
#define TOG_PCTSCREEN_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include "stack"
#ifdef __unix__
#include <cstdio>
#include <cstdlib>
#endif

using namespace std;

class PctScreen {
public:
    PctScreen(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    virtual void draw(Display* display, Window window, GC graphics_content);

    void setPct(double pct);

private:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    double pct;
};

#endif //TOG_PCTSCREEN_H
