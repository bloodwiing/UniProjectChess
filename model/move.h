#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <stdint.h>
#include "../abstract/defs.h"
#include "vector.h"

typedef struct Move {
    Vector vector;
    bool_t repeat;
} Move;

void initMove(Move * move, Vector vector, bool_t repeat);
void initMoveRaw(Move * move, vec_t x, vec_t y, bool_t repeat);
void printMove(Move move);

void freeMove(Move * move);

#endif
