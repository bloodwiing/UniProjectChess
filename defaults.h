#ifndef CHESS_DEFAULTS_H
#define CHESS_DEFAULTS_H

#include <stdint.h>

#include "model/piece.h"
#include "model/scenario.h"
#include "model/team.h"

#define DEFAULT_TEAM_TYPE_WHITE 0
#define DEFAULT_TEAM_TYPE_BLACK 1

#define DEFAULT_PIECE_TYPE_PAWN 0
#define DEFAULT_PIECE_TYPE_ROOK 1
#define DEFAULT_PIECE_TYPE_KNIGHT 2
#define DEFAULT_PIECE_TYPE_BISHOP 3
#define DEFAULT_PIECE_TYPE_QUEEN 4
#define DEFAULT_PIECE_TYPE_KING 5

Piece createDefaultPawn(uint8_t team);
Piece createDefaultRook(uint8_t team);
Piece createDefaultKnight(uint8_t team);
Piece createDefaultBishop(uint8_t team);
Piece createDefaultQueen(uint8_t team);
Piece createDefaultKing(uint8_t team);

Team createDefaultTeamWhite();
Team createDefaultTeamBlack();

Scenario * createDefaultScenario();

void saveDefaultScenario(UserSettings * settings);

#endif
