#include "tile.h"
#include <stdlib.h>

Tile * createTile(GamePiece * game_piece) {
    Tile * out = calloc(1, sizeof(Tile));
    out->game_piece = game_piece;
    return out;
}

void saveTile(Tile * tile, FILE * stream) {
    uint8_t filled = tile->game_piece != NULL;
    fwrite(&filled, sizeof(uint8_t), 1, stream);
    if (filled)
        saveGamePiece(tile->game_piece, stream);
}

Tile * loadTile(FILE * stream) {
    Tile * out = malloc(sizeof(Tile));
    uint8_t filled = 0;
    fread(&filled, sizeof(uint8_t), 1, stream);
    if (filled)
        out->game_piece = loadGamePiece(stream);
    else
        out->game_piece = NULL;
    return out;
}
