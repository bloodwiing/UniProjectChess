#include "render.h"

#include "./con_lib.h"
#include "./notation.h"
#include "./shape.h"

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

void renderPieceWithBackground(UserSettings * settings, Team team, Piece piece, int bg, bool_t invert) {
    if (invert)
        renderPieceColoured(settings, team.colour, bg, piece);
    else
        renderPieceColoured(settings, bg, team.colour, piece);
}

void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg, bool_t invert) {
    Piece * piece = getOriginalPiece(game_piece, scenario);
    renderPieceWithBackground(settings, scenario->teams[piece->team], *piece, bg, invert);
}

void renderPiece(UserSettings * settings, Team team, Piece piece) {
    renderPieceWithBackground(settings, team, piece, COLOR_RESET, false);
}

void renderBoard(Board * board, Rect draw_rect, Rect board_rect, bool_t with_coords) {
    Rect original_rect = getBoardRect(board, 0, 0);
    original_rect.width -= 2;
    original_rect.height -= 2;

    board_rect.x *= 2;

    int offset_x = board_rect.x < 0 ? board_rect.x : 0,
        offset_y = board_rect.y < 0 ? board_rect.y : 0;

    int i = board_rect.x - offset_x,
        j = board_rect.y - offset_y;

    int reset_i = i - 1,
        reset_j = j - 1;

    int pos_x = draw_rect.x - i + 1,
        pos_y = draw_rect.y + draw_rect.height - 2 - j;

    int width = board_rect.width + i - 2,
        height = board_rect.height + j - 2;

    for (j = reset_j; j <= height; j++) {
        // left margin clear
        con_set_pos(pos_x + reset_i, pos_y - j + board_rect.y * 2 - offset_y);
#ifdef DEBUG_BOARD_RENDERING
        con_set_color(COLOR_LIGHT_MAGENTA, COLOR_RESET);
#endif
        wprintf(L"%*s", -offset_x, "");

        for (i = reset_i; i <= width; i++) {
#ifdef DEBUG_BOARD_RENDERING
            con_set_color(COLOR_RED, COLOR_RESET);
#endif
            con_set_pos(pos_x + i - offset_x, pos_y - j + board_rect.y * 2 - offset_y);

            // board edge check + rendering
            wchar_t edge;
            box_space_t spaces = with_coords ? BOX_IGNORE_TOP | BOX_IGNORE_LEFT : 0;
            if ((edge = getBoxBorder(board->user_settings->display.unicode ? BOX_BORDER_DOUBLE : BOX_BORDER_DOUBLE_ASCII,
                                     createRect(-1, -1, original_rect.width + 1, original_rect.height + 1),
                                     i, original_rect.height - j - 1, spaces)) != L'\0')
#ifdef DEBUG_BOARD_RENDERING
                renderTextColoured(board->user_settings, COLOR_ORANGE, COLOR_LIGHT_GRAY, L"%lc", edge);
#else
                renderTextColoured(board->user_settings, COLOR_RESET, COLOR_DARK_GRAY, L"%lc", edge);
#endif

            // board bounds check
            else if (i >= 0 && j >= 0
                     && i < original_rect.width && j < original_rect.height) {
                if (i % 2 == 1) {  // on grid check
                    int tile = i / 2 + j * board->width;

                    // piece rendering
                    GamePiece *game_piece;
                    if ((game_piece = board->tiles[tile]->game_piece) != NULL)
#ifdef DEBUG_BOARD_RENDERING
                        renderGamePieceWithBackground(board->user_settings, board->scenario, game_piece, COLOR_BLUE);
#else
                        renderGamePieceWithBackground(board->user_settings, board->scenario, game_piece, COLOR_RESET, false);
#endif
                    else
                        wprintf(L" ");
                }
                else
                    wprintf(L" ");
            }
        }

        // right margin clear
#ifdef DEBUG_BOARD_RENDERING
        con_set_color(COLOR_CYAN, COLOR_RESET);
#endif
        wprintf(L"%*s", draw_rect.width - (width - reset_i) - 1 + offset_x, "");
    }

    // top margin clear
    for (int end = draw_rect.height - board_rect.height + offset_y; end > 0; end--) {
#ifdef DEBUG_BOARD_RENDERING
        con_set_color(COLOR_GREEN, COLOR_RESET);
#endif
        con_set_pos(draw_rect.x, draw_rect.y + end - 1);
        wprintf(L"%*s", draw_rect.width, "");
    }

    // bottom margin clear
    for (int end = offset_y; end < 0; end++) {
#ifdef DEBUG_BOARD_RENDERING
        con_set_color(COLOR_YELLOW, COLOR_RESET);
#endif
        con_set_pos(draw_rect.x, draw_rect.y + draw_rect.height + end);
        wprintf(L"%*s", draw_rect.width, "");
    }

    if (with_coords) {
        // files
        for (i = reset_i + 1; i < width / 2; i++) {
            char * file = getFileNotation(i);
            // above the chess board (top bar out of view)
            if (height < board->height)
                con_set_pos(pos_x + i * 2 - offset_x + 1, draw_rect.y - 1);
            else {
                // clean above the chess board if it was rendered there
                if (board_rect.height < board->height) {
                    con_set_pos(pos_x + i * 2 - offset_x + 1, draw_rect.y - 1);
                    wprintf(L" ");
                }
                // on border
                con_set_pos(pos_x + i * 2 - offset_x + 1,
                            draw_rect.y + draw_rect.height - board_rect.height + offset_y);
            }
            renderTextColoured(board->user_settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%hs", file);
            free(file);
        }
        // ranks
        for (j = reset_j; j <= height; j++) {
            // if the rank is out of range, skip it
            if (j < 0 || j >= board->height)
                continue;
            char * rank = getRankNotation(j);
            con_set_pos(draw_rect.x - offset_x, pos_y - j + board_rect.y * 2 - offset_y);
            renderTextColoured(board->user_settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%hs", rank);
            free(rank);
        }
    }
}

void renderBoardWithSelection(Board * board, Rect draw_rect, Rect board_rect, int sel_x, int sel_y, bool_t with_coords) {
    renderBoard(board, draw_rect, board_rect, with_coords);

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

                if (!isTileVisible(board_rect, target_x, target_y))
                    continue;
                setCursorAtTile(draw_rect, board_rect, target_x, target_y);

                GamePiece * occupant;

                if (!isMoveValid(board, sel_x, sel_y, target_x, target_y, true))
                    continue;

                if ((occupant = tile->game_piece) != NULL  // tile has a piece
                        && (occupant->team != selected->game_piece->team)  // AND piece is an enemy
                        && (target->type & PATH_TYPE_ATTACK))  // AND the path supports attacking
                    renderGamePieceWithBackground(board->user_settings, board->scenario, occupant, COLOR_GREEN, false);
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

            if (!isTileVisible(board_rect, target_x, target_y))
                continue;
            setCursorAtTile(draw_rect, board_rect, target_x, target_y);

            Tile * target = getTile(board, target_x, target_y);
            GamePiece * occupant;

            if (((occupant = target->game_piece) != NULL) && (occupant->team != selected->game_piece->team))
                renderGamePieceWithBackground(board->user_settings, board->scenario, occupant, COLOR_GREEN, false);
            else if (occupant == NULL)
                renderTextColoured(board->user_settings, COLOR_GREEN, COLOR_LIGHT_GRAY, L" ");
        }
    }
}

void renderScenario(Scenario * scenario, UserSettings * settings, Rect draw_rect, Rect board_rect, bool_t with_coords) {
    Exception exception = {};
    Board * board = createBoard(scenario, settings, &exception);
    if (board == NULL && exception.status) {
        clearRect(draw_rect);
        reportExceptionAtPos(exception, draw_rect.x, draw_rect.y);
        return;
    }
    renderBoard(board, draw_rect, board_rect, with_coords);
    freeBoard(board, false);
}

void setCursorAtTile(Rect draw_rect, Rect board_rect, int x, int y) {
    x = (x - board_rect.x + 1) * 2;
    y = board_rect.y - 2 - y;
    con_set_pos(draw_rect.x + x, draw_rect.y + draw_rect.height + y);
}

bool_t isTileVisible(Rect board_rect, int x, int y) {
    int offset_x = board_rect.x < 0 ? board_rect.x : 0,
        offset_y = board_rect.y < 0 ? board_rect.y : 0;

    x -= board_rect.x - 1 - offset_x;
    y -= board_rect.y - 1 - offset_y;

    return x >= 0 && y >= 0 && x < (board_rect.width + 1) / 2 && y < board_rect.height;
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

void clearRect(Rect rect) {
    for (int i = rect.y; i < rect.y + rect.height;) {
        con_set_pos(rect.x, i++);
        wprintf(L"%*s", rect.width, "");
    }
}
