#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include <stdarg.h>
#include <wchar.h>

#include "engine/board.h"
#include "engine/gamepiece.h"

#include "settings/settings.h"

#include "model/piece.h"
#include "model/scenario.h"
#include "model/team.h"

void renderText(wchar_t * format, ...);
void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...);

void renderPieceWithBackground(UserSettings * settings, Team * team, Piece * piece, int bg);
void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg);
void renderPiece(UserSettings * settings, Team * team, Piece * piece);

void renderBoard(Board * board, int pos_x, int pos_y, int i, int j, int w, int h);
void renderBoardWithSelection(Board * board, int pos_x, int pos_y, int i, int j, int w, int h, int sel_x, int sel_y);
void renderScenario(Scenario * scenario, UserSettings * settings, int pos_x, int pos_y, int i, int j, int w, int h);

void clearRect(int x, int y, int w, int h);

#endif //CHESS_RENDER_H
