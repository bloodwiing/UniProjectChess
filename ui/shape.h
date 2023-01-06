#ifndef CHESS_SHAPE_H
#define CHESS_SHAPE_H

#include "model/rect.h"

#include "settings/settings.h"

#define BOX_BORDER(name) wchar_t (name)(UserSettings * settings, Rect rect, int x, int y)

BOX_BORDER(getSingleBoxBorder);
BOX_BORDER(getDoubleBoxBorder);

void drawBox(UserSettings * settings, BOX_BORDER(border), Rect rect, int bg, int fg);

void drawSingleBox(UserSettings * settings, Rect rect, int bg, int fg);
void drawDoubleBox(UserSettings * settings, Rect rect, int bg, int fg);

#endif //CHESS_SHAPE_H
