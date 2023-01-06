#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include <stdarg.h>
#include <wchar.h>

#include "engine/board.h"
#include "engine/gamepiece.h"

#include "settings/settings.h"

#include "model/piece.h"
#include "model/rect.h"
#include "model/scenario.h"
#include "model/team.h"

void renderText(wchar_t * format, ...);
void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...);

void renderPieceColoured(UserSettings * settings, int bg, int fg, Piece piece);
void renderPieceWithBackground(UserSettings * settings, Team team, Piece piece, int bg);
void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg);
void renderPiece(UserSettings * settings, Team team, Piece piece);

void renderBoard(Board * board, Rect draw_rect, Rect board_rect);
void renderBoardWithSelection(Board * board, Rect draw_rect, Rect board_rect, int sel_x, int sel_y);
void renderScenario(Scenario * scenario, UserSettings * settings, Rect draw_rect, Rect board_rect);

void setCursorAtTile(Rect draw_rect, Rect board_rect, int x, int y);
bool_t isTileVisible(Rect board_rect, int x, int y);

void ditherEffect();

void clearRect(Rect rect);

#endif //CHESS_RENDER_H
