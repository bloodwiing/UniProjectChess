#include "shape.h"

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "ui/con_lib.h"
#include "ui/render.h"

wchar_t getBoxBorder(wchar_t * data, Rect rect, int x, int y) {
    x -= rect.x;
    y -= rect.y;

    if (y < 0 || y > rect.height)
        x = -1;
    else if (x < 0 || x > rect.width)
        y = -1;

    x = x == 0 ? 1 : ( x == rect.width ? 2 : 0 );
    y = y == 0 ? 1 : ( y == rect.height ? 2 : 0 );

    return data[x | (y << 2)];
}

void drawBox(UserSettings * settings, wchar_t * border, Rect rect, int bg, int fg) {
    wchar_t * horizontal = calloc(rect.width - 1, sizeof(wchar_t));
    wmemset(horizontal, getBoxBorder(border, RECT_3X3(0, 0), 1, 0), rect.width - 1);

    con_set_pos(rect.x + 1, rect.y);
    renderTextColoured(settings, bg, fg, L"%s", horizontal);
    con_set_pos(rect.x + 1, rect.y + rect.height);
    renderTextColoured(settings, bg, fg, L"%s", horizontal);

    free(horizontal);

    for (int i = rect.y; i <= rect.height + rect.y; i++) {
        con_set_pos(rect.x, i);
        renderTextColoured(settings, bg, fg, L"%lc", getBoxBorder(border, rect, rect.x, i));
        con_set_pos(rect.x + rect.width, i);
        renderTextColoured(settings, bg, fg, L"%lc", getBoxBorder(border, rect, rect.x + rect.width, i));
    }
}

void drawSingleBox(UserSettings * settings, Rect rect, int bg, int fg) {
    drawBox(settings, settings->display.unicode ? BOX_BORDER_SINGLE : BOX_BORDER_SINGLE_ASCII, rect, bg, fg);
}

void drawDoubleBox(UserSettings * settings, Rect rect, int bg, int fg) {
    drawBox(settings, settings->display.unicode ? BOX_BORDER_DOUBLE : BOX_BORDER_DOUBLE_ASCII, rect, bg, fg);
}
