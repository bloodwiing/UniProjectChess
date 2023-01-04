#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <stdint.h>
#include <stdio.h>

#include "abstract/defs.h"

#include "model/teamdirection.h"
#include "model/vector8.h"

typedef struct Move {
    bool_t normalised;

    Vector8 vector;
    bool_t repeat;
} Move;

Move createMove(Vector8 vector, bool_t repeat, bool_t normalised);
Move createMoveRaw(coord_t x, coord_t y, bool_t repeat, bool_t normalised);

void saveMove(Move move, FILE * stream);
Move loadMove(FILE * stream);

void normaliseMove(Move * move, TeamDirection direction);

void printMove(Move move);

#endif
