#include "render.h"

#include "./con_lib.h"

#include <stdio.h>

#include "engine/validation.h"

void renderText(wchar_t * format, ...) {
    va_list argv;
    va_start(argv, format);
    vfwprintf(stdout, format, argv);
    va_end(argv);
}

void renderTextColoured(UserSettings * settings, int bg, int fg, wchar_t * format, ...) {
    if (settings->display.colourful)
        con_set_color(bg, fg);

    va_list argv;
    va_start(argv, format);
    vfwprintf(stdout, format, argv);
    va_end(argv);

    if (settings->display.colourful)
        con_set_color(COLOR_RESET, COLOR_RESET);
}

void renderPieceColoured(UserSettings * settings, int bg, int fg, Piece piece) {
    if (settings->display.unicode)
        renderTextColoured(settings, bg, fg, L"%ls", piece.unicode);
    else
        renderTextColoured(settings, bg, fg, L"%c", piece.symbol);
}

void renderPieceWithBackground(UserSettings * settings, Team team, Piece piece, int bg) {
    renderPieceColoured(settings, bg, team.colour, piece);
}

void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg) {
    Piece * piece = getOriginalPiece(game_piece, scenario);
    renderPieceWithBackground(settings, scenario->teams[piece->team], *piece, bg);
}

void renderPiece(UserSettings * settings, Team team, Piece piece) {
    renderPieceWithBackground(settings, team, piece, COLOR_RESET);
}

char getEdgeChar(int i, int j, int board_width, int board_height) {
    char edge = ' ';

    if (i < 0 || i >= board_width)
        edge = '|';
    if (j < 0 || j >= board_height) {
        if (edge != ' ')
            edge = '+';
        else
            edge = '-';
    }

    return edge;
}

void renderBoard(Board * board, Rect draw_rect, Rect board_rect) {
    Rect original_rect = getBoardRect(board, 0, 0);
    original_rect.width -= 2;
    original_rect.height -= 2;

    board_rect.x *= 2;

    int i = board_rect.x,
        j = board_rect.y;

    int reset_i = i - 1,
        reset_j = j - 1;

    int pos_x = draw_rect.x - i + 1,
        pos_y = draw_rect.y + draw_rect.height - 2 - j;

    int width = board_rect.width + i - 2,
        height = board_rect.height + j - 2;

    for (--j; j <= height; j++) {
        for (i = reset_i; i <= width; i++) {
            con_set_pos(pos_x + i, pos_y - j + board_rect.y * 2);
            char edge;

            if ((edge = getEdgeChar(i, j, original_rect.width, original_rect.height)) != ' ')
                renderTextColoured(board->user_settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%c", edge);

            else if (j >= 0 && i >= 0 && i % 2 == 1) {
                int tile = i / 2 + j * board->width;

                GamePiece * game_piece;
                if ((game_piece = board->tiles[tile]->game_piece) != NULL)
                    renderGamePieceWithBackground(board->user_settings, board->scenario, game_piece, COLOR_RESET);
                else
                    wprintf(L" ");
            } else
                wprintf(L" ");
        }

        wprintf(L"%*s", draw_rect.width - (width - reset_i) - 1, "");
    }

    for (int end = 0; end <= draw_rect.height - height - reset_j + 1;) {
        con_set_pos(pos_x - 1, pos_y + j + end++);
        wprintf(L"%*s", draw_rect.width, "");
    }
}

void renderBoardWithSelection(Board * board, Rect draw_rect, Rect board_rect, int sel_x, int sel_y) {
    renderBoard(board, draw_rect, board_rect);

    uint8_t bottom_left_x = draw_rect.x - board_rect.x + 2,
            bottom_left_y = draw_rect.y + board_rect.height + board_rect.y + 3;

    if (sel_x != -1 && sel_y != -1) {
        Tile * selected = getTile(board, sel_x, sel_y);

        for (int origin_index = 0; origin_index < selected->origin_count;) {

            Path * target = selected->origins[origin_index++];

            uint8_t target_x = sel_x, target_y = sel_y;

            while (target->next_path != NULL) {
                target_x += target->vector.x;
                target_y += target->vector.y;

                Tile * tile = target->next_tile;
                target = target->next_path;

                con_set_pos(bottom_left_x + target_x * 2, bottom_left_y - target_y);

                GamePiece * occupant;

                if (!isMoveValid(board, sel_x, sel_y, target_x, target_y, true))
                    continue;

                if ((occupant = tile->game_piece) != NULL  // tile has a piece
                        && (occupant->team != selected->game_piece->team)  // AND piece is an enemy
                        && (target->type & PATH_TYPE_ATTACK))  // AND the path supports attacking
                    renderGamePieceWithBackground(board->user_settings, board->scenario, occupant, COLOR_GREEN);
                else if (((occupant == NULL)  // ELSE tile is free
                            && (target->type & PATH_TYPE_MOVE))  // AND path supports moving
                        || (tile->phantom_count)  // OR tile has phantom pieces
                            && (target->type & PATH_TYPE_ATTACK))  // AND the path supports attacking
                    renderTextColoured(board->user_settings, COLOR_GREEN, COLOR_LIGHT_GRAY, L" ");
            }
        }

        Piece * piece = getOriginalPiece(selected->game_piece, board->scenario);

        for (move_index_t move_index = 0; move_index < piece->move_set.special_count;) {
            SpecialMove * special = piece->move_set.specials + move_index++;

            if (!isSpecialMoveValid(board, sel_x, sel_y, special))
                continue;

            Vector normalised = toVector(special->data.vector);
            ucoord_t target_x = sel_x + normalised.x,
                     target_y = sel_y + normalised.y;

            con_set_pos(bottom_left_x + target_x * 2, bottom_left_y - target_y);

            Tile * target = getTile(board, target_x, target_y);
            GamePiece * occupant;

            if (((occupant = target->game_piece) != NULL) && (occupant->team != selected->game_piece->team))
                renderGamePieceWithBackground(board->user_settings, board->scenario, occupant, COLOR_GREEN);
            else if (occupant == NULL)
                renderTextColoured(board->user_settings, COLOR_GREEN, COLOR_LIGHT_GRAY, L" ");
        }
    }
}

void renderScenario(Scenario * scenario, UserSettings * settings, Rect draw_rect, Rect board_rect) {
    Exception exception = {};
    Board * board = createBoard(scenario, settings, &exception);
    if (board == NULL && exception.status) {
        clearRect(draw_rect.x, draw_rect.y, draw_rect.width, draw_rect.height);
        reportExceptionAtPos(exception, draw_rect.x, draw_rect.y);
        return;
    }
    renderBoard(board, draw_rect, board_rect);
    freeBoard(board, false);
}

void setCursorAtTile(Rect draw_rect, Rect board_rect, int x, int y) {
    con_set_pos(draw_rect.x + (x - board_rect.x + 1) * 2, draw_rect.y + board_rect.height - 2 - y + board_rect.y);
}

void ditherEffect() {
    ConSize size = con_get_size();
    con_set_color(COLOR_RESET, COLOR_RESET);
    for (int y = 0; y < size.height; y++)
        for (int x = 0; x < size.width / 2; x++)
            if ((x + y) % 2) {
                con_set_pos(x * 2, y);
                putwchar(L' ');
                putwchar(L' ');
            }
    con_flush();
}

void clearRect(int x, int y, int w, int h) {
    for (int i = y; i < y + h;) {
        con_set_pos(x, i++);
        wprintf(L"%*s", w, "");
    }
}
