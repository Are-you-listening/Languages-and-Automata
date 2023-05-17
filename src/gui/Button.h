//
// Created by tibov on 17/05/23.
//

#ifndef TOG_BUTTON_H
#define TOG_BUTTON_H


class Button {
public:
    Button(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    bool isClicked(unsigned int mouse_x, unsigned int mouse_y);
    virtual void click() = 0;
private:
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;

};


#endif //TOG_BUTTON_H
