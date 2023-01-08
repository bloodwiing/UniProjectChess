#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include <stdarg.h>
#include <wchar.h>

#include "./generic/rect.h"
#include "./generic/shape.h"
#include "./generic/text.h"

#include "engine/board.h"
#include "engine/gamepiece.h"

#include "settings/settings.h"

#include "model/piece.h"
#include "model/scenario.h"
#include "model/team.h"

void renderPieceColoured(UserSettings * settings, int bg, int fg, Piece piece);
void renderPieceWithBackground(UserSettings * settings, Team team, Piece piece, int bg, bool_t invert);
void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg, bool_t invert);
void renderPiece(UserSettings * settings, Team team, Piece piece);

void renderBoard(Board * board, Rect draw_rect, Rect board_rect, int cur_x, int cur_y, bool_t with_coords, bool_t margin);
void renderBoardWithSelection(Board * board, Rect draw_rect, Rect board_rect, int cur_x, int cur_y, int sel_x, int sel_y, bool_t with_coords, bool_t margin);
void renderScenario(Scenario * scenario, UserSettings * settings, Rect draw_rect, Rect board_rect, bool_t with_coords, bool_t margin);

void setCursorAtTile(Rect draw_rect, Rect board_rect, int x, int y);
bool_t isTileVisible(Rect board_rect, int x, int y);

void ditherEffect();

void clearRect(Rect rect);

#endif //CHESS_RENDER_H
