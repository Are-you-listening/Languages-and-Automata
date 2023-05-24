//
// Created by tibov on 17/05/23.
//

#include "SongListWidgetSingle.h"

SongListWidgetSingle::SongListWidgetSingle(unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                                           unsigned int max): SongListWidget(x, y, width, height, {}), max(max) {
    current = 0;
}

bool SongListWidgetSingle::inWidget(unsigned int mouse_x, unsigned int mouse_y) {
    return SongListWidget::inWidget(mouse_x, mouse_y) && current < max;
}

Song *SongListWidgetSingle::getSong() {
    if (getSongs().size()> 0){
        Song* s = getSongs()[0]->getSong();
        return s;
    }else{
        return nullptr;
    }
}
void SongListWidgetSingle::doScrolled(unsigned int mouse_x, unsigned int mouse_y, bool up){}
