//
// Created by tibov on 17/05/23.
//

#ifndef TOG_SONGLISTWIDGETSINGLE_H
#define TOG_SONGLISTWIDGETSINGLE_H
#include "SongListWidget.h"

class SongListWidgetSingle: public SongListWidget {
public:
    SongListWidgetSingle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int max);
    bool inWidget(unsigned int mouse_x, unsigned int mouse_y);
    Song* getSong();
private:
    unsigned int max;
    unsigned int current;
};


#endif //TOG_SONGLISTWIDGETSINGLE_H
