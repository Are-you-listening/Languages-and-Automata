//
// Created by tibov on 18/05/23.
//

#include "PctScreen.h"
#include <cmath>
#include "string"
PctScreen::PctScreen(unsigned int x, unsigned int y, unsigned int width, unsigned int height): x(x), y(y), width(width), height(height) {
    pct = 0;
}

void PctScreen::draw(Display *display, Window window, GC graphics_content) {
    XSetForeground(display,graphics_content, (80 << 16) + (80 << 8) + 80);
    XFillRectangle(display, window, graphics_content, x, y, width, height);

    XFontStruct* font_info = XLoadQueryFont(display, "-Sony-Fixed-Medium-R-Normal--24-170-100-100-C-120-JISX0201.1976-0");
    XSetForeground(display,graphics_content, 0);
    XSetFont(display, graphics_content, font_info->fid);

    if (pct > 50){
        XSetForeground(display,graphics_content, (0 << 16) + (255 << 8) + 0);
    }else{
        XSetForeground(display,graphics_content, (255 << 16) + (0 << 8) + 0);
    }

    string pct_s = to_string(lround(pct));
    pct_s += "%";
    XDrawString(display, window, graphics_content, x+lround(width/2.5), y+height/2, &pct_s[0], min(4, (int) pct_s.size()));
}

void PctScreen::setPct(double pct) {
    PctScreen::pct = pct;
}
