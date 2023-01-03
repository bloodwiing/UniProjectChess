#include "tile.h"

#include <stdlib.h>

#include "./validation.h"

Tile * createTile(GamePiece * game_piece, ucoord_t x, ucoord_t y) {
    Tile * out = calloc(1, sizeof(Tile));
    out->game_piece = game_piece;
    if (game_piece != NULL)
        game_piece->position = out;
    out->x = x;
    out->y = y;
    return out;
}

void saveTile(Tile * tile, FILE * stream) {
    bool_t filled = tile->game_piece != NULL;
    fwrite(&filled, sizeof(bool_t), 1, stream);
    if (filled)
        saveGamePiece(tile->game_piece, stream);
}

Tile * loadTile(FILE * stream) {
    Tile * out = malloc(sizeof(Tile));
    bool_t filled = 0;
    fread(&filled, sizeof(bool_t), 1, stream);
    out->game_piece = filled ? loadGamePiece(stream) : NULL;
    return out;
}

bool_t compPath(Path * path, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type) {
    return path->piece == piece && path->repeat == repeat && compVectors(path->vector, vector) && (path->type & type) == type;
}

Path * addPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type) {
    tile->paths = realloc(tile->paths, sizeof(Path) * ++tile->path_count);
    tile->paths[tile->path_count - 1] = createPath(piece, vector, repeat, type);
    return tile->paths[tile->path_count - 1];
}

Path * findPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type) {
    for (path_index_t i = 0; i < tile->path_count;) {
        Path * path = tile->paths[i++];
        if (compPath(path, piece, vector, repeat, type))
            return path;
    }
    return NULL;
}

bool_t hasPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type) {
    return findPath(tile, piece, vector, repeat, type) != NULL;
}

void removePath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat, path_type_t type) {
    for (path_index_t i = 0; i < tile->path_count; i++) {
        Path * path = tile->paths[i];
        if (compPath(path, piece, vector, repeat, type)) {
            tile->paths[i] = tile->paths[--tile->path_count];
            tile->paths = realloc(tile->paths, sizeof(Path) * tile->path_count);
            return;
        }
    }
}

Path * addOrigin(Tile * tile, Vector vector, bool_t repeat, path_type_t type) {
    tile->origins = realloc(tile->origins, sizeof(Path) * ++tile->origin_count);
    tile->origins[tile->origin_count - 1] = createPath(tile->game_piece, vector, repeat, type);
    return tile->origins[tile->origin_count - 1];
}

Path * findOrigin(Tile * tile, Vector vector, bool_t repeat, path_type_t type) {
    for (path_index_t i = 0; i < tile->origin_count;) {
        Path * path = tile->origins[i++];
        if (compPath(path, tile->game_piece, vector, repeat, type))
            return path;
    }
    return NULL;
}

bool_t hasOrigin(Tile * tile, Vector vector, bool_t repeat, path_type_t type) {
    return findOrigin(tile, vector, repeat, type) != NULL;
}

void clearOrigins(Tile * tile) {
    for (; tile->origin_count > 0;) {
        Path * path = tile->origins[--tile->origin_count];
        deletePath(tile, path);
    }
    free(tile->origins);
    tile->origins = NULL;
}

void freeTile(Tile * tile) {
    freeGamePiece(tile->game_piece);
    for (path_index_t i = 0; i < tile->path_count;)
        freePath(tile->paths[i++]);
    free(tile->paths);
    for (path_index_t i = 0; i < tile->origin_count;)
        freePath(tile->origins[i++]);
    free(tile->origins);
    free(tile);
}
