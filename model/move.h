#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <stdint.h>
#include "defs.h"

typedef struct Move {
    int8_t x, y;
    bool_t repeat;
} Move;

void initMove(Move * move, int8_t x, int8_t y, bool_t repeat);
void printMove(Move move);

#endif
