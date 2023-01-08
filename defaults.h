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

/// Creates a classic pawn
/// \param team Team index that the pawn belongs to
Piece createDefaultPawn(uint8_t team);

/// Creates a classic rook
/// \param team Team index that the rook belongs to
Piece createDefaultRook(uint8_t team);

/// Creates a classic knight
/// \param team Team index that the knight belongs to
Piece createDefaultKnight(uint8_t team);

/// Creates a classic bishop
/// \param team Team index that the bishop belongs to
Piece createDefaultBishop(uint8_t team);

/// Creates a classic queen
/// \param team Team index that the queen belongs to
Piece createDefaultQueen(uint8_t team);

/// Creates a classic king
/// \param team Team index that the king belongs to
Piece createDefaultKing(uint8_t team);


/// Creates a classic white team with all the 6 piece types of team number 0
Team createDefaultTeamWhite();

/// Creates a classic black team with all the 6 piece types of team number 1
Team createDefaultTeamBlack();

/// Creates a classic chess scenario
Scenario * createDefaultScenario();


/// Saves the default scenario to ./scenario/classic.chess
/// \param settings User settings, only used to track the info to logs
void saveDefaultScenario(UserSettings * settings);

#endif
