#include "board.h"
#include "../ui/con_lib.h"
#include <string.h>
#include "../ui/render.h"
#include "validation.h"
#include <wchar.h>

#define EXCEPTION_BOARD_MULTIPLE_PROTECT 1, false, "A team cannot have multiple Protect-flagged pieces"
#define EXCEPTION_BOARD_NO_PROTECT 2, false, "A team needs to have at least one Protect-flagged piece"

Board * createEmptyBoard(Scenario * scenario, UserSettings * settings) {
    Board * out = malloc(sizeof(Board));

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
    for (team_index_t i = 0; i < scenario->team_count; i++)
        out->teams[i] = scenario->teams[i];

    return out;
}

Board * createBoard(Scenario * scenario, UserSettings * settings, Exception * exception) {
    Board * out = createEmptyBoard(scenario, settings);

    for (spawn_index_t i = 0; i < scenario->spawn_count;) {
        Spawn * spawn = scenario->spawns + i++;
        Team * team = out->teams + spawn->team;
        Piece * piece = team->pieces + spawn->type;

        GamePiece * game_piece = createGamePiece(piece, spawn->type);
        Tile * tile = getTile(out, spawn->x, spawn->y);
        tile->game_piece = game_piece;
        game_piece->position = tile;

        if (piece->protect) {
            if (team->protected_piece != NULL) {
                updateException(exception, EXCEPTION_BOARD_MULTIPLE_PROTECT);
                freeBoard(out);
                return NULL;
            }
            team->protected_piece = game_piece;
        }
    }

    for (team_index_t i = 0; i < out->team_count;) {
        if (out->teams[i++].protected_piece == NULL) {
            updateException(exception, EXCEPTION_BOARD_NO_PROTECT);
            freeBoard(out);
            return NULL;
        }
    }

    out->active_turn = 1;

    return out;
}

void saveBoard(Board * board, FILE * stream) {
    saveScenario(board->scenario, stream);
    for (tile_index_t i = 0; i < board->width * board->height;) {
        saveTile(board->tiles[i++], stream);
    }
    fwrite(&board->active_turn, sizeof(uint8_t), 1, stream);
}

Board * loadBoard(UserSettings * settings, FILE * stream, Exception * exception) {
    Scenario * scenario = loadScenario(stream);
    Board * out = createEmptyBoard(scenario, settings);

    for (tile_index_t i = 0; i < scenario->size_x * scenario->size_y; i++) {
        Tile * tile = loadTile(stream);
        if (tile->game_piece != NULL) {
            Team * team = getGamePieceTeam(out, tile->game_piece);
            out->tiles[i]->game_piece = tile->game_piece;

            if (getOriginalPiece(tile->game_piece, scenario)->protect) {
                if (team->protected_piece != NULL) {
                    updateException(exception, EXCEPTION_BOARD_MULTIPLE_PROTECT);
                    free(tile);
                    freeBoard(out);
                    freeScenario(scenario);
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
            freeBoard(out);
            freeScenario(scenario);
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
}

void moveBoardGamePiece(Board * board, ucoord_t from_x, ucoord_t from_y, ucoord_t to_x, ucoord_t to_y) {
    Tile * from = getTile(board, from_x, from_y),
         * to = getTile(board, to_x, to_y);

    freeGamePiece(to->game_piece);

    to->game_piece = from->game_piece;
    to->game_piece->moves++;
    from->game_piece = NULL;

    to->game_piece->position = to;

    updateTilePaths(board, from_x, from_y);
    updateTilePaths(board, to_x, to_y);
}

void freeBoard(Board * board) {
    for (tile_index_t i = 0; i < board->width * board->height;)
        freeTile(board->tiles[i++]);
    free(board->tiles);
    free(board->teams);
    free(board);
}
