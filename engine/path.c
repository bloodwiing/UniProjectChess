#include "path.h"

Path * createPath(GamePiece * game_piece, Vector vector, bool_t repeat) {
    Path * out = malloc(sizeof(Path));
    out->piece = game_piece;
    out->vector = vector;
    out->repeat = repeat;
    out->next_tile = NULL;
    out->next_path = NULL;
    return out;
}
