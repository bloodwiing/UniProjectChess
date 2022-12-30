#ifndef CHESS_TILE_H
#define CHESS_TILE_H

#include "gamepiece.h"
#include "path.h"
#include <stdio.h>

typedef struct Tile {
    GamePiece * game_piece;

    uint16_t path_count;
    Path ** paths;

    uint16_t origin_count;
    Path ** origins;
} Tile;

Tile * createTile(GamePiece * game_piece);
void saveTile(Tile * tile, FILE * stream);
Tile * loadTile(FILE * stream);

bool_t compPath(Path * path, GamePiece * piece, Vector vector, bool_t repeat);

Path * addPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat);
Path * findPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat);
bool_t hasPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat);
void removePath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat);

Path * addOrigin(Tile * tile, Vector vector, bool_t repeat);
Path * findOrigin(Tile * tile, Vector vector, bool_t repeat);
bool_t hasOrigin(Tile * tile, Vector vector, bool_t repeat);
void clearOrigins(Tile * tile);

#endif //CHESS_TILE_H
