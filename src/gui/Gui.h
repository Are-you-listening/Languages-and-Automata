//
// Created by tibov on 14/05/23.
//

#ifndef TOG_GUI_H
#define TOG_GUI_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#ifdef __unix__
#include <cstdio>
#include <cstdlib>
#endif




class Gui {
public:
    Gui();
    void start();

public:
private:
    Display* display;
    Window window;
    GC graphics_content;
};


#endif //TOG_GUI_H
