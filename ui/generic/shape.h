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

/// Gets a wide char based on if the Point(X,Y) is on an edge or corner of the Rect
/// \param data The visual box data (use provided macros)
/// \param rect The rect to test
/// \param x The point's X
/// \param y The point's Y
/// \param flag A conditional flag of which sides to ignore and render as blank (use macros with an OR operation)
wchar_t getBoxBorder(wchar_t * data, Rect rect, int x, int y, box_space_t flag);

/// Draws a coloured box based on Rect to the screen
/// \param settings Display settings
/// \param border The visual box data (use provided macros)
/// \param rect The size and position of the box
/// \param bg Background colour
/// \param fg Foreground colour
void drawBox(UserSettings * settings, wchar_t * border, Rect rect, int bg, int fg);

/// Draws a coloured box based on Rect to the screen, alternative to drawBox() that sizes the box down to fit within the rect
/// \param settings Display settings
/// \param border The visual box data (use provided macros)
/// \param rect The size and position of the box
/// \param bg Background colour
/// \param fg Foreground colour
void drawBoxContained(UserSettings * settings, wchar_t * border, Rect rect, int bg, int fg);

/// Draws a coloured single-line box based on Rect to the screen
/// \param settings Display settings
/// \param rect The size and position of the box
/// \param bg Background colour
/// \param fg Foreground colour
void drawSingleBox(UserSettings * settings, Rect rect, int bg, int fg);

/// Draws a coloured double-line box based on Rect to the screen
/// \param settings Display settings
/// \param rect The size and position of the box
/// \param bg Background colour
/// \param fg Foreground colour
void drawDoubleBox(UserSettings * settings, Rect rect, int bg, int fg);

/// Draws a coloured single-line box based on Rect to the screen, alternative to drawSingleBox() that sizes the box down to fit within the rect
/// \param settings Display settings
/// \param rect The size and position of the box
/// \param bg Background colour
/// \param fg Foreground colour
void drawSingleBoxContained(UserSettings * settings, Rect rect, int bg, int fg);

/// Draws a coloured double-line box based on Rect to the screen, alternative to drawDoubleBox() that sizes the box down to fit within the rect
/// \param settings Display settings
/// \param rect The size and position of the box
/// \param bg Background colour
/// \param fg Foreground colour
void drawDoubleBoxContained(UserSettings * settings, Rect rect, int bg, int fg);

#endif //CHESS_SHAPE_H
