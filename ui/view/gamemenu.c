#include "gamemenu.h"

#include "ui/render.h"

void renderGameScreen(UserSettings * settings, GameState * state, Board * board);

void resumeGameLoop(UserSettings * settings, GameState * state, bool_t save_state) {
    con_clear();
    renderGameScreen(settings, state, state->board);

    bool_t game_active = true;

    while (game_active) {
        if (hasConsoleSizeChanged(settings)) {
            con_clear();
            renderGameScreen(settings, state, state->board);
        }

        if (evaluateGameInput(state, &game_active))
            renderGameScreen(settings, state, state->board);
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
    Rect draw_rect = createRect(2, 1, settings->size.width / 2 - 4, settings->size.height - 1);
    Rect board_rect = getBoardCenteredRect(board, draw_rect, state->cur_x, state->cur_y);

    if (!state->piece_selected)
        renderBoard(board, draw_rect, board_rect, state->cur_x, state->cur_y, true);
    else
        renderBoardWithSelection(board, draw_rect, board_rect, state->cur_x, state->cur_y, state->sel_x, state->sel_y, true);

    setCursorAtTile(draw_rect, board_rect, state->cur_x, state->cur_y);
    GamePiece * game_piece = getGamePieceAtCursor(state);

    if (game_piece != NULL) {
        Piece piece = *getOriginalPiece(game_piece, board->scenario);
        Team team = board->teams[game_piece->team];
        renderPieceWithBackground(settings, team, piece, COLOR_BLACK, true);
    } else {
        renderTextColoured(settings, COLOR_DARK_GRAY, COLOR_LIGHT_GRAY, L"X");
    }

    int right = settings->size.width - settings->size.width / 2;

    con_set_pos(right, 2);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Current team: ");
    renderTextColoured(settings, COLOR_RESET, getActiveTeam(board)->colour, L"%-*hs", TEAM_NAME_LENGTH, getActiveTeam(board)->name);
    con_set_pos(right, 3);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Selected piece: ");
    if (state->piece_selected)
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%-*hs", PIECE_NAME_LENGTH,
                           getOriginalPiece(board->tiles[state->sel_x + board->width * state->sel_y]->game_piece, board->scenario)->name);
    else
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%-*ls", PIECE_NAME_LENGTH, L"None");

    con_flush();
}
