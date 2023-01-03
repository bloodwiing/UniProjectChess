#ifndef CHESS_MOVESET_H
#define CHESS_MOVESET_H

#include <stdio.h>
#include <stdint.h>

#include "./move.h"
#include "./specialmove.h"

typedef struct MoveSet {
    Move * moves;
    move_index_t move_count;

    Move * attacks;
    move_index_t attack_count;

    SpecialMove * specials;
    move_index_t special_count;
} MoveSet;

MoveSet createMoveSet();

void saveMoveSet(MoveSet move_set, FILE * stream);
MoveSet loadMoveSet(FILE * stream);
void printMoveSet(MoveSet move_set);

void addBasicMove(MoveSet * move_set, Move move);
void addAttackMove(MoveSet * move_set, Move move);
void addSpecialMove(MoveSet * move_set, SpecialMove move);

void addBasicAttackMove(MoveSet * move_set, Move move);

void appendBasicMoves(MoveSet * move_set, Move * moves, size_t n);
void appendAttackMoves(MoveSet * move_set, Move * moves, size_t n);
void appendSpecialMoves(MoveSet * move_set, SpecialMove * moves, size_t n);

void appendBasicAttackMoves(MoveSet * move_set, Move * moves, size_t n);

void freeMoveSet(MoveSet * move_set);

#endif
