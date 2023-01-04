#ifndef CHESS_VALIDATION_H
#define CHESS_VALIDATION_H

#include "./board.h"
#include "./tile.h"
#include "./path.h"

#include "abstract/defs.h"

void extendPath(Board * board, Path * start, ucoord_t i, ucoord_t j);
void deletePath(Tile * start, Path * path);
void trimPath(Tile * start, Path * path);

void createSinglePathing(Board * board, ucoord_t x, ucoord_t y);
void createBoardPathing(Board * board);

bool_t validateInBounds(Board * board, ucoord_t x, ucoord_t y);
bool_t validatePath(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y);

void updateTilePaths(Board * board, ucoord_t x, ucoord_t y);

bool_t isTileDangerous(Tile * tile, team_index_t team);

bool_t isMoveCheckingSelf(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y);
bool_t isTeamChecked(Team * team);
bool_t isTeamCheckedAfterMove(Board * board, Team * team, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y);
bool_t isMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y, bool_t validate_path);

bool_t isSpecialMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, SpecialMove * special_move);

#endif //CHESS_VALIDATION_H
