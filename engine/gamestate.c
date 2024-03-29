#include "gamestate.h"

#include "./validation.h"

#include "enum/key.h"

#include "ui/con_lib.h"
#include "ui/view/promotionmenu.h"

#define LOG_MODULE L"GameState"

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
    logInfo(settings, LOG_MODULE, L"Loading state from file...");
    Board * board = loadBoard(settings, stream, exception);
    if (board == NULL && exception->status) {
        logError(settings, LOG_MODULE, L"Load failed!");
        return NULL;
    }
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
    logInfo(settings, LOG_MODULE, L"Loading state from file...");
    FILE * file = fopen(GAME_STATE_SAVE_FILE, "rb");
    if (file == NULL) {
        logError(settings, LOG_MODULE, L"Load failed!");
        return NULL;
    }
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

void executeGameMove(UserSettings * settings, GameState * state) {
    if (!state->piece_selected) {  // if nothing is selected
        logInfo(settings, LOG_MODULE, L"Selecting piece at cursor");
        logDebug(settings, LOG_MODULE, L"cursor x=%d y=%d", state->cur_x, state->cur_y);
        reselectGameAtCursor(state);  // just select
    }
    else {
        logInfo(settings, LOG_MODULE, L"Checking possible actions");

        if (isMoveValid(state->board, state->sel_x, state->sel_y, state->cur_x, state->cur_y, true)) {  // valid basic/attack move
            logInfo(settings, LOG_MODULE, L"Move validated!");

            logInfo(settings, LOG_MODULE, L"Moving piece...");
            moveBoardGamePiece(state->board, state->sel_x, state->sel_y, state->cur_x, state->cur_y);
            nextBoardTurn(state->board);

            GamePiece * game_piece = getGamePieceAtCursor(state);
            logInfo(settings, LOG_MODULE, L"Checking for promotions...");
            handleGamePiecePromotion(state->board, game_piece);  // promotion check

            state->stale = !hasTeamPossibleMoves(state->board, getActiveTeam(state->board));
            state->check = isTeamChecked(getActiveTeam(state->board));

            state->piece_selected = false;
        }
        else {  // check for special/none
            Tile * selected = getTile(state->board, state->sel_x, state->sel_y);
            GamePiece * game_piece = selected->game_piece;
            Piece * piece = getOriginalPiece(game_piece, state->board->scenario);

            logInfo(settings, LOG_MODULE, L"Checking special moves...");
            if (piece != NULL) {
                for (move_index_t move_index = 0; move_index < piece->move_set.special_count;) {  // loop over every special
                    SpecialMove * special = piece->move_set.specials + move_index++;

                    Vector vector = toVector(special->data.vector);

                    if (state->sel_x + vector.x != state->cur_x || state->sel_y + vector.y != state->cur_y)  // if the result of the special move doesn't match the target under the cursor - it's the wrong move
                        continue;

                    if (!isSpecialMoveValid(state->board, state->sel_x, state->sel_y, special))  // if it's not a valid special move
                        continue;

                    logInfo(settings, LOG_MODULE, L"Validated special move!");

                    logInfo(settings, LOG_MODULE, L"Moving piece...");
                    moveBoardGamePiece(state->board, state->sel_x, state->sel_y, state->sel_x + vector.x, state->sel_y + vector.y);  // move the main piece
                    if (special->data.is_phantom) {  // register phantom piece
                        logDebug(settings, LOG_MODULE, L"phantom x=%d y=%d", special->data.phantom.x, special->data.phantom.y);
                        Tile * phantom_tile = getTile(state->board, state->sel_x + vector.x + special->data.phantom.x, state->sel_y + vector.y + special->data.phantom.y);
                        addPhantom(state->board, phantom_tile, game_piece);
                    }

                    logInfo(settings, LOG_MODULE, L"Checking for promotions...");
                    handleGamePiecePromotion(state->board, game_piece);  // promotion check

                    logInfo(settings, LOG_MODULE, L"Moving extra pieces...");
                    for (special_extra_index_t i = 0; i < special->extra_count;) {  // move every extra piece
                        SpecialMoveExtra extra = special->extra[i++];

                        logDebug(settings, LOG_MODULE, L"special_extra_index=%d", i-1);

                        ucoord_t pos_x = state->sel_x + extra.piece_location.x,
                                 pos_y = state->sel_y + extra.piece_location.y;

                        GamePiece * extra_game_piece = getBoardGamePiece(state->board, pos_x, pos_y);
                        Vector extra_vector = toVector(extra.data.vector);

                        logInfo(settings, LOG_MODULE, L"Moving piece...");
                        moveBoardGamePiece(state->board, pos_x, pos_y, pos_x + extra_vector.x, pos_y + extra_vector.y);
                        if (extra.data.is_phantom) {  // register phantom pieces
                            logDebug(settings, LOG_MODULE, L"phantom x=%d y=%d", extra.data.phantom.x, extra.data.phantom.y);
                            Tile * phantom_tile = getTile(state->board, pos_x + extra_vector.x + extra.data.phantom.x, pos_y + extra_vector.y + extra.data.phantom.y);
                            addPhantom(state->board, phantom_tile, extra_game_piece);
                        }

                        logInfo(settings, LOG_MODULE, L"Checking for promotions...");
                        handleGamePiecePromotion(state->board, extra_game_piece);  // promotion check
                    }
                    nextBoardTurn(state->board);  // finish turn

                    state->stale = !hasTeamPossibleMoves(state->board, getActiveTeam(state->board));
                    state->check = isTeamChecked(getActiveTeam(state->board));

                    state->piece_selected = false;
                    return;
                }
            }

            logInfo(settings, LOG_MODULE, L"Fallback: reselect");
            logDebug(settings, LOG_MODULE, L"cursor x=%d y=%d", state->cur_x, state->cur_y);
            reselectGameAtCursor(state);
        }
    }
}

bool_t evaluateGameInput(UserSettings * settings, GameState * state, bool_t * game_active) {
    key_code_t key;
    if ((key = con_read_key()) != 0) {
        switch (key) {
            CASE_KEY_DOWN:
                logInfo(settings, LOG_MODULE, L"Moving cursor down...");
                if (state->cur_y-- <= 0) state->cur_y = state->board->height - 1;
                break;

            CASE_KEY_UP:
                logInfo(settings, LOG_MODULE, L"Moving cursor up...");
                if (state->cur_y++ >= state->board->height - 1) state->cur_y = 0;
                break;

            CASE_KEY_LEFT:
                logInfo(settings, LOG_MODULE, L"Moving cursor left...");
                if (state->cur_x-- <= 0) state->cur_x = state->board->width - 1;
                break;

            CASE_KEY_RIGHT:
                logInfo(settings, LOG_MODULE, L"Moving cursor right...");
                if (state->cur_x++ >= state->board->width - 1) state->cur_x = 0;
                break;

            CASE_KEY_CONFIRM:
                logInfo(settings, LOG_MODULE, L"Running action...");
                executeGameMove(settings, state);
                if (state->stale)
                    *game_active = false;
                break;

            CASE_KEY_CANCEL:
                *game_active = false;
                break;

            default:
                break;
        }

        return true;
    }
    return false;
}
