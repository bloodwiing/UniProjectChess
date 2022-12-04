#include "validation.h"
#include "../model/vector.h"

bool_t validatePieceAnyMove(Board * board, int x, int y, int i, int j) {
    if (board->tiles[i + board->width * j]->game_piece != NULL)
        return validatePieceAttack(board, x, y, i, j);
    else
        return validatePieceMove(board, x, y, i, j);
}

bool_t validatePieceMove(Board * board, int x, int y, int i, int j) {
    Piece * piece = getOriginalPiece(board->tiles[x + board->width * y]->game_piece, board->scenario);

    if (board->tiles[i + board->width * j]->game_piece != NULL)
        return false;

    for (int m = 0; m < piece->move_set->move_count; ++m) {
        Move * move = piece->move_set->moves + m;
        Vector * abs_move = normaliseCoordinates(move->x, move->y, (board->teams + piece->team)->direction);
        int offset_x = x + abs_move->x,
            offset_y = y + abs_move->y;

        int repeat = 1;
        while (validateInBounds(board, offset_x, offset_y)) {
            if (offset_x == i && offset_y == j)
                return true;

            if (board->tiles[offset_x + board->width * offset_y]->game_piece != NULL)
                break;

            if (!move->repeat)
                break;

            offset_x = x + abs_move->x * ++repeat;
            offset_y = y + abs_move->y * repeat;
        }

        free(abs_move);
    }

    return false;
}

bool_t validatePieceAttack(Board * board, int x, int y, int i, int j) {
    Piece * piece = getOriginalPiece(board->tiles[x + board->width * y]->game_piece, board->scenario);
    Piece * target;

    if (board->tiles[i + board->width * j]->game_piece == NULL)
        return false;

    target = getOriginalPiece(board->tiles[i + board->width * j]->game_piece, board->scenario);

    if (target->team == piece->team)
        return false;

    for (int m = 0; m < piece->move_set->attack_count; ++m) {
        Move * move = piece->move_set->attacks + m;
        Vector * abs_move = normaliseCoordinates(move->x, move->y, (board->teams + piece->team)->direction);
        int offset_x = x + abs_move->x,
            offset_y = y + abs_move->y;

        int repeat = 1;
        while (validateInBounds(board, offset_x, offset_y)) {
            if (offset_x == i && offset_y == j)
                return true;

            if (board->tiles[offset_x + board->width * offset_y]->game_piece != NULL)
                break;

            if (!move->repeat)
                break;

            offset_x = x + abs_move->x * ++repeat;
            offset_y = y + abs_move->y * repeat;
        }

        free(abs_move);
    }

    return false;
}

bool_t validateInBounds(Board * board, int x, int y) {
    return x < board->width && x >= 0 && y < board->height && y >= 0;
}
