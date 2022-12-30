#include "tile.h"
#include <stdlib.h>
#include "validation.h"

Tile * createTile(GamePiece * game_piece) {
    Tile * out = calloc(1, sizeof(Tile));
    out->game_piece = game_piece;
    return out;
}

void saveTile(Tile * tile, FILE * stream) {
    uint8_t filled = tile->game_piece != NULL;
    fwrite(&filled, sizeof(uint8_t), 1, stream);
    if (filled)
        saveGamePiece(tile->game_piece, stream);
}

Tile * loadTile(FILE * stream) {
    Tile * out = malloc(sizeof(Tile));
    uint8_t filled = 0;
    fread(&filled, sizeof(uint8_t), 1, stream);
    if (filled)
        out->game_piece = loadGamePiece(stream);
    else
        out->game_piece = NULL;
    return out;
}

bool_t compPath(Path * path, GamePiece * piece, Vector vector, bool_t repeat) {
    return path->piece == piece && path->repeat == repeat && compVectors(path->vector, vector);
}

Path * addPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat) {
    tile->paths = realloc(tile->paths, sizeof(Path) * ++tile->path_count);
    tile->paths[tile->path_count - 1] = createPath(piece, vector, repeat);
    return tile->paths[tile->path_count - 1];
}

Path * findPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat) {
    for (int i = 0; i < tile->path_count;) {
        Path * path = tile->paths[i++];
        if (compPath(path, piece, vector, repeat))
            return path;
    }
    return NULL;
}

bool_t hasPath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat) {
    return findPath(tile, piece, vector, repeat) != NULL;
}

void removePath(Tile * tile, GamePiece * piece, Vector vector, bool_t repeat) {
    for (int i = 0; i < tile->path_count; i++) {
        Path * path = tile->paths[i];
        if (compPath(path, piece, vector, repeat)) {
            tile->paths[i] = tile->paths[--tile->path_count];
            tile->paths = realloc(tile->paths, sizeof(Path) * tile->path_count);
            break;
        }
    }
}

Path * addOrigin(Tile * tile, Vector vector, bool_t repeat) {
    tile->origins = realloc(tile->origins, sizeof(Path) * ++tile->origin_count);
    tile->origins[tile->origin_count - 1] = createPath(tile->game_piece, vector, repeat);
    return tile->origins[tile->origin_count - 1];
}

Path * findOrigin(Tile * tile, Vector vector, bool_t repeat) {
    for (int i = 0; i < tile->origin_count;) {
        Path * path = tile->origins[i++];
        if (compPath(path, tile->game_piece, vector, repeat))
            return path;
    }
    return NULL;
}

bool_t hasOrigin(Tile * tile, Vector vector, bool_t repeat) {
    return findOrigin(tile, vector, repeat) != NULL;
}

void clearOrigins(Tile * tile) {
    for (; tile->origin_count > 0;) {
        Path * path = tile->origins[--tile->origin_count];
        deletePath(tile, path);
    }
    free(tile->origins);
    tile->origins = NULL;
}
