#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "../model/scenario.h"
#include "../settings/settings.h"
#include "tile.h"
#include "../abstract/exception.h"
#include <stdint.h>
#include <stdio.h>

typedef struct Board {
    UserSettings * user_settings;
    Scenario * scenario;

    uint8_t width, height;
    Tile ** tiles;

    Team * teams;
    uint8_t team_count;

    uint8_t active_turn;
} Board;

Board * createBoard(Scenario * scenario, UserSettings * settings, Exception * exception);
void saveBoard(Board * board, FILE * stream);
Board * loadBoard(UserSettings * settings, FILE * stream, Exception * exception);

Team * getTeam(Board * board, int index);
Team * getActiveTeam(Board * board);
Team * getPieceTeam(Board * board, Piece * piece);
Team * getGamePieceTeam(Board * board, GamePiece * piece);

Tile * getTile(Board * board, int x, int y);
GamePiece * getBoardGamePiece(Board * board, int x, int y);
Piece * getTilePiece(Board * board, Tile * tile);

void nextBoardTurn(Board * board);
void moveBoardGamePiece(Board * board, int from_x, int from_y, int to_x, int to_y);

#endif
