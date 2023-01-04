#include "gamemenu.h"

#include "ui/render.h"

void renderGameScreen(UserSettings * settings, GameState * state, Board * board);

void resumeGameLoop(UserSettings * settings, GameState * state, bool_t save_state) {
    con_clear();
    renderGameScreen(settings, state, state->board);

    bool_t game_active = true;

    while (game_active) {
        if (evaluateGameInput(state, &game_active))
            renderGameScreen(settings, state, state->board);
//        con_sleep(0.08f);
    }

    if (save_state)
        saveGameStateDefault(state);
}

void beginNewGameLoop(UserSettings * settings, Board * board, bool_t save_state) {
    GameState * state = createGameState(board);
    resumeGameLoop(settings, state, save_state);
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
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Current team: ");
    renderTextColoured(settings, COLOR_RESET, getActiveTeam(board)->colour, L"%-*hs", TEAM_NAME_LENGTH, getActiveTeam(board)->name);
    con_set_pos(32, 3);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Selected piece: ");
    if (state->piece_selected)
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%-*hs", PIECE_NAME_LENGTH,
                           getOriginalPiece(board->tiles[state->sel_x + board->width * state->sel_y]->game_piece, board->scenario)->name);
    else
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%-*ls", PIECE_NAME_LENGTH, L"None");

    con_flush();
}
