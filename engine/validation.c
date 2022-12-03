#include "validation.h"

bool_t validatePieceAnyMove(Board * board, int x, int y, int i, int j) {
    Piece * piece = board->tiles[x + board->width * y];

    if (board->tiles[i + board->width * j] != NULL)
        return validatePieceAttack(board, x, y, i, j);
    else
        return validatePieceMove(board, x, y, i, j);
}

bool_t validatePieceMove(Board * board, int x, int y, int i, int j) {
    Piece * piece = board->tiles[x + board->width * y];

    if (board->tiles[i + board->width * j] != NULL)
        return false;

    for (int m = 0; m < piece->move_set->move_count; ++m) {
        Move * move = piece->move_set->moves + m;
        int offset_x = x + move->x,
            offset_y = y + move->y;

        while (validateInBounds(board, offset_x, offset_y)) {
            if (offset_x == i && offset_y == j)
                return true;

            if (board->tiles[offset_x + board->width * offset_y] != NULL)
                break;

            if (!move->repeat)
                break;
        }
    }

    return false;
}

bool_t validatePieceAttack(Board * board, int x, int y, int i, int j) {
    Piece * piece = board->tiles[x + board->width * y];
    Piece * target;

    if ((target = board->tiles[i + board->width * j]) == NULL)
        return false;

    if (target->team == piece->team)
        return false;

    for (int m = 0; m < piece->move_set->attack_count; ++m) {
        Move * move = piece->move_set->attacks + m;
        int offset_x = x + move->x,
            offset_y = y + move->y;

        while (validateInBounds(board, offset_x, offset_y)) {
            if (offset_x == i && offset_y == j)
                return true;

            if (board->tiles[offset_x + board->width * offset_y] != NULL)
                break;

            if (!move->repeat)
                break;
        }
    }

    return false;
}

bool_t validateInBounds(Board * board, int x, int y) {
    return x < board->width && x >= 0 && y < board->height && y >= 0;
}
