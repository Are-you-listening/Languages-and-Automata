//
// Created by tibov on 18/05/23.
//

#ifndef TOG_BUTTONBOOL_H
#define TOG_BUTTONBOOL_H
#include "Button.h"

class ButtonBool: public Button{
public:
    ButtonBool(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool on);
    void click();
    void draw(Display* display, Window window, GC graphics_content);
    bool isOn();
private:
    bool on;

};


#endif //TOG_BUTTONBOOL_H
