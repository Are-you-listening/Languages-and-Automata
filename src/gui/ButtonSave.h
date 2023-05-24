//
// Created by tibov on 23/05/23.
//

#ifndef TOG_BUTTONSAVE_H
#define TOG_BUTTONSAVE_H

#include "Button.h"
#include "SongListWidgetSingle.h"

class ButtonSave: public Button{
public:
    ButtonSave(unsigned int x, unsigned int y, unsigned int width, unsigned int height, SongListWidgetSingle* song_box);
    void click();
    void draw(Display* display, Window window, GC graphics_content);
private:
    SongListWidgetSingle* song_box;
};


#endif //TOG_BUTTONSAVE_H
