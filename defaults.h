#ifndef CHESS_DEFAULTS_H
#define CHESS_DEFAULTS_H

#include <stdint.h>
#include "piece.h"
#include "team.h"
#include "board.h"

Piece * createDefaultPawn(uint8_t team);
Piece * createDefaultRook(uint8_t team);
Piece * createDefaultKnight(uint8_t team);
Piece * createDefaultBishop(uint8_t team);
Piece * createDefaultQueen(uint8_t team);
Piece * createDefaultKing(uint8_t team);

Team * createDefaultTeamWhite();
Team * createDefaultTeamBlack();

Board * createDefaultBoard();

#endif
