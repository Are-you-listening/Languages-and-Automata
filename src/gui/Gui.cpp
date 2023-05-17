
#include "Gui.h"
#include "SongListWidget.h"
Gui::Gui() {
    display = XOpenDisplay((char *)0);


    window =XCreateSimpleWindow(display,
                                DefaultRootWindow(display),
                                0, 0, 1600, 1000, 0, 0, 0);

    XSetStandardProperties(display, window, "I am Listening", NULL, None, NULL, 0, NULL);
    XSelectInput(display, window, ExposureMask | ButtonPressMask | PointerMotionMask);
    graphics_content=XCreateGC(display, window, 0, NULL);


    XClearWindow(display, window);
    XMapRaised(display, window);
}

void Gui::start() {

    bool has_selected = false;
    SongWidget* selected = nullptr;

    SongListWidget song_list(20, 20, 250, 800, {new Song("midi_files/(metalslug1) (1).mid",0)});
    SongListWidget compare(400, 20, 250, 800, {});

    vector<SongListWidget> song_groups = {song_list, compare};


    XEvent event;
    while (true){
        XNextEvent(display, &event);

        if(event.type==Expose && event.xexpose.count==0) {
            for (auto& s_l: song_groups){
                s_l.draw(display, window,graphics_content);
            }
        }
        if(event.type==ButtonPress) {
            unsigned int button = event.xbutton.button;

            unsigned int mouse_x = event.xbutton.x;
            unsigned int mouse_y = event.xbutton.y;

            if (button == 4 || button == 5){
                for (auto& s_l: song_groups){
                    s_l.doScrolled(mouse_x, mouse_y, button == 4);
                    s_l.draw(display, window,graphics_content);
                }

            }

            if (button == 1){

                if (!has_selected){
                    for (auto& s_l: song_groups){
                        selected = s_l.select(mouse_x, mouse_y);
                        s_l.draw(display, window,graphics_content);
                        if (selected != nullptr){
                            selected->setPosMouse(mouse_x, mouse_y);
                            selected->draw(display, window,graphics_content);
                            has_selected = true;
                            break;
                        }
                    }


                }else{
                    for (auto& s_l: song_groups){
                        if (s_l.inWidget(mouse_x, mouse_y)){
                            selected->clear(display, window);

                            s_l.addSong(selected);
                            has_selected = false;
                            break;
                        }
                    }


                    selected->draw(display, window,graphics_content, false);
                    for (auto& s_l: song_groups){
                        s_l.draw(display, window,graphics_content);
                    }
                }

            }

        }

        if(event.type==MotionNotify && selected != nullptr && has_selected) {
            unsigned int mouse_x = event.xbutton.x;
            unsigned int mouse_y = event.xbutton.y;

            selected->setPosMouse(mouse_x, mouse_y);
            for (auto& s_l: song_groups){
                s_l.draw(display, window,graphics_content);
            }
            selected->draw(display, window,graphics_content, false);
        }

    }

}
