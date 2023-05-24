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
#include "Song.h"
#endif
#include <vector>
class Gui {
public:
    Gui(const vector<Song*>& s);
    void start();
private:
    Display* display;
    Window window;
    GC graphics_content;
    vector<Song*> s;
};

#endif //TOG_GUI_H
