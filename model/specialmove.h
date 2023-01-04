#ifndef CHESS_SPECIALMOVE_H
#define CHESS_SPECIALMOVE_H

#include <stdio.h>

#include "./specialdata.h"
#include "./teamdirection.h"
#include "./vector8.h"

#include "abstract/defs.h"

typedef struct SpecialMoveExtra {
    Vector8 piece_location;
    SpecialData data;
} SpecialMoveExtra;

typedef struct SpecialMove {
    SpecialData data;

    special_extra_index_t extra_count;
    SpecialMoveExtra * extra;
} SpecialMove;

SpecialMove createSpecialMove(SpecialData data);

void saveSpecialMove(SpecialMove move, FILE * stream);
SpecialMove loadSpecialMove(FILE * stream);
void printSpecialMove(SpecialMove special_move);

void addSpecialMoveExtra(SpecialMove * special_move, Vector8 piece_location, SpecialData data);

void normaliseSpecialMove(SpecialMove * special_move, TeamDirection direction);

void freeSpecialMove(SpecialMove * special_move);

#endif //CHESS_SPECIALMOVE_H
