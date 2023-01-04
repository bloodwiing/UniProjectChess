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

    for (move_index_t move_index = 0; move_index < piece->move_set.move_count;) {
        Move move = piece->move_set.moves[move_index++];
        Vector relative = toVector(move.vector);

        fillPathsFromPoint(board, origin, relative, move.repeat, PATH_TYPE_MOVE, x, y);
    }

    for (move_index_t move_index = 0; move_index < piece->move_set.attack_count;) {
        Move move = piece->move_set.attacks[move_index++];
        Vector relative = toVector(move.vector);

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

    MoveSet move_set = getOriginalPiece(piece, board->scenario)->move_set;

    Tile * target = getTile(board, target_x, target_y);

    if (target->game_piece != NULL) {
        if (target->game_piece->team == piece->team)
            return false;

        for (move_index_t move_index = 0; move_index < move_set.attack_count;) {
            Move move = move_set.attacks[move_index++];
            Vector relative = toVector(move.vector);

            if (hasPath(target, piece, relative, move.repeat, PATH_TYPE_ATTACK))
                return true;
        }
        return false;
    }
    else if (target->phantom_count) {
        for (move_index_t move_index = 0; move_index < move_set.attack_count;) {
            Move move = move_set.attacks[move_index++];
            Vector relative = toVector(move.vector);

            if (hasPath(target, piece, relative, move.repeat, PATH_TYPE_ATTACK))
                return true;
        }
        return false;
    }
    else {
        for (move_index_t move_index = 0; move_index < move_set.move_count;) {
            Move move = move_set.moves[move_index++];
            Vector relative = toVector(move.vector);

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
    }
    else {
        for (path_index_t i = 0; i < tile->path_count;) {
            Path * path = tile->paths[i++];
            if (path->repeat)
                trimPath(tile, path);
        }
        clearOrigins(tile);
        createSinglePathing(board, x, y);
    }
}

bool_t isTileDangerous(Tile * tile, team_index_t team) {
    for (path_index_t i = 0; i < tile->path_count;) {
        Path * path = tile->paths[i++];
        if (path->piece->team != team)
            return true;
    }
    return false;
}

bool_t isMoveCheckingSelf(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y) {
    GamePiece * origin_gp = getTile(board, origin_x, origin_y)->game_piece;
    Piece * origin = getOriginalPiece(origin_gp, board->scenario);
    if (origin == NULL)
        return false;
    Team * team = getPieceTeam(board, origin);

    bool_t protected_moved = false;  // if the origin is a protected tile, then read the protected tile as a target
    Tile * protected_tile = team->protected_piece->position;
    if (origin_gp == team->protected_piece) {
        protected_tile = getTile(board, target_x, target_y);
        protected_moved = true;
    }

    for (path_index_t i = 0; i < protected_tile->path_count;) {  // loop over everything that is attacking the protected piece
        Path * path = protected_tile->paths[i++];
        if (path->piece->team == origin->team)  // if path of same team <=> not an enemy and continue
            continue;

        if (path->piece == getBoardGamePiece(board, target_x, target_y) && (path->type & PATH_TYPE_ATTACK))  // if path is being attacked by this move
            continue;

        Tile * tile_start = path->piece->position;  // the starting point of the enemy path

        int x = tile_start->x, y = tile_start->y;

        while (validateInBounds(board, x += path->vector.x, y += path->vector.y) && (!(x == target_x && y == target_y) || protected_moved)) {  // keep in bounds and check if the path will be intersected by this move (if so, stop moving, unless intersection is with a protected piece)
            if (getTile(board, x, y) == protected_tile)
                return true;
            GamePiece * obstacle;
            if ((obstacle = getBoardGamePiece(board, x, y)) != NULL) {
                if (x == origin_x && y == origin_y)  // if the piece moved from this location, continue the path
                    continue;
                if ((getOriginalPiece(obstacle, board->scenario) && !protected_moved)  // if the path now ends at the protected tile (that wasn't moved)
                    && (obstacle->team != origin->team))  // if the obstacle is an enemy type
                    return true;
                break;
            }
            if (!path->repeat)
                break;
        }
    }

    return false;
}

bool_t isTeamChecked(Team * team) {
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
    if (getTile(board, origin_x, origin_y) == tile)  // if the move is of a protected piece
        return isTileDangerous(target_tile, protected->team);  // then only check if the target location is not safe

    for (path_index_t i = 0; i < tile->path_count;) {
        Path * path = tile->paths[i++];
        if (path->piece->team != protected->team) {
            Tile * path_start = path->piece->position;
            ucoord_t x = path_start->x, y = path_start->y;

            if (path->piece == target_tile->game_piece && (path->type & PATH_TYPE_ATTACK))  // if the move is going to capture this piece
                continue;  // then it is safe

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

bool_t isMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, ucoord_t target_x, ucoord_t target_y, bool_t validate_path) {
    Team * move_team = getGamePieceTeam(board, getBoardGamePiece(board, origin_x, origin_y));
    return (
        (
            (!validate_path || validatePath(board, origin_x, origin_y, target_x, target_y))  // check if the piece is even able to move there
            && !isMoveCheckingSelf(board, origin_x, origin_y, target_x, target_y)  // AND move must not cause a check
        ) && (  // AND
            !isTeamChecked(move_team)  // either the team needs to not be in check
            || !isTeamCheckedAfterMove(board, move_team, origin_x, origin_y, target_x, target_y)  // OR the active check needs to be resolved / not created
        )
   );
}

bool_t checkSpecialDataRequirements(Board * board, ucoord_t origin_x, ucoord_t origin_y, SpecialData * special_data) {
    GamePiece * game_piece = getBoardGamePiece(board, origin_x, origin_y);
    if (game_piece == NULL)
        return false;
    Piece * piece = getOriginalPiece(game_piece, board->scenario);

    if (special_data->is_first_move && game_piece->moves > 0)  // first move check
        return false;

    Vector normalised = toVector(special_data->vector);
    if (!validateInBounds(board, origin_x + normalised.x, origin_y + normalised.y))  // the special move must not go out of bounds
        return false;

    if (getTile(board, origin_x + normalised.x, origin_y + normalised.y)->game_piece != NULL)  // the end needs to be not occupied
        return false;

    for (count_t i = 0; i < special_data->condition_count;) {
        Vector condition = toVector(special_data->conditions[i++]);
        Tile * tile = getTile(board, origin_x + condition.x, origin_y + condition.y);
        if (tile->game_piece != NULL)  // the passing tile must be emtpy
            return false;
        if (piece->protected && !special_data->is_check_safe && isTileDangerous(tile, piece->team))  // if the passing tile would cause a check, it fails
            return false;
    }

    if (isTeamChecked(getTeam(board, piece->team)) && !special_data->is_check_safe)  // move cannot be run while in check and being non-check-safe
        return false;

    return true;  // requirements passed (does not mean that the move is fully valid, just that it can be evaluated)
}

bool_t isSpecialMoveValid(Board * board, ucoord_t origin_x, ucoord_t origin_y, SpecialMove * special_move) {
    GamePiece * game_piece = getBoardGamePiece(board, origin_x, origin_y);
    if (game_piece == NULL)
        return false;
    Piece * piece = getOriginalPiece(game_piece, board->scenario);

    if (special_move < piece->move_set.specials || special_move > piece->move_set.specials + sizeof(SpecialMove) * (piece->move_set.special_count - 1))  // special move must be from the piece itself
        return false;

    if (!checkSpecialDataRequirements(board, origin_x, origin_y, &special_move->data))  // initial test
        return false;

    for (special_extra_index_t i = 0; i < special_move->extra_count;) {
        SpecialMoveExtra extra = special_move->extra[i++];

        Vector location = toVector(extra.piece_location);

        ucoord_t from_x = origin_x + location.x,
                 from_y = origin_y + location.y;

        GamePiece * extra_piece;
        if ((extra_piece = getBoardGamePiece(board, from_x, from_y)) == NULL || extra_piece->piece != extra.piece_type)  // if the type doesn't match - fail
            return false;

        if (!checkSpecialDataRequirements(board, from_x, from_y, &extra.data))  // extra tests
            return false;
    }

    Board * prediction = cloneBoard(board);  // create a scenario for all the moved pieces

    Vector offset = toVector(special_move->data.vector);
    moveBoardGamePiece(prediction, origin_x, origin_y, origin_x + offset.x, origin_y + offset.y);
    for (special_extra_index_t i = 0; i < special_move->extra_count;) {
        SpecialMoveExtra extra = special_move->extra[i++];

        Vector location = toVector(extra.piece_location),
               vector = toVector(extra.data.vector);

        ucoord_t from_x = origin_x + location.x,
                 from_y = origin_y + location.y,
                 to_x = from_x + vector.x,
                 to_y = from_y + vector.y;

        moveBoardGamePiece(prediction, from_x, from_y, to_x, to_y);
    }

    bool_t valid = isTeamChecked(getTeam(prediction, board->active_turn));  // and check if the scenario is checked

    freeBoard(prediction, false);
    return !valid;
}
