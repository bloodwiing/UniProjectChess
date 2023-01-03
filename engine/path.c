#include "path.h"

Path * createPath(GamePiece * game_piece, Vector vector, bool_t repeat, path_type_t type) {
    Path * out = malloc(sizeof(Path));
    out->piece = game_piece;

    out->vector = vector;
    out->repeat = repeat;
    out->type = type;

    out->next_tile = NULL;
    out->next_path = NULL;
    return out;
}

void freePath(Path * path) {
    free(path);
}
