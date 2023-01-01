#include "validation.h"

void extendPath(Board * board, Path * start, int x, int y) {
    if (!hasPath(getTile(board, x, y), start->piece, start->vector, start->repeat))
        return;

    while (start->next_path != NULL) {
        x += start->vector.x;
        y += start->vector.y;
        start = start->next_path;
    }

    while (validateInBounds(board, (x += start->vector.x), (y += start->vector.y))) {
        Tile * target = getTile(board, x, y);

        Path * new_path = addPath(target, start->piece, start->vector, start->repeat);
        start->next_tile = target;
        start->next_path = new_path;
        start = new_path;

        if (!start->repeat || target->game_piece != NULL)
            break;
    }
}

void deletePath(Tile * start, Path * path) {
    while (path != NULL) {
        Tile * previous_tile = start;
        start = path->next_tile;

        Path * next = path->next_path;
        path->next_tile = NULL;
        path->next_path = NULL;
        removePath(previous_tile, path->piece, path->vector, path->repeat);
        free(path);
        path = next;
    }
}

void trimPath(Tile * start, Path * path) {
    while (path->next_path != NULL) {
        if (start->game_piece != NULL) {
            deletePath(path->next_tile, path->next_path);
            path->next_path = NULL;
            path->next_tile = NULL;
            break;
        }
        start = path->next_tile;
        path = path->next_path;
    }
}

void fillPathsFromPoint(Board * board, Tile * origin, Vector vector, bool_t repeat, int i, int j) {
    Path * prev_path = addOrigin(origin, vector, repeat);

    while (validateInBounds(board, i += vector.x, j += vector.y)) {
        Tile * target = getTile(board, i, j);

        Path * new_path = addPath(target, origin->game_piece, vector, repeat);
        prev_path->next_tile = target;
        prev_path->next_path = new_path;
        prev_path = new_path;

        if (!repeat || target->game_piece != NULL)
            break;
    }
}

void createSinglePathing(Board * board, int x, int y) {
    Tile * origin = getTile(board, x, y);
    Piece * piece = getTilePiece(board, origin);

    if (piece == NULL)
        return;

    Team * team;
    if ((team = getTeam(board, piece->team)) == NULL)
        return;
    TeamDirection direction = team->direction;

    for (int move_index = 0; move_index < piece->move_set->move_count;) {
        Move move = piece->move_set->moves[move_index++];
        Vector relative = normaliseVector(move.vector, direction);

        fillPathsFromPoint(board, origin, relative, move.repeat, x, y);
    }

    for (int move_index = 0; move_index < piece->move_set->attack_count;) {
        Move move = piece->move_set->attacks[move_index++];
        Vector relative = normaliseVector(move.vector, direction);

        if (hasOrigin(origin, relative, move.repeat))
            continue;

        fillPathsFromPoint(board, origin, relative, move.repeat, x, y);
    }
}

void createBoardPathing(Board * board) {
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width;) {
            createSinglePathing(board, x++, y);
        }
    }
}

bool_t validateInBounds(Board * board, int x, int y) {
    return x < board->width && x >= 0 && y < board->height && y >= 0;
}

bool_t validatePath(Board * board, int origin_x, int origin_y, int target_x, int target_y) {
    GamePiece * piece = getBoardGamePiece(board, origin_x, origin_y);

    if (piece == NULL)
        return false;

    MoveSet * move_set = getOriginalPiece(piece, board->scenario)->move_set;

    Team * team;
    if ((team = getTeam(board, piece->team)) == NULL)
        return false;
    TeamDirection direction = team->direction;

    Tile * target = getTile(board, target_x, target_y);

    if (target->game_piece != NULL) {
        if (target->game_piece->team == piece->team)
            return false;

        for (int move_index = 0; move_index < move_set->attack_count;) {
            Move move = move_set->attacks[move_index++];
            Vector relative = normaliseVector(move.vector, direction);

            if (hasPath(target, piece, relative, move.repeat))
                return true;
        }
        return false;
    }
    else {
        for (int move_index = 0; move_index < move_set->move_count;) {
            Move move = move_set->moves[move_index++];
            Vector relative = normaliseVector(move.vector, direction);

            if (hasPath(target, piece, relative, move.repeat))
                return true;
        }
        return false;
    }
}

void updateTilePaths(Board * board, int x, int y) {
    Tile * tile = getTile(board, x, y);

    if (tile->game_piece == NULL) {
        for (int i = 0; i < tile->path_count;) {
            Path * path = tile->paths[i++];
            if (path->repeat)
                extendPath(board, path, x, y);
        }
        clearOrigins(tile);
    } else {
        for (int i = 0; i < tile->path_count;) {
            Path * path = tile->paths[i++];
            if (path->repeat)
                trimPath(tile, path);
        }
        clearOrigins(tile);
        createSinglePathing(board, x, y);
    }
}
