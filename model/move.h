#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <stdint.h>
#include <stdio.h>

#include "./vector8.h"

#include "abstract/defs.h"

typedef struct Move {
    Vector8 vector;
    bool_t repeat;
} Move;

Move createMove(Vector8 vector, bool_t repeat);
Move createMoveRaw(coord_t x, coord_t y, bool_t repeat);

void saveMove(Move move, FILE * stream);
Move loadMove(FILE * stream);

void printMove(Move move);

#endif
