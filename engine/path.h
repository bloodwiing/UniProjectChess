#ifndef CHESS_PATH_H
#define CHESS_PATH_H

#include "gamepiece.h"
#include "../model/vector.h"

struct Tile;

typedef struct Path {
    GamePiece * piece;
    Vector vector;
    bool_t repeat;
    struct Tile * next_tile;
    struct Path * next_path;
} Path;

Path * createPath(GamePiece * game_piece, Vector vector, bool_t repeat);

#endif //CHESS_PATH_H
