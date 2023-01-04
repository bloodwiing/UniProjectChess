#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <stdio.h>
#include <stdint.h>

#include "model/move/moveset.h"

#include "abstract/defs.h"
#include "utils/encoding.h"
#include "teamdirection.h"

#define PIECE_UNICODE_LENGTH 4
#define PIECE_NAME_LENGTH 20

typedef struct Piece {
    char name[PIECE_NAME_LENGTH];

    bool_t upgradable;
    bool_t protect;

    team_index_t team;

    char symbol;
    wchar_t unicode[PIECE_UNICODE_LENGTH];

    MoveSet move_set;
} Piece;

Piece createPiece(char * name, wchar_t * unicode, char symbol, bool_t upgradable, bool_t protect, team_index_t team, MoveSet move_set);
void savePiece(Piece piece, FILE * stream);
Piece loadPiece(FILE * stream);
void printPiece(Piece piece);

void freePiece(Piece * piece);

#endif
