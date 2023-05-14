
#include "Gui.h"

Gui::Gui() {
    display = XOpenDisplay((char *)0);


    window =XCreateSimpleWindow(display,
                                DefaultRootWindow(display),
                                0, 0, 1000, 1000, 0, 0, 0);

    XSetStandardProperties(display, window, "Are You Listening", NULL, None, NULL, 0, NULL);
    XSelectInput(display, window, ExposureMask | ButtonPressMask);
    graphics_content=XCreateGC(display, window, 0, NULL);


    XClearWindow(display, window);
    XMapRaised(display, window);
}

void Gui::start() {
    XEvent event;
    while (true){
        XNextEvent(display, &event);

        if(event.type==Expose && event.xexpose.count==0) {

        }
        if(event.type==ButtonPress) {

            unsigned int mouse_x = event.xbutton.x;
            unsigned int mouse_y = event.xbutton.y;

        }

    }

}
