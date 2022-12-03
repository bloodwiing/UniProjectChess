#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include "../settings/settings.h"
#include "../model/piece.h"
#include "../model/team.h"

void renderText(wchar_t * format, ...);
void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...);
void renderPieceWithBackground(UserSettings * settings, Team * team, Piece * piece, int bg);
void renderPiece(UserSettings * settings, Team * team, Piece * piece);

void clearRect(int x, int y, int w, int h);

#endif //CHESS_RENDER_H
