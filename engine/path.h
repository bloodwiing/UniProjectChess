#ifndef CHESS_PATH_H
#define CHESS_PATH_H

#include "gamepiece.h"
#include "../model/vector.h"

typedef uint8_t PathType_t;
#define PATH_TYPE_ANY 0
#define PATH_TYPE_MOVE 1
#define PATH_TYPE_ATTACK 2
#define PATH_TYPE_ALL 3

struct Tile;

typedef struct Path {
    GamePiece * piece;

    Vector vector;
    bool_t repeat;
    PathType_t type;

    struct Tile * next_tile;
    struct Path * next_path;
} Path;

Path * createPath(GamePiece * game_piece, Vector vector, bool_t repeat, PathType_t type);

void freePath(Path * path);

#endif //CHESS_PATH_H
