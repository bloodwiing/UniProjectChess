#ifndef CHESS_SPECIALMOVE_H
#define CHESS_SPECIALMOVE_H

#include "../model/defs.h"
#include <stdio.h>

typedef struct SpecialMove {
    int8_t x, y;

    bool_t first_move;

    bool_t vulnerable;
    int8_t vul_x, vul_y;
} SpecialMove;

void initSpecialMove(SpecialMove * special_move, int8_t x, int8_t y, bool_t first_move, bool_t vulnerable, int8_t vul_x, int8_t vul_y);
void initSpecialMoveSafe(SpecialMove * special_move, int8_t x, int8_t y, bool_t first_move);
void initSpecialMoveVulnerable(SpecialMove * special_move, int8_t x, int8_t y, bool_t first_move, int8_t vul_x, int8_t vul_y);
void saveSpecialMove(SpecialMove * special_move, FILE * stream);
SpecialMove * loadSpecialMove(FILE * stream);
void printSpecialMove(SpecialMove special_move);

#endif //CHESS_SPECIALMOVE_H
