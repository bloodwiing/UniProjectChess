#include "gamepiece.h"

#include <stdlib.h>
#include <string.h>

#include "model/vector.h"

#define STRUCT_GAMEPIECE_SIZE_WITHOUT_POINTERS offsetof(GamePiece, vul_y) + sizeof(ucoord_t)

GamePiece * createGamePiece(Piece * piece, piece_index_t piece_index) {
    GamePiece * out = calloc(1, sizeof(GamePiece));
    out->team = piece->team;
    out->piece = piece_index;
    out->vulnerable = false;
    out->position = NULL;
    return out;
}

Piece * getOriginalPiece(GamePiece * game_piece, Scenario * scenario) {
    if (game_piece == NULL)
        return NULL;
    Team * team = scenario->teams + game_piece->team;
    return team->pieces + game_piece->piece;
}

void saveGamePiece(GamePiece * game_piece, FILE * stream) {
    fwrite(game_piece, STRUCT_GAMEPIECE_SIZE_WITHOUT_POINTERS, 1, stream);
}

GamePiece * loadGamePiece(FILE * stream) {
    GamePiece * out = malloc(sizeof(GamePiece));
    fread(out, STRUCT_GAMEPIECE_SIZE_WITHOUT_POINTERS, 1, stream);
    return out;
}

void markGamePieceVulnerable(Scenario * scenario, GamePiece * game_piece, SpecialMove * special_move, ucoord_t x, ucoord_t y) {
    // To-Do
}

GamePiece * cloneGamePiece(GamePiece * game_piece) {
    if (game_piece == NULL)
        return NULL;
    GamePiece * out = calloc(1, sizeof(GamePiece));
    memcpy(out, game_piece, sizeof(GamePiece));
    return out;
}

void freeGamePiece(GamePiece * game_piece) {
    free(game_piece);
}
