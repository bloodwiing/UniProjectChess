#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <stdio.h>
#include <stdint.h>
#include "defs.h"
#include "moveset.h"

typedef struct Piece {
    char name[20];

    wchar_t unicode[4];
    char symbol;

    bool_t upgradable;
    bool_t protect;

    uint8_t team;

    MoveSet * move_set;
} Piece;

void initPiece(Piece * piece, char * name, wchar_t * unicode, char symbol, bool_t upgradable, bool_t protect, uint8_t team, MoveSet * move_set);
void savePiece(Piece * piece, FILE * stream);
void loadPiece(Piece * piece, FILE * stream);
void printPiece(Piece * piece);

#endif
