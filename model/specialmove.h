#ifndef CHESS_SPECIALMOVE_H
#define CHESS_SPECIALMOVE_H

#include <stdio.h>

#include "./vector8.h"

#include "abstract/defs.h"

typedef struct SpecialMove {
    Vector8 vector;
    bool_t is_first_move;

    bool_t is_vulnerable;
    Vector8 vulnerable;
} SpecialMove;

SpecialMove createSpecialMove(Vector8 vector, bool_t is_first_move, bool_t is_vulnerable, Vector8 vulnerable);
SpecialMove createSpecialMoveSafe(Vector8 vector, bool_t is_first_move);
SpecialMove createSpecialMoveVulnerable(Vector8 vector, bool_t is_first_move, Vector8 vulnerable);

SpecialMove createSpecialMoveRaw(coord_t x, coord_t y, bool_t is_first_move, bool_t is_vulnerable, coord_t vul_x, coord_t vul_y);
SpecialMove createSpecialMoveSafeRaw(coord_t x, coord_t y, bool_t is_first_move);
SpecialMove createSpecialMoveVulnerableRaw(coord_t x, coord_t y, bool_t is_first_move, coord_t vul_x, coord_t vul_y);

void saveSpecialMove(SpecialMove move, FILE * stream);
SpecialMove loadSpecialMove(FILE * stream);

void printSpecialMove(SpecialMove special_move);

#endif //CHESS_SPECIALMOVE_H
