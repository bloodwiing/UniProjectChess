#ifndef CHESS_VALIDATION_H
#define CHESS_VALIDATION_H

#include "../abstract/defs.h"
#include "board.h"

void extendPath(Board * board, Path * start, int i, int j);
void deletePath(Tile * start, Path * path);
void trimPath(Tile * start, Path * path);

void createSinglePathing(Board * board, int x, int y);
void createBoardPathing(Board * board);

bool_t validateInBounds(Board * board, int x, int y);
bool_t validatePath(Board * board, int origin_x, int origin_y, int target_x, int target_y);

void updateTilePaths(Board * board, int x, int y);

bool_t isMoveCheckingSelf(Board * board, int origin_x, int origin_y, int target_x, int target_y);
bool_t isMoveValid(Board * board, int origin_x, int origin_y, int target_x, int target_y);
bool_t isTeamChecked(Board * board, Team * team);

#endif //CHESS_VALIDATION_H
