#include "gamestate.h"

#include "./validation.h"

#include "enum/key.h"

#include "ui/con_lib.h"

GameState * createGameState(Board * board) {
    GameState * out = calloc(1, sizeof(GameState));
    out->board = board;
    createBoardPathing(board);
    return out;
}

void saveGameState(GameState * game_state, FILE * stream) {
    saveBoard(game_state->board, stream);
}

GameState * loadGameState(UserSettings * settings, FILE * stream, Exception * exception) {
    Board * board = loadBoard(settings, stream, exception);
    if (board == NULL && exception->status)
        return NULL;
    return createGameState(board);
}

void saveGameStateDefault(GameState * game_state) {
    FILE * file = fopen(GAME_STATE_SAVE_FILE, "wb");
    if (file == NULL)
        return;
    saveGameState(game_state, file);
    fclose(file);
}

GameState * loadGameStateDefault(UserSettings * settings, Exception * exception) {
    FILE * file = fopen(GAME_STATE_SAVE_FILE, "rb");
    if (file == NULL)
        return NULL;
    GameState * out = loadGameState(settings, file, exception);
    fclose(file);
    return out;
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
    if (!state->piece_selected)  // if nothing is selected
        reselectGameAtCursor(state);  // just select
    else {
        if (isMoveValid(state->board, state->sel_x, state->sel_y, state->cur_x, state->cur_y, true)) {  // valid basic/attack move

            moveBoardGamePiece(state->board, state->sel_x, state->sel_y, state->cur_x, state->cur_y);
            nextBoardTurn(state->board);

            state->piece_selected = false;
        } else {  // check for special/none
            Tile * selected = getTile(state->board, state->sel_x, state->sel_y);
            Piece * piece = getOriginalPiece(selected->game_piece, state->board->scenario);

            if (piece != NULL) {
                for (move_index_t move_index = 0; move_index < piece->move_set.special_count;) {  // loop over every special
                    SpecialMove * special = piece->move_set.specials + move_index++;

                    Vector vector = toVector(special->data.vector);

                    if (state->sel_x + vector.x != state->cur_x || state->sel_y + vector.y != state->cur_y)  // if the result of the special move doesn't match the target under the cursor - it's the wrong move
                        continue;

                    if (!isSpecialMoveValid(state->board, state->sel_x, state->sel_y, special))  // if it's not a valid special move
                        continue;

                    moveBoardGamePiece(state->board, state->sel_x, state->sel_y, state->sel_x + vector.x, state->sel_y + vector.y);  // move the main piece

                    for (special_extra_index_t i = 0; i < special->extra_count;) {  // move every extra piece
                        SpecialMoveExtra extra = special->extra[i++];

                        ucoord_t pos_x = state->sel_x + extra.piece_location.x,
                                 pos_y = state->sel_y + extra.piece_location.y;

                        Vector extra_vector = toVector(extra.data.vector);

                        moveBoardGamePiece(state->board, pos_x, pos_y, pos_x + extra_vector.x, pos_y + extra_vector.y);
                    }
                    nextBoardTurn(state->board);  // finish turn

                    state->piece_selected = false;
                    return;
                }
            }

            reselectGameAtCursor(state);
        }
    }
}

bool_t evaluateGameInput(GameState * state, bool_t * game_active) {
    key_code_t key;
    if ((key = con_read_key()) != 0) {
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

            default:
                break;
        }

        return true;
    }
    return false;
}
