#ifndef CHESS_GAMEPIECE_H
#define CHESS_GAMEPIECE_H

#include "../model/piece.h"
#include "../model/scenario.h"
#include <stdio.h>

typedef struct GamePiece {
    uint8_t team;
    uint8_t piece;

    uint16_t moves;

    bool_t vulnerable;
    uint8_t vul_x, vul_y;
} GamePiece;

GamePiece * createGamePiece(Piece * piece, uint8_t piece_index);
Piece * getOriginalPiece(GamePiece * game_piece, Scenario * scenario);
void saveGamePiece(GamePiece * game_piece, FILE * stream);
GamePiece * loadGamePiece(FILE * stream);
void markGamePieceVulnerable(Scenario * scenario, GamePiece * game_piece, SpecialMove * special_move, uint8_t x, uint8_t y);

#endif //CHESS_GAMEPIECE_H
