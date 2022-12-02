#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include "../settings/settings.h"
#include "../model/piece.h"
#include "../model/team.h"

void renderText(wchar_t * text);
void renderTextColoured(UserSettings * settings, int fg, int bg, wchar_t * text);
void renderPiece(UserSettings * settings, Team * team, Piece * piece);

#endif //CHESS_RENDER_H
