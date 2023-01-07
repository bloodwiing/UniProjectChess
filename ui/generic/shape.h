#ifndef CHESS_SHAPE_H
#define CHESS_SHAPE_H

#include "./rect.h"

#include "settings/settings.h"

#define BOX_BORDER_SINGLE L"\0││──┌┐└┘"
#define BOX_BORDER_DOUBLE L"\0║║══╔╗╚╝"
#define BOX_BORDER_SINGLE_ASCII L"\0||--++++"
#define BOX_BORDER_DOUBLE_ASCII L"\0##==####"

typedef uint8_t box_space_t;
#define BOX_IGNORE_LEFT 0x1
#define BOX_IGNORE_RIGHT 0x2
#define BOX_IGNORE_TOP 0x4
#define BOX_IGNORE_BOTTOM 0x8
#define BOX_IGNORE_TOP_LEFT 0x10
#define BOX_IGNORE_TOP_RIGHT 0x20
#define BOX_IGNORE_BOTTOM_LEFT 0x40
#define BOX_IGNORE_BOTTOM_RIGHT 0x80

wchar_t getBoxBorder(wchar_t * data, Rect rect, int x, int y, box_space_t flag);

void drawBox(UserSettings * settings, wchar_t * border, Rect rect, int bg, int fg);
void drawBoxContained(UserSettings * settings, wchar_t * border, Rect rect, int bg, int fg);

void drawSingleBox(UserSettings * settings, Rect rect, int bg, int fg);
void drawDoubleBox(UserSettings * settings, Rect rect, int bg, int fg);

void drawSingleBoxContained(UserSettings * settings, Rect rect, int bg, int fg);
void drawDoubleBoxContained(UserSettings * settings, Rect rect, int bg, int fg);

#endif //CHESS_SHAPE_H
