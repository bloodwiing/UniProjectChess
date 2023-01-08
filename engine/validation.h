#ifndef CHESS_VALIDATION_H
#define CHESS_VALIDATION_H

#include "./board.h"
#include "./tile.h"
#include "./path.h"

#include "abstract/defs.h"

/// Resumes and extends the path until a collision occurs
void extendPath(Board * board, Path * start, ucoord_t i, ucoord_t j);

/// Completely removes the path from this point of it
void deletePath(Tile * start, Path * path);

/// Trims the path at the current point, making it end there and deleting everything that used to continue from it
void trimPath(Tile * start, Path * path);


/// Creates pathing for the single tile
void createSinglePathing(Board * board, ucoord_t x, ucoord_t y);

/// Creates pathing for the whole board
void createBoardPathing(Board * board);


/// Checks if the coordinates are within bounds
bool_t validateInBounds(Board * board, ucoord_t x, ucoord_t y);

/// Validates the path if the piece can even move from the origin to the target tile
bool_t validatePath(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y);


/// Updates pathing of the given coordinate
void updateTilePaths(Board * board, ucoord_t x, ucoord_t y);


/// Checks if the given tile is targeted by a team besides of the provided index
bool_t isTileDangerous(Tile * tile, team_index_t team);


/// Checks if the team has any possible moves (stalemate / checkmate)
bool_t hasTeamPossibleMoves(Board * board, Team * team);


/// Checks if the move will cause the team to check itself
bool_t isMoveCheckingSelf(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y);

/// Checks if the team is currently in check
bool_t isTeamChecked(Team * team);

/// Checks if the move causes or does not resolve the team's check
bool_t isTeamCheckedAfterMove(Board * board, Team * team, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y);

/// Checks if the move from the origin coordinates to the target coordinates is valid (optional path validation)
bool_t isMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y, bool_t validate_path);


/// Checks if the special move is valid from the given origin
bool_t isSpecialMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, SpecialMove * special_move);

#endif //CHESS_VALIDATION_H
