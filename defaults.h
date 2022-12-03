#ifndef CHESS_DEFAULTS_H
#define CHESS_DEFAULTS_H

#include <stdint.h>
#include "model/piece.h"
#include "model/team.h"
#include "model/scenario.h"

void initDefaultPawn(Piece * piece, uint8_t team);
void initDefaultRook(Piece * piece, uint8_t team);
void initDefaultKnight(Piece * piece, uint8_t team);
void initDefaultBishop(Piece * piece, uint8_t team);
void initDefaultQueen(Piece * piece, uint8_t team);
void initDefaultKing(Piece * piece, uint8_t team);

Team * createDefaultTeamWhite();
Team * createDefaultTeamBlack();

Scenario * createDefaultScenario();

#endif
