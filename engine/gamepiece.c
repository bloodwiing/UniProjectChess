#include "gamepiece.h"
#include "../model/vector.h"
#include <stdlib.h>

GamePiece * createGamePiece(Piece * piece, uint8_t piece_index) {
    GamePiece * out = calloc(1, sizeof(GamePiece));
    out->team = piece->team;
    out->piece = piece_index;
    out->vulnerable = false;
    return out;
}

Piece * getOriginalPiece(GamePiece * game_piece, Scenario * scenario) {
    Team * team = scenario->teams + game_piece->team;
    return team->pieces + game_piece->piece;
}

void saveGamePiece(GamePiece * game_piece, FILE * stream) {
    fwrite(game_piece, sizeof(GamePiece), 1, stream);
}

GamePiece * loadGamePiece(FILE * stream) {
    GamePiece * out = malloc(sizeof(GamePiece));
    fread(out, sizeof(GamePiece), 1, stream);
    return out;
}

void markGamePieceVulnerable(Scenario * scenario, GamePiece * game_piece, SpecialMove * special_move, uint8_t x, uint8_t y) {
    game_piece->vulnerable = true;
    Vector vul_pos = normaliseCoordinates(x, y, (scenario->teams + game_piece->team)->direction);
    game_piece->vul_x = (int16_t)x + vul_pos.x;
    game_piece->vul_x = (int16_t)y + vul_pos.y;
}

void freeGamePiece(GamePiece * game_piece) {
    free(game_piece);
}
