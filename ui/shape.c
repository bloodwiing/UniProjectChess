#include "shape.h"

#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "ui/con_lib.h"
#include "ui/render.h"

BOX_BORDER(getSingleBoxBorder) {
    x -= rect.x;
    y -= rect.y;

    x = x == 0 ? 1 : ( x == rect.width ? 2 : 0 );
    y = y == 0 ? 1 : ( y == rect.height ? 2 : 0 );

    if (x == 0 && y != 0)
        return settings->display.unicode ? L'─' : L'-';

    if (x == 2) {
        switch (y) {
            case 2:
                return settings->display.unicode ? L'┘' : L'+';
            case 1:
                return settings->display.unicode ? L'┐' : L'+';
            default:
                return settings->display.unicode ? L'│' : L'|';
        }
    }

    if (x == 1) {
        switch (y) {
            case 2:
                return settings->display.unicode ? L'└' : L'+';
            case 1:
                return settings->display.unicode ? L'┌' : L'+';
            default:
                return settings->display.unicode ? L'│' : L'|';
        }
    }

    return L'\0';
}

BOX_BORDER(getDoubleBoxBorder) {
    x -= rect.x;
    y -= rect.y;

    x = x == 0 ? 1 : ( x == rect.width ? 2 : 0 );
    y = y == 0 ? 1 : ( y == rect.height ? 2 : 0 );

    if (x == 0 && y != 0)
        return settings->display.unicode ? L'═' : L'=';

    if (x == 2) {
        switch (y) {
            case 2:
                return settings->display.unicode ? L'╝' : L'#';
            case 1:
                return settings->display.unicode ? L'╗' : L'#';
            default:
                return settings->display.unicode ? L'║' : L'#';
        }
    }

    if (x == 1) {
        switch (y) {
            case 2:
                return settings->display.unicode ? L'╚' : L'#';
            case 1:
                return settings->display.unicode ? L'╔' : L'#';
            default:
                return settings->display.unicode ? L'║' : L'#';
        }
    }

    return L'\0';
}

void drawBox(UserSettings * settings, BOX_BORDER(border), Rect rect, int bg, int fg) {
    wchar_t * horizontal = calloc(rect.width - 1, sizeof(wchar_t));
    wmemset(horizontal, border(settings, RECT_3X3(0, 0), 1, 0), rect.width - 1);

    con_set_pos(rect.x + 1, rect.y);
    renderTextColoured(settings, bg, fg, L"%s", horizontal);
    con_set_pos(rect.x + 1, rect.y + rect.height);
    renderTextColoured(settings, bg, fg, L"%s", horizontal);

    free(horizontal);

    for (int i = rect.y; i <= rect.height + rect.y; i++) {
        con_set_pos(rect.x, i);
        renderTextColoured(settings, bg, fg, L"%lc", border(settings, rect, rect.x, i));
        con_set_pos(rect.x + rect.width, i);
        renderTextColoured(settings, bg, fg, L"%lc", border(settings, rect, rect.x + rect.width, i));
    }
}

void drawSingleBox(UserSettings * settings, Rect rect, int bg, int fg) {
    drawBox(settings, getSingleBoxBorder, rect, bg, fg);
}

void drawDoubleBox(UserSettings * settings, Rect rect, int bg, int fg) {
    drawBox(settings, getDoubleBoxBorder, rect, bg, fg);
}
