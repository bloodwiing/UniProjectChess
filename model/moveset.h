#ifndef CHESS_MOVESET_H
#define CHESS_MOVESET_H

#include <stdio.h>
#include <stdint.h>
#include "move.h"
#include "specialmove.h"

typedef struct MoveSet {
    Move * moves;
    move_index_t move_count;

    Move * attacks;
    move_index_t attack_count;

    SpecialMove * specials;
    move_index_t special_count;
} MoveSet;

void initMoveSet(MoveSet * move_set, Move * moves, move_index_t move_count, Move * attacks, move_index_t attack_count, SpecialMove * specials, move_index_t special_count);
void saveMoveSet(MoveSet * move_set, FILE * stream);
MoveSet * loadMoveSet(FILE * stream);
void printMoveSet(MoveSet * move_set);

void freeMoveSet(MoveSet * move_set);

#endif
