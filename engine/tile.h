#ifndef CHESS_TILE_H
#define CHESS_TILE_H

#include <stdio.h>

#include "./gamepiece.h"
#include "./path.h"

typedef count16_t path_index_t;

typedef struct Tile {
    GamePiece * game_piece;

    path_index_t path_count;
    Path ** paths;

    path_index_t origin_count;
    Path ** origins;

    ucoord_t x, y;
} Tile;

Tile * createTile(GamePiece * game_piece, ucoord_t x, ucoord_t y);
void saveTile(Tile * tile, FILE * stream);
Tile * loadTile(FILE * stream);

bool_t compPath(Path * path, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type);

Path * addPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type);
Path * findPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type);
bool_t hasPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type);
void removePath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type);

Path * addOrigin(Tile * tile, Vector vector, bool_t repeat, path_type_t type);
Path * findOrigin(Tile * tile, Vector vector, bool_t repeat, path_type_t type);
bool_t hasOrigin(Tile * tile, Vector vector, bool_t repeat, path_type_t type);
void clearOrigins(Tile * tile);

void freeTile(Tile * tile);

#endif //CHESS_TILE_H
