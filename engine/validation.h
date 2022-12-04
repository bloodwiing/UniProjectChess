#ifndef CHESS_VALIDATION_H
#define CHESS_VALIDATION_H

#include "../model/defs.h"
#include "board.h"

bool_t validatePieceAnyMove(Board * board, int x, int y, int i, int j);
bool_t validatePieceMove(Board * board, int x, int y, int i, int j);
bool_t validatePieceAttack(Board * board, int x, int y, int i, int j);

bool_t validateInBounds(Board * board, int x, int y);

#endif //CHESS_VALIDATION_H
