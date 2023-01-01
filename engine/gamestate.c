#include "gamestate.h"
#include "validation.h"
#include "../ui/con_lib.h"
#include "../enum/key.h"

GameState * createGameState(Board * board) {
    GameState * out = malloc(sizeof(GameState));
    out->board = board;
    out->piece_selected = false;
    out->cur_x = out->cur_y = out->sel_x = out->sel_y = 0;
    createBoardPathing(board);
    return out;
}

void saveGameState(GameState * game_state, FILE * stream) {
    saveBoard(game_state->board, stream);
}

GameState * loadGameState(UserSettings * settings, FILE * stream) {
    return createGameState(loadBoard(settings, stream));
}

GamePiece * getGamePieceAtCursor(GameState * state) {
    return getBoardGamePiece(state->board, state->cur_x, state->cur_y);
}

GamePiece * getGamePieceSelected(GameState * state) {
    return getBoardGamePiece(state->board, state->sel_x, state->sel_y);
}

bool_t isGameCursorOnSelected(GameState * state) {
    return state->cur_x == state->sel_x && state->cur_y == state->sel_y;
}

void reselectGameAtCursor(GameState * state) {
    GamePiece * game_piece;
    if ((game_piece = getGamePieceAtCursor(state)) == NULL || (state->piece_selected && isGameCursorOnSelected(state)) || game_piece->team != state->board->active_turn) {
        state->piece_selected = false;
        return;
    }

    state->sel_x = state->cur_x;
    state->sel_y = state->cur_y;
    state->piece_selected = true;
}

void executeGameMove(GameState * state) {
    if (!state->piece_selected)
        reselectGameAtCursor(state);
    else {
        if (validatePath(state->board, state->sel_x, state->sel_y, state->cur_x, state->cur_y)) {
            moveBoardGamePiece(state->board, state->sel_x, state->sel_y, state->cur_x, state->cur_y);
            nextBoardTurn(state->board);
            state->piece_selected = false;
        } else {
            reselectGameAtCursor(state);
        }
    }
}

bool_t evaluateGameInput(GameState * state, bool_t * game_active) {
    uint32_t key;
    while ((key = con_read_key()) != 0) {
        switch (key) {
            case KEY_ARROW_UP:
            case KEY_W:
                if (state->cur_y-- <= 0) state->cur_y = state->board->height - 1;
                break;

            case KEY_ARROW_DOWN:
            case KEY_S:
                if (state->cur_y++ >= state->board->height - 1) state->cur_y = 0;
                break;

            case KEY_ARROW_LEFT:
            case KEY_A:
                if (state->cur_x-- <= 0) state->cur_x = state->board->width - 1;
                break;

            case KEY_ARROW_RIGHT:
            case KEY_D:
                if (state->cur_x++ >= state->board->width - 1) state->cur_x = 0;
                break;

            case KEY_ENTER:
                executeGameMove(state);
                break;

            case KEY_Q:
                *game_active = false;
                break;
        }
        return true;
    }
    return false;
}
