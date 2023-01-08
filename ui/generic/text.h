#ifndef CHESS_TEXT_H
#define CHESS_TEXT_H

#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>

#include "./rect.h"

#include "settings/settings.h"

/// Gets the amount of characters until a space of the end of the string
size_t getWordLen(wchar_t * string);

/// Renders text to screen
void renderText(const wchar_t * format, ...);

/// Renders coloured text to screen
/// \param settings Display Settings
/// \param bg Background colour
/// \param fg Foreground colour
void renderTextColoured(UserSettings * settings, int bg, int fg, const wchar_t * format, ...);

/// Renders coloured text wrapped in a region on a screen
/// \param settings Display Settings
/// \param bg Background colour
/// \param fg Foreground colour
/// \param rect The rect that will contain the text
void renderTextColouredWrappedRect(UserSettings * settings, int bg, int fg, Rect rect, const wchar_t * format, ...);

#endif //CHESS_TEXT_H
