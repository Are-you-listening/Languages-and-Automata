
#include "Gui.h"
#include "SongListWidget.h"
#include "ButtonCompare.h"
#include "SongListWidgetSingle.h"
#include "ButtonBool.h"
#include "PctScreen.h"
Gui::Gui() {
    display = XOpenDisplay((char *)0);


    window =XCreateSimpleWindow(display,
                                DefaultRootWindow(display),
                                0, 0, 1600, 1000, 0, 0, (40 << 16)+(40 << 8)+50);

    XSetStandardProperties(display, window, "I am Listening", NULL, None, NULL, 0, NULL);
    XSelectInput(display, window, ExposureMask | ButtonPressMask | PointerMotionMask);
    graphics_content=XCreateGC(display, window, 0, NULL);


    XClearWindow(display, window);
    XMapRaised(display, window);
}

void Gui::start() {


    bool has_selected = false;
    SongWidget* selected = nullptr;
    int draw_counter = 0;

    SongListWidget* song_list = new SongListWidget(20, 20, 250, 700, {
                                                                      new Song("midi_files/world-1-birabuto-remix-.mid",0),
                                                                      new Song("midi_files/world-1-birabuto-4-.mid",0),
                                                                      new Song("midi_files/HarryPotterPrologue(3).mid",0),
            new Song("midi_files/c0.mid",0),new Song("midi_files/c1.mid",0),
            new Song("midi_files/c2.mid",0),new Song("midi_files/c3.mid",0),
            new Song("midi_files/c4.mid",0),new Song("midi_files/c5.mid",0),new Song("midi_files/c0.mid",0),
                                                                      new Song("midi_files/c6.mid",0),
                                                                      new Song("midi_files/c7.mid",0),
            new Song("midi_files/c8.mid",0),
            new Song("midi_files/c9.mid",0),
                                                                      new Song("midi_files/c10.mid",0),
                                                                      new Song("midi_files/c11.mid",0),
                                                                      new Song("midi_files/c12.mid",0), new Song("midi_files/c13.mid",0)
    });
    SongListWidget* compare = new SongListWidget(400, 20, 250, 700, {});
    SongListWidgetSingle* single_song = new SongListWidgetSingle(400, 750, 250, 100, 1);

    ButtonBool* complement_button = new ButtonBool(660, 860, 49, 49, false);
    ButtonBool* reverse_button = new ButtonBool(660, 911, 49, 49, false);

    PctScreen* result = new PctScreen(700, 400, 200, 200);

    ButtonCompare* compare_button = new ButtonCompare(400, 860, 250, 100, compare, single_song, complement_button, reverse_button, result);



    vector<SongListWidget*> song_groups = {song_list, compare, single_song};
    vector<Button*> buttons = {compare_button, complement_button, reverse_button};


    XEvent event;
    while (true){
        XNextEvent(display, &event);

        if(event.type==Expose && event.xexpose.count==0) {
            for (auto& s_l: song_groups){
                s_l->draw(display, window,graphics_content);
            }
            for (auto& b: buttons){
                b->draw(display, window,graphics_content);
            }
            result->draw(display, window,graphics_content);
        }
        if(event.type==ButtonPress) {
            unsigned int button = event.xbutton.button;

            unsigned int mouse_x = event.xbutton.x;
            unsigned int mouse_y = event.xbutton.y;

            for (auto& b: buttons){
                if (b->isClicked(mouse_x, mouse_y)){
                    b->click();

                }
            }

            if (button == 4 || button == 5){
                for (auto& s_l: song_groups){
                    s_l->doScrolled(mouse_x, mouse_y, button == 4);
                    s_l->draw(display, window,graphics_content);
                }

            }

            if (button == 1){

                if (!has_selected){
                    for (auto& s_l: song_groups){
                        selected = s_l->select(mouse_x, mouse_y);
                        s_l->draw(display, window,graphics_content);
                        if (selected != nullptr){
                            selected->setPosMouse(mouse_x, mouse_y);
                            selected->draw(display, window,graphics_content);
                            has_selected = true;
                            break;
                        }
                    }


                }else{
                    for (auto& s_l: song_groups){
                        if (s_l->inWidget(mouse_x, mouse_y)){
                            selected->clear(display, window);

                            s_l->addSong(selected);
                            has_selected = false;
                            break;
                        }
                    }

                    selected->draw(display, window,graphics_content, false);
                    for (auto& s_l: song_groups){
                        s_l->draw(display, window,graphics_content);
                    }
                }



            }

            for (auto& b: buttons){
                b->draw(display, window,graphics_content);
            }
            result->draw(display, window,graphics_content);

        }

        if(event.type==MotionNotify && selected != nullptr && has_selected) {
            unsigned int mouse_x = event.xbutton.x;
            unsigned int mouse_y = event.xbutton.y;

            if (draw_counter == 6){
                draw_counter = 0;
                selected->setPosMouse(mouse_x, mouse_y);
                for (auto& s_l: song_groups){
                    s_l->draw(display, window,graphics_content);
                }

                for (auto& b: buttons){
                    b->draw(display, window,graphics_content);
                }

                result->draw(display, window,graphics_content);

                selected->draw(display, window,graphics_content, false);
            }else{
                draw_counter += 1;
            }




        }

    }

}
