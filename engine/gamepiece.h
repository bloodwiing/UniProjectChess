#ifndef CHESS_GAMEPIECE_H
#define CHESS_GAMEPIECE_H

#include <stdio.h>

#include "model/piece.h"
#include "model/scenario.h"

struct Tile;

typedef struct GamePiece {
    team_index_t team;
    piece_index_t piece;

    count16_t moves;

    struct Tile * position;
} GamePiece;

GamePiece * createGamePiece(Piece * piece, piece_index_t piece_index);
Piece * getOriginalPiece(GamePiece * game_piece, Scenario * scenario);

void saveGamePiece(GamePiece * game_piece, FILE * stream);
GamePiece * loadGamePiece(FILE * stream);

GamePiece * cloneGamePiece(GamePiece * game_piece);
void freeGamePiece(GamePiece * game_piece);

#endif //CHESS_GAMEPIECE_H
