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

    uint8_t x, y;
} Tile;

Tile * createTile(GamePiece * game_piece, uint8_t x, uint8_t y);
void saveTile(Tile * tile, FILE * stream);
Tile * loadTile(FILE * stream);

bool_t compPath(Path * path, GamePiece * piece, Vector vector, bool_t repeat, PathType_t type);

Path * addPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, PathType_t type);
Path * findPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, PathType_t type);
bool_t hasPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, PathType_t type);
void removePath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, PathType_t type);

Path * addOrigin(Tile * tile, Vector vector, bool_t repeat, PathType_t type);
Path * findOrigin(Tile * tile, Vector vector, bool_t repeat, PathType_t type);
bool_t hasOrigin(Tile * tile, Vector vector, bool_t repeat, PathType_t type);
void clearOrigins(Tile * tile);

#endif //CHESS_TILE_H
