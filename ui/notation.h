#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "engine/moveentry.h"

#include "settings/settings.h"

char * getFileNotation(uint8_t x);
char * getRankNotation(uint8_t y);

wchar_t * generateMoveNotation(UserSettings * settings, MoveEntry entry);

#endif //CHESS_NOTATION_H
