#include "board.h"

#include <string.h>
#include <wchar.h>

#include "./validation.h"

#include "ui/con_lib.h"
#include "ui/render.h"
#include "ui/view/promotionmenu.h"

#define EXCEPTION_BOARD_MULTIPLE_PROTECT 0x10001, false, "A team cannot have multiple Protect-flagged pieces"
#define EXCEPTION_BOARD_NO_PROTECT 0x10002, false, "A team needs to have at least one Protect-flagged piece"
#define EXCEPTION_BOARD_NO_PROMOTIONS 0x10003, false, "A promotable piece exists, but no pieces can be promoted to"

Board * createEmptyBoard(Scenario * scenario, UserSettings * settings, Exception * exception) {
    Board * out = calloc(1, sizeof(Board));

    out->user_settings = settings;
    out->scenario = scenario;

    out->width = scenario->size_x;
    out->height = scenario->size_y;

    out->tiles = calloc(scenario->size_x * scenario->size_y, sizeof(Tile *));
    for (tile_index_t i = 0; i < scenario->size_x * scenario->size_y; i++) {
        out->tiles[i] = calloc(1, sizeof(Tile));
        out->tiles[i]->x = i % scenario->size_x;
        out->tiles[i]->y = i / scenario->size_x;
    }

    out->team_count = scenario->team_count;
    out->teams = calloc(scenario->team_count, sizeof(Team));
    for (team_index_t i = 0; i < scenario->team_count; i++) {
        out->teams[i] = scenario->teams[i];

        bool_t has_promotable = false;

        Team * team = out->teams + i;
        for (piece_index_t piece_type = 0; piece_type < team->piece_count; piece_type++) {
            Piece * piece = team->pieces + piece_type;

            if (piece->promotable) {
                has_promotable = true;
            } else if (!piece->protected) {
                addPromotion(team, piece_type);
            }
        }

        if (has_promotable && team->promotion_count == 0) {
            updateException(exception, EXCEPTION_BOARD_NO_PROMOTIONS);
            freeBoard(out, false);
            return NULL;
        }
    }

    return out;
}

Board * createBoard(Scenario * scenario, UserSettings * settings, Exception * exception) {
    Board * out = createEmptyBoard(scenario, settings, exception);
    if (out == NULL && exception->status)
        return NULL;

    for (spawn_index_t i = 0; i < scenario->spawn_count;) {
        Spawn * spawn = scenario->spawns + i++;
        Team * team = out->teams + spawn->team;
        Piece * piece = team->pieces + spawn->type;

        GamePiece * game_piece = createGamePiece(piece, spawn->type);
        Tile * tile = getTile(out, spawn->x, spawn->y);
        tile->game_piece = game_piece;
        game_piece->position = tile;

        if (piece->protected) {
            if (team->protected_piece != NULL) {
                updateException(exception, EXCEPTION_BOARD_MULTIPLE_PROTECT);
                freeBoard(out, false);
                return NULL;
            }
            team->protected_piece = game_piece;
        }
    }

    for (team_index_t i = 0; i < out->team_count;) {
        if (out->teams[i++].protected_piece == NULL) {
            updateException(exception, EXCEPTION_BOARD_NO_PROTECT);
            freeBoard(out, false);
            return NULL;
        }
    }

    return out;
}

void saveBoard(Board * board, FILE * stream) {
    saveScenario(board->scenario, stream, false);
    for (tile_index_t i = 0; i < board->width * board->height;) {
        saveTile(board->tiles[i++], stream);
    }
    fwrite(&board->active_turn, sizeof(uint8_t), 1, stream);
}

Board * loadBoard(UserSettings * settings, FILE * stream, Exception * exception) {
    Scenario * scenario = loadScenario(stream, false, exception);
    if (scenario == NULL && exception->status)
        return NULL;

    Board * out = createEmptyBoard(scenario, settings, exception);
    if (out == NULL && exception->status)
        return NULL;

    for (tile_index_t i = 0; i < scenario->size_x * scenario->size_y; i++) {
        Tile * board_tile = out->tiles[i];
        Tile * tile = loadTile(stream);

        board_tile->phantom_count = tile->phantom_count;
        board_tile->phantoms = tile->phantoms;

        if (tile->game_piece != NULL) {
            Team * team = getGamePieceTeam(out, tile->game_piece);

            board_tile->game_piece = tile->game_piece;
            tile->game_piece->position = board_tile;

            if (getOriginalPiece(tile->game_piece, scenario)->protected) {
                if (team->protected_piece != NULL) {
                    updateException(exception, EXCEPTION_BOARD_MULTIPLE_PROTECT);
                    free(tile);
                    freeBoard(out, true);
                    return NULL;
                }
                team->protected_piece = tile->game_piece;
            }
        }
        free(tile);
    }

    for (team_index_t i = 0; i < out->team_count;) {
        if (out->teams[i++].protected_piece == NULL) {
            updateException(exception, EXCEPTION_BOARD_NO_PROTECT);
            freeBoard(out, true);
            return NULL;
        }
    }

    fread(&out->active_turn, sizeof(team_index_t), 1, stream);

    return out;
}

Team * getTeam(Board * board, ucoord_t index) {
    if (index >= board->team_count)
        return NULL;
    return board->teams + index;
}

Team * getActiveTeam(Board * board) {
    return getTeam(board, board->active_turn);
}

Team * getPieceTeam(Board * board, Piece * piece) {
    return getTeam(board, piece->team);
}

Team * getGamePieceTeam(Board * board, GamePiece * piece) {
    return getTeam(board, piece->team);
}

Tile * getTile(Board * board, ucoord_t x, ucoord_t y) {
    return board->tiles[x + board->width * y];
}

GamePiece * getBoardGamePiece(Board * board, ucoord_t x, ucoord_t y) {
    return getTile(board, x, y)->game_piece;
}

Piece * getTilePiece(Board * board, Tile * tile) {
    if (tile->game_piece == NULL)
        return NULL;
    return getOriginalPiece(tile->game_piece, board->scenario);
}

void nextBoardTurn(Board * board) {
    if (board->active_turn++ >= board->team_count - 1) board->active_turn = 0;
    for (tile_index_t i = 0; i < board->width * board->height;)
        updatePhantoms(board->tiles[i++]);
}

void moveBoardGamePiece(Board * board, ucoord_t from_x, ucoord_t from_y, ucoord_t to_x, ucoord_t to_y) {
    Tile * from = getTile(board, from_x, from_y),
         * to = getTile(board, to_x, to_y);

    freeGamePiece(to->game_piece);

    to->game_piece = from->game_piece;
    to->game_piece->moves++;
    from->game_piece = NULL;

    to->game_piece->position = to;

    capturePhantoms(board, to);

    updateTilePaths(board, from_x, from_y);
    updateTilePaths(board, to_x, to_y);
}

bool_t canPromoteGamePiece(Board * board, GamePiece * game_piece) {
    if (game_piece == NULL)
        return false;

    Piece * piece = getOriginalPiece(game_piece, board->scenario);
    Team * team = getGamePieceTeam(board, game_piece);

    if (!piece->promotable)
        return false;

    switch (team->direction) {
        case TeamDirectionUp:
            return game_piece->position->y == 0;
        case TeamDirectionDown:
            return game_piece->position->y == board->height - 1;
        case TeamDirectionLeft:
            return game_piece->position->x == 0;
        case TeamDirectionRight:
            return game_piece->position->x == board->width - 1;
    }
    return false;
}

void promoteGamePiece(Board * board, GamePiece * game_piece, piece_index_t new_type) {
    if (game_piece == NULL)
        return;

    game_piece->piece = new_type;
    updateTilePaths(board, game_piece->position->x, game_piece->position->y);
}

Board * cloneBoard(Board * board) {
    Exception exception;
    Board * out = createEmptyBoard(board->scenario, board->user_settings, &exception);
    if (out == NULL && exception.status) {
        reportException(exception);
        return NULL;
    }

    for (tile_index_t i = 0; i < board->width * board->height; i++) {
        out->tiles[i]->game_piece = cloneGamePiece(board->tiles[i]->game_piece);

        GamePiece * game_piece = out->tiles[i]->game_piece;
        if (game_piece == NULL)
            continue;

        Piece * piece = getOriginalPiece(game_piece, board->scenario);
        if (piece->protected) {
            Team *team = getTeam(out, piece->team);
            team->protected_piece = game_piece;
        }
    }

    createBoardPathing(out);

    out->active_turn = board->active_turn;
    return out;
}

void handleGamePiecePromotion(Board * board, GamePiece * game_piece) {
    if (canPromoteGamePiece(board, game_piece))
        promoteGamePiece(board, game_piece, promotionMenuLoop(board->user_settings, getGamePieceTeam(board, game_piece)));
}

void freeBoard(Board * board, bool_t free_scenario) {
    for (tile_index_t i = 0; i < board->width * board->height;)
        freeTile(board->tiles[i++]);
    for (team_index_t i = 0; i < board->team_count;)
        clearPromotions(&board->teams[i++]);
    free(board->tiles);
    free(board->teams);
    if (free_scenario)
        freeScenario(board->scenario);
    free(board);
}
