#ifndef CHESS_TEXT_H
#define CHESS_TEXT_H

#include <stdarg.h>
#include <stdint.h>
#include <wchar.h>

#include "./rect.h"

#include "settings/settings.h"

size_t getWordLen(wchar_t * string);

void renderText(const wchar_t * format, ...);
void renderTextColoured(UserSettings * settings, int bg, int fg, const wchar_t * format, ...);
void renderTextColouredWrappedRect(UserSettings * settings, int bg, int fg, Rect rect, const wchar_t * format, ...);

#endif //CHESS_TEXT_H
