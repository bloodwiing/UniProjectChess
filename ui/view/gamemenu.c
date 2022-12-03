#include "gamemenu.h"
#include "../board.h"
#include "../con_lib.h"
#include "../../enum/key.h"
#include "../../engine/gamestate.h"
#include "../render.h"

void gameLoop(UserSettings * settings, Scenario * scenario) {
    con_clear();
    Board * board = createBoard(scenario, settings);
    GameState * state = createGameState(board);

    renderBoard(board, 2, 2, 0, 0, 30, 10);

    Piece * piece = board->tiles[0];
    if (piece != NULL) {
        renderPieceWithBackground(settings, board->teams + piece->team, piece, COLOR_DARK_GRAY);
    } else {
        renderTextColoured(settings, COLOR_DARK_GRAY, COLOR_LIGHT_GRAY, L"X");
    }

    uint32_t key;
    while (1) {

        while ((key = con_read_key()) != 0) {
            switch (key) {
                case KEY_ARROW_UP:
                case KEY_W:
                    if (state->cur_y-- == 0) state->cur_y = state->board->height - 1;
                    break;

                case KEY_ARROW_DOWN:
                case KEY_S:
                    if (state->cur_y++ == state->board->height - 1) state->cur_y = 0;
                    break;

                case KEY_ARROW_LEFT:
                case KEY_A:
                    if (state->cur_x-- == 0) state->cur_x = state->board->width - 1;
                    break;

                case KEY_ARROW_RIGHT:
                case KEY_D:
                    if (state->cur_x++ == state->board->width - 1) state->cur_x = 0;
                    break;

                case KEY_ENTER:
                    if (board->tiles[state->cur_x + board->width * state->cur_y] == NULL)
                        break;
                    state->sel_x = state->cur_x;
                    state->sel_y = state->cur_y;
                    state->piece_selected = true;
                    break;
            }

            if (!state->piece_selected)
                renderBoard(board, 2, 2, 0, 0, 30, 10);
            else
                renderBoardWithSelection(board, 2, 2, 0, 0, 30, 10, state->sel_x, state->sel_y);

            con_set_pos(state->cur_x * 2 + 4, state->cur_y + 3);
            piece = board->tiles[state->cur_x + board->width * state->cur_y];

            if (piece != NULL) {
                renderPieceWithBackground(settings, board->teams + piece->team, piece, COLOR_DARK_GRAY);
            } else {
                renderTextColoured(settings, COLOR_DARK_GRAY, COLOR_LIGHT_GRAY, L"X");
            }
        }

        con_sleep(0.08f);
    }

    free(state);
    free(board);
}
