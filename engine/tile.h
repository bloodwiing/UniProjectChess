#ifndef CHESS_TILE_H
#define CHESS_TILE_H

#include <stdio.h>

#include "./gamepiece.h"
#include "./path.h"

struct Board;

typedef count16_t path_index_t;
typedef uint16_t tile_index_t;

typedef struct Tile {
    GamePiece * game_piece;

    path_index_t path_count;
    Path ** paths;

    path_index_t origin_count;
    Path ** origins;

    ucoord_t x, y;

    count_t phantom_count;
    tile_index_t * phantoms;

    count_t new_phantom_count;
    tile_index_t * new_phantoms;
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

/// Adds a new phantom piece (special move consequence)
void addPhantom(struct Board * board, Tile * tile, GamePiece * game_piece);

/// Update's every phantom piece (removes old, activates new)
void updatePhantoms(Tile * tile);

/// Causes all phantom pieces on that tile to be captured
void capturePhantoms(struct Board * board, Tile * tile);

void freeTile(Tile * tile);

#endif //CHESS_TILE_H
