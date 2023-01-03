#ifndef CHESS_GAMEPIECE_H
#define CHESS_GAMEPIECE_H

#include "../model/piece.h"
#include "../model/scenario.h"
#include <stdio.h>

struct Tile;

typedef struct GamePiece {
    team_index_t team;
    piece_index_t piece;

    count16_t moves;

    bool_t vulnerable;
    ucoord_t vul_x, vul_y;

    struct Tile * position;
} GamePiece;

GamePiece * createGamePiece(Piece * piece, piece_index_t piece_index);
Piece * getOriginalPiece(GamePiece * game_piece, Scenario * scenario);

void saveGamePiece(GamePiece * game_piece, FILE * stream);
GamePiece * loadGamePiece(FILE * stream);

void markGamePieceVulnerable(Scenario * scenario, GamePiece * game_piece, SpecialMove * special_move, ucoord_t x, ucoord_t y);

void freeGamePiece(GamePiece * game_piece);

#endif //CHESS_GAMEPIECE_H
