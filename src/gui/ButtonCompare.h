//
// Created by tibov on 17/05/23.
//

#ifndef TOG_BUTTONCOMPARE_H
#define TOG_BUTTONCOMPARE_H
#include "Button.h"
#include "SongListWidget.h"
#include "SongListWidgetSingle.h"
#include "ButtonBool.h"
class ButtonCompare: public Button {
public:
    ButtonCompare(unsigned int x, unsigned int y, unsigned int width, unsigned int height, SongListWidget* compare, SongListWidgetSingle* song_box, ButtonBool* comp, ButtonBool* reverse);
    void click();
private:
    SongListWidget* compare;
    SongListWidgetSingle* song_box;
    ButtonBool* complement_button;
    ButtonBool* reverse_button;
};


#endif //TOG_BUTTONCOMPARE_H
