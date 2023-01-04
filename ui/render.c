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

void renderPieceWithBackground(UserSettings * settings, Team * team, Piece * piece, int bg) {
    if (settings->display.unicode)
        renderTextColoured(settings, bg, team->colour, L"%ls", piece->unicode);
    else
        renderTextColoured(settings, bg, team->colour, L"%c", piece->symbol);
}

void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg) {
    Piece * piece = getOriginalPiece(game_piece, scenario);
    Team * team = scenario->teams + piece->team;
    renderPieceWithBackground(settings, team, piece, bg);
}

void renderPiece(UserSettings * settings, Team * team, Piece * piece) {
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

void renderBoard(Board * board, int pos_x, int pos_y, int i, int j, int w, int h) {
    int board_width = board->width * 2 + 1,
            board_height = board->height;

    int width = w > board_width + 2 ? board_width + 2 : w,
            height = h > board_height + 2 ? board_height + 2 : h;

    if (i > board_width + 2 - width) i = board_width + 2 - width;
    if (j > board_height + 2 - height) j = board_height + 2 - height;
    int reset_i = i - 1;
    int reset_j = j - 1;

    pos_x += 1 - i;
    pos_y += 1 - j;

    width += i - 2;
    height += j - 2;

    for (--j; j <= height; ++j) {
        for (i = reset_i; i <= width; ++i) {
            con_set_pos(pos_x + i, pos_y + j);
            char edge;

            if ((edge = getEdgeChar(i, j, board_width, board_height)) != ' ')
                renderTextColoured(board->user_settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%c", edge);

            else if (i % 2 == 1) {
                int tile = i / 2 + j * board->width;

                GamePiece * game_piece;
                if ((game_piece = board->tiles[tile]->game_piece) != NULL)
                    renderGamePieceWithBackground(board->user_settings, board->scenario, game_piece, COLOR_RESET);
                else
                    wprintf(L" ");
            } else
                wprintf(L" ");
        }

        wprintf(L"%*s", w - (width - reset_i) - 1, "");
    }

    for (int end = 0; end <= h - height - reset_j + 1;) {
        con_set_pos(pos_x - 1, pos_y + j + end++);
        wprintf(L"%*s", w, "");
    }
}

void renderBoardWithSelection(Board * board, int pos_x, int pos_y, int i, int j, int w, int h, int sel_x, int sel_y) {
    renderBoard(board, pos_x, pos_y, i, j, w, h);

    uint8_t top_left_x = pos_x - i + 2,
            top_left_y = pos_y - j + 1;

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

                con_set_pos(top_left_x + target_x * 2, top_left_y + target_y);

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

            con_set_pos(top_left_x + target_x * 2, top_left_y + target_y);

            Tile * target = getTile(board, target_x, target_y);
            GamePiece * occupant;

            if (((occupant = target->game_piece) != NULL) && (occupant->team != selected->game_piece->team))
                renderGamePieceWithBackground(board->user_settings, board->scenario, occupant, COLOR_GREEN);
            else if (occupant == NULL)
                renderTextColoured(board->user_settings, COLOR_GREEN, COLOR_LIGHT_GRAY, L" ");
        }
    }
}

void renderScenario(Scenario * scenario, UserSettings * settings, int pos_x, int pos_y, int i, int j, int w, int h) {
    Exception exception = {};
    Board * board = createBoard(scenario, settings, &exception);
    if (board == NULL && exception.status) {
        clearRect(pos_x, pos_y, w, h);
        reportExceptionAtPos(exception, pos_x, pos_y);
        return;
    }
    renderBoard(board, pos_x, pos_y, i, j, w, h);
    freeBoard(board, false);
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
