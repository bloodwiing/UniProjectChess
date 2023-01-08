#ifndef CHESS_RECT_H
#define CHESS_RECT_H

#include "settings/settings.h"

#define RECT_3X3(x, y) createRect(x, y, 3, 3)
#define RECT_LINE(x, y, w) createRect(x, y, w, 1)

typedef struct Rect {
    int x, y, width, height;
} Rect;

/// Creates a Rect with the given parameters
Rect createRect(int x, int y, int width, int height);

/// Offsets the Rect / adds to each of its fields the provided offset
Rect offsetRect(Rect rect, int x, int y, int width, int height);

/// Insets the rect with the given amount<br>
/// A positive number will make it shrink equally from every side, a negative will cause it to grow
Rect insetRect(Rect rect, int amount);


/// Calculates the needed offset for a Rect to be centered within a containing rect
Rect centerRectInRect(int inner_width, int inner_height, Rect outer);

/// Creates a rect of the given size that's centered to the console size bounds
Rect getScreenCenteredRect(int width, int height);


/// Shrinks the rect and clamps the position so that it doesn't exceed the provided bounds
Rect fitRect(Rect rect, int bound_width, int bound_height);


/// A helper function to report a Rect to the log
/// \param settings Display Settings
/// \param module The name of the module that's logging
/// \param name The name of the variable
/// \param rect The value of the variable
void logDebugRect(UserSettings * settings, const wchar_t * module, const char * name, Rect rect);


/// A helper function to get a Rect that represents the Console's size
Rect getScreenRect();

#endif //CHESS_RECT_H
