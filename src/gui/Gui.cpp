
#include "Gui.h"
#include "SongListWidget.h"
Gui::Gui() {
    display = XOpenDisplay((char *)0);


    window =XCreateSimpleWindow(display,
                                DefaultRootWindow(display),
                                0, 0, 1600, 1000, 0, 0, 0);

    XSetStandardProperties(display, window, "I am Listening", NULL, None, NULL, 0, NULL);
    XSelectInput(display, window, ExposureMask | ButtonPressMask);
    graphics_content=XCreateGC(display, window, 0, NULL);


    XClearWindow(display, window);
    XMapRaised(display, window);
}

void Gui::start() {

    SongListWidget song_list(20, 20, 250, 800);
    XEvent event;
    while (true){
        XNextEvent(display, &event);

        if(event.type==Expose && event.xexpose.count==0) {
            song_list.draw(display, window,graphics_content);
        }
        if(event.type==ButtonPress) {
            unsigned int button = event.xbutton.button;

            unsigned int mouse_x = event.xbutton.x;
            unsigned int mouse_y = event.xbutton.y;

            if (button == 4 || button == 5){
                //scroll up
                song_list.doScrolled(mouse_x, mouse_y, button == 4);
                song_list.draw(display, window,graphics_content);
            }


        }

    }

}
