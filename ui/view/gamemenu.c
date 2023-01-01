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
    int16_t victor = -1;

    while (game_active) {
        if (evaluateGameInput(state, &game_active))
            renderGameScreen(settings, state, board);
//        con_sleep(0.08f);
    }

    if (victor == -1) {
        FILE * file = fopen("./data/save.bin", "wb");

        if (file != NULL) {
            saveGameState(state, file);
            fclose(file);
        }
    } else {
        con_set_pos(2, 12);
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%hs ", (board->teams + victor)->name);
        renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"wins!");
        con_sleep(3.0f);
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
