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
void initMoveRaw(Move * move, int8_t x, int8_t y, bool_t repeat);
void printMove(Move move);

#endif
