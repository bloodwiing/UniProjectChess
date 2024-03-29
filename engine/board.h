#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <stdint.h>
#include <stdio.h>

#include "./tile.h"

#include "abstract/exception.h"

#include "ui/generic/rect.h"
#include "model/scenario.h"

#include "settings/settings.h"

typedef uint16_t tile_index_t;

typedef struct Board {
    UserSettings * user_settings;
    Scenario * scenario;

    ucoord_t width, height;
    Tile ** tiles;

    Team * teams;
    team_index_t team_count;

    team_index_t active_turn;
} Board;

Board * createBoard(Scenario * scenario, UserSettings * settings, Exception * exception);
void saveBoard(Board * board, FILE * stream);
Board * loadBoard(UserSettings * settings, FILE * stream, Exception * exception);

Team * getTeam(Board * board, team_index_t index);
Team * getActiveTeam(Board * board);
Team * getPieceTeam(Board * board, Piece * piece);
Team * getGamePieceTeam(Board * board, GamePiece * piece);

Tile * getTile(Board * board, ucoord_t x, ucoord_t y);
GamePiece * getBoardGamePiece(Board * board, ucoord_t x, ucoord_t y);
Piece * getTilePiece(Board * board, Tile * tile);

Rect getBoardRect(Board * board, int x, int y);
Rect getBoardRectWithinRect(Board * board, int x, int y, Rect rect);
Rect getBoardCenteredRect(Board * board, Rect rect, int cursor_x, int cursor_y);

void nextBoardTurn(Board * board);
void moveBoardGamePiece(Board * board, ucoord_t from_x, ucoord_t from_y, ucoord_t to_x, ucoord_t to_y);

bool_t canPromoteGamePiece(Board * board, GamePiece * game_piece);
void promoteGamePiece(Board * board, GamePiece * game_piece, piece_index_t new_type);
void handleGamePiecePromotion(Board * board, GamePiece * game_piece);

Board * cloneBoard(Board * board);
void freeBoard(Board * board, bool_t free_scenario);


#endif
