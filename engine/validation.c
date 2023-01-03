#include "validation.h"

void extendPath(Board * board, Path * start, ucoord_t x, ucoord_t y) {
    if (!hasPath(getTile(board, x, y), start->piece, start->vector, start->repeat, start->type))
        return;

    while (start->next_path != NULL) {
        x += start->vector.x;
        y += start->vector.y;
        start = start->next_path;
    }

    while (validateInBounds(board, (x += start->vector.x), (y += start->vector.y))) {
        Tile * target = getTile(board, x, y);

        Path * new_path = addPath(target, start->piece, start->vector, start->repeat, start->type);
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
        removePath(previous_tile, path->piece, path->vector, path->repeat, path->type);
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

void fillPathsFromPoint(Board * board, Tile * origin, Vector vector, bool_t repeat, path_type_t type, ucoord_t i, ucoord_t j) {
    Path * prev_path = addOrigin(origin, vector, repeat, type);

    while (validateInBounds(board, i += vector.x, j += vector.y)) {
        Tile * target = getTile(board, i, j);

        Path * new_path = addPath(target, origin->game_piece, vector, repeat, type);
        prev_path->next_tile = target;
        prev_path->next_path = new_path;
        prev_path = new_path;

        if (!repeat || target->game_piece != NULL)
            break;
    }
}

void expandPathTypes(Path * start, path_type_t type) {
    while (start != NULL) {
        start->type |= type;
        start = start->next_path;
    }
}

void createSinglePathing(Board * board, ucoord_t x, ucoord_t y) {
    Tile * origin = getTile(board, x, y);
    Piece * piece = getTilePiece(board, origin);

    if (piece == NULL)
        return;

    Team * team;
    if ((team = getTeam(board, piece->team)) == NULL)
        return;
    TeamDirection direction = team->direction;

    for (move_index_t move_index = 0; move_index < piece->move_set->move_count;) {
        Move move = piece->move_set->moves[move_index++];
        Vector relative = normaliseVector(move.vector, direction);

        fillPathsFromPoint(board, origin, relative, move.repeat, PATH_TYPE_MOVE, x, y);
    }

    for (move_index_t move_index = 0; move_index < piece->move_set->attack_count;) {
        Move move = piece->move_set->attacks[move_index++];
        Vector relative = normaliseVector(move.vector, direction);

        Path * origin_path;
        if ((origin_path = findOrigin(origin, relative, move.repeat, PATH_TYPE_ANY)) != NULL) {
            if ((origin_path->type & PATH_TYPE_ATTACK) == 0)
                expandPathTypes(origin_path, PATH_TYPE_ATTACK);
            continue;
        }

        fillPathsFromPoint(board, origin, relative, move.repeat, PATH_TYPE_ATTACK, x, y);
    }
}

void createBoardPathing(Board * board) {
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width;) {
            createSinglePathing(board, x++, y);
        }
    }
}

bool_t validateInBounds(Board * board, ucoord_t x, ucoord_t y) {
    return x < board->width && x >= 0 && y < board->height && y >= 0;
}

bool_t validatePath(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y) {
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

        for (move_index_t move_index = 0; move_index < move_set->attack_count;) {
            Move move = move_set->attacks[move_index++];
            Vector relative = normaliseVector(move.vector, direction);

            if (hasPath(target, piece, relative, move.repeat, PATH_TYPE_ATTACK))
                return true;
        }
        return false;
    }
    else {
        for (move_index_t move_index = 0; move_index < move_set->move_count;) {
            Move move = move_set->moves[move_index++];
            Vector relative = normaliseVector(move.vector, direction);

            if (hasPath(target, piece, relative, move.repeat, PATH_TYPE_MOVE))
                return true;
        }
        return false;
    }
}

void updateTilePaths(Board * board, ucoord_t x, ucoord_t y) {
    Tile * tile = getTile(board, x, y);

    if (tile->game_piece == NULL) {
        for (int i = 0; i < tile->path_count;) {
            Path * path = tile->paths[i++];
            if (path->repeat)
                extendPath(board, path, x, y);
        }
        clearOrigins(tile);
    } else {
        for (path_index_t i = 0; i < tile->path_count;) {
            Path * path = tile->paths[i++];
            if (path->repeat)
                trimPath(tile, path);
        }
        clearOrigins(tile);
        createSinglePathing(board, x, y);
    }
}

bool_t isMoveCheckingSelf(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y) {
    Tile * origin = getTile(board, origin_x, origin_y);

    for (path_index_t i = 0; i < origin->path_count;) {
        Path * path = origin->paths[i++];
        int x = origin_x, y = origin_y;

        while (validateInBounds(board, x += path->vector.x, y += path->vector.y) && !(x == target_x && y == target_y)) {  // keep in bounds and check if the path will be intersected by this move (if so, stop moving)
            GamePiece * obstacle;
            if ((obstacle = getBoardGamePiece(board, x, y)) != NULL) {
                if ((getOriginalPiece(obstacle, board->scenario)->protect)  // if the path now ends at a protected piece
                    && (obstacle->team != path->piece->team)  // if the protected piece is not the end of the same team's path (aka enemy is targeting)
                    && (obstacle->team == origin->game_piece->team)  // if the protected piece is of own team
                    && (path->piece != getBoardGamePiece(board, target_x, target_y)))  // if the piece is not being attacked by this move
                    return true;
                break;
            }
            if (!path->repeat)
                break;
        }
    }

    return false;
}

bool_t isTeamChecked(Board * board, Team * team) {
    GamePiece * protected = team->protected_piece;
    Tile * tile = protected->position;

    for (path_index_t i = 0; i < tile->path_count;) {
        Path * path = tile->paths[i++];
        if (path->piece->team != protected->team)
            return true;
    }

    return false;
}

bool_t isTeamCheckedAfterMove(Board * board, Team * team, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y) {
    GamePiece * protected = team->protected_piece;
    Tile * tile = protected->position;

    Tile * target_tile = getTile(board, target_x, target_y);
    if (getTile(board, origin_x, origin_y) == tile) {  // if the move is of a protected piece
        for (path_index_t i = 0; i < target_tile->path_count;) {
            Path * path = target_tile->paths[i++];
            if (path->piece->team != protected->team)  // then only check if the target location is not safe
                return true;
        }
        return false;
    }

    for (path_index_t i = 0; i < tile->path_count;) {
        Path * path = tile->paths[i++];
        if (path->piece->team != protected->team) {
            Tile * path_start = path->piece->position;
            ucoord_t x = path_start->x, y = path_start->y;

            if (path->piece == getBoardGamePiece(board, target_x, target_y))
                continue;

            while (validateInBounds(board, x += path->vector.x, y += path->vector.y) && !(x == target_x && y == target_y)) {  // keep in bounds and check if the path will be intersected by this move (if so, stop moving)
                GamePiece * obstacle;
                if ((obstacle = getBoardGamePiece(board, x, y)) != NULL) {
                    if (obstacle == protected)
                        return true;
                    break;
                }
                if (!path->repeat)
                    break;
            }
        }
    }

    return false;
}

bool_t isMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y) {
    Team * move_team = getGamePieceTeam(board, getBoardGamePiece(board, origin_x, origin_y));
    return validatePath(board, origin_x, origin_y, target_x, target_y)  // check if the piece is even able to move there
           && !isMoveCheckingSelf(board, origin_x, origin_y, target_x, target_y)
           && (!isTeamChecked(board, move_team)  // either the team needs to not be in check
               || !isTeamCheckedAfterMove(board, move_team, origin_x, origin_y, target_x, target_y));  // or the active check needs to be resolved / not created
}
