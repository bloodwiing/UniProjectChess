#ifndef CHESS_SHAPE_H
#define CHESS_SHAPE_H

#include "model/rect.h"

#include "settings/settings.h"

#define BOX_BORDER_SINGLE L"\0││ ─┌┐ ─└┘    "
#define BOX_BORDER_DOUBLE L"\0║║ ═╔╗ ═╚╝    "
#define BOX_BORDER_SINGLE_ASCII L"\0|| -++ -++    "
#define BOX_BORDER_DOUBLE_ASCII L"\0## =## =##    "

wchar_t getBoxBorder(wchar_t * data, Rect rect, int x, int y);

void drawBox(UserSettings * settings, wchar_t * border, Rect rect, int bg, int fg);

void drawSingleBox(UserSettings * settings, Rect rect, int bg, int fg);
void drawDoubleBox(UserSettings * settings, Rect rect, int bg, int fg);

#endif //CHESS_SHAPE_H
