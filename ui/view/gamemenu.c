#include "gamemenu.h"
#include "../../engine/board.h"
#include "../con_lib.h"
#include "../../enum/key.h"
#include "../../engine/gamestate.h"
#include "../render.h"
#include "../../engine/validation.h"

void renderGameScreen(UserSettings * settings, GameState * state, Board * board);

void gameLoop(UserSettings * settings, Board * board) {
    con_clear();
    GameState * state = createGameState(board);

    renderGameScreen(settings, state, board);

    bool_t game_active = true;

    uint32_t key;
    while (game_active) {

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
                    if (!state->piece_selected) {
                        GamePiece * game_piece;
                        if ((game_piece = board->tiles[state->cur_x + board->width * state->cur_y]->game_piece) == NULL)
                            break;
                        if (game_piece->team != board->active_turn)
                            break;
                        state->sel_x = state->cur_x;
                        state->sel_y = state->cur_y;
                        state->piece_selected = true;
                    } else {
                        if (validatePieceAnyMove(board, state->sel_x, state->sel_y, state->cur_x, state->cur_y)) {
                            board->tiles[state->cur_x + board->width * state->cur_y]->game_piece = board->tiles[state->sel_x + board->width * state->sel_y]->game_piece;
                            board->tiles[state->sel_x + board->width * state->sel_y]->game_piece = NULL;
                            board->tiles[state->cur_x + board->width * state->cur_y]->game_piece->moves++;
                            state->piece_selected = false;
                            if (board->active_turn++ >= board->team_count - 1) board->active_turn = 0;
                        } else {
                            state->piece_selected = false;
                        }
                    }
                    break;

                case KEY_Q:
                    game_active = false;
                    break;
            }

            renderGameScreen(settings, state, board);
        }

//        con_sleep(0.08f);
    }

    FILE * file = fopen("./data/save.bin", "wb");

    if (file != NULL) {
        saveGameState(state, file);
        fclose(file);
    }

    free(state);
}

void renderGameScreen(UserSettings * settings, GameState * state, Board * board) {
    if (!state->piece_selected)
        renderBoard(board, 2, 2, 0, 0, 30, 10);
    else
        renderBoardWithSelection(board, 2, 2, 0, 0, 30, 10, state->sel_x, state->sel_y);

    con_set_pos(state->cur_x * 2 + 4, state->cur_y + 3);
    GamePiece * game_piece = board->tiles[state->cur_x + board->width * state->cur_y]->game_piece;

    if (game_piece != NULL) {
        renderPieceWithBackground(settings, board->teams + game_piece->team, getOriginalPiece(game_piece, board->scenario), COLOR_DARK_GRAY);
    } else {
        renderTextColoured(settings, COLOR_DARK_GRAY, COLOR_LIGHT_GRAY, L"X");
    }

    con_set_pos(32, 2);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Current team: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%-*hs", TEAM_NAME_LENGTH, board->teams + board->active_turn);
    con_set_pos(32, 3);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Selected piece: ");
    if (state->piece_selected)
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%-*hs", PIECE_NAME_LENGTH,
                           getOriginalPiece(board->tiles[state->sel_x + board->width * state->sel_y]->game_piece, board->scenario)->name);
    else
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%-*ls", PIECE_NAME_LENGTH, L"None");
}
