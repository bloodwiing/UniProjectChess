#ifndef CHESS_TILE_H
#define CHESS_TILE_H

#include "gamepiece.h"
#include <stdio.h>

typedef struct Tile {
    GamePiece * game_piece;
} Tile;

Tile * createTile(GamePiece * game_piece);
void saveTile(Tile * tile, FILE * stream);
Tile * loadTile(FILE * stream);

#endif //CHESS_TILE_H
