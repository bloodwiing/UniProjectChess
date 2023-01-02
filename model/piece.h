#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <stdio.h>
#include <stdint.h>
#include "../abstract/defs.h"
#include "moveset.h"
#include "../utils/encoding.h"

#define PIECE_UNICODE_LENGTH 4
#define PIECE_NAME_LENGTH 20

typedef struct Piece {
    char name[PIECE_NAME_LENGTH];

    bool_t upgradable;
    bool_t protect;

    uint8_t team;

    char symbol;
    wchar_t unicode[PIECE_UNICODE_LENGTH];

    MoveSet * move_set;
} Piece;

void initPiece(Piece * piece, char * name, wchar_t * unicode, char symbol, bool_t upgradable, bool_t protect, uint8_t team, MoveSet * move_set);
void savePiece(Piece * piece, FILE * stream);
void loadPiece(Piece * piece, FILE * stream);
void printPiece(Piece * piece);

#endif
