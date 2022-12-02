#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include "../settings/settings.h"
#include "../model/piece.h"
#include "../model/team.h"

void renderText(wchar_t * format, ...);
void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...);
void renderPiece(UserSettings * settings, Team * team, Piece * piece);

#endif //CHESS_RENDER_H
