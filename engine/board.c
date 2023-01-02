#include "board.h"
#include "../ui/con_lib.h"
#include <string.h>
#include "../ui/render.h"
#include "validation.h"
#include <wchar.h>

#define EXCEPTION_BOARD_MULTIPLE_PROTECT 1, false, "A team cannot have multiple Protect-flagged pieces"

Board * createEmptyBoard(Scenario * scenario, UserSettings * settings) {
    Board * out = malloc(sizeof(Board));

    out->user_settings = settings;
    out->scenario = scenario;

    out->width = scenario->size_x;
    out->height = scenario->size_y;

    out->tiles = calloc(scenario->size_x * scenario->size_y, sizeof(Tile *));
    for (int i = 0; i < scenario->size_x * scenario->size_y; i++) {
        out->tiles[i] = calloc(1, sizeof(Tile));
        out->tiles[i]->x = i % scenario->size_x;
        out->tiles[i]->y = i / scenario->size_x;
    }

    out->team_count = scenario->team_count;
    out->teams = calloc(scenario->team_count, sizeof(Team));
    for (int i = 0; i < scenario->team_count; i++)
        out->teams[i] = scenario->teams[i];

    return out;
}

Board * createBoard(Scenario * scenario, UserSettings * settings, Exception * exception) {
    Board * out = createEmptyBoard(scenario, settings);

    for (int i = 0; i < scenario->spawn_count;) {
        Spawn * spawn = scenario->spawns + i++;
        Team * team = scenario->teams + spawn->team;
        Piece * piece = team->pieces + spawn->type;

        GamePiece * game_piece = createGamePiece(piece, spawn->type);
        getTile(out, spawn->x, spawn->y)->game_piece = game_piece;

        if (piece->protect) {
            if (team->protected_piece == NULL) {
                updateException(exception, EXCEPTION_BOARD_MULTIPLE_PROTECT);
                return NULL;
            }
            team->protected_piece = game_piece;
        }
    }

    out->active_turn = 1;

    return out;
}

void saveBoard(Board * board, FILE * stream) {
    saveScenario(board->scenario, stream);
    for (int i = 0; i < board->width * board->height;) {
        saveTile(board->tiles[i++], stream);
    }
    fwrite(&board->active_turn, sizeof(uint8_t), 1, stream);
}

Board * loadBoard(UserSettings * settings, FILE * stream, Exception * exception) {
    Scenario * scenario = loadScenario(stream);
    Board * out = createEmptyBoard(scenario, settings);

    for (int i = 0; i < scenario->size_x * scenario->size_y; i++) {
        Tile * tile = loadTile(stream);
        if (tile->game_piece != NULL) {
            Team * team = getGamePieceTeam(out, tile->game_piece);
            out->tiles[i]->game_piece = tile->game_piece;

            if (getOriginalPiece(tile->game_piece, scenario)->protect) {
                if (team->protected_piece == NULL) {
                    updateException(exception, EXCEPTION_BOARD_MULTIPLE_PROTECT);
                    return NULL;
                }
                team->protected_piece = tile->game_piece;
            }
        }
        free(tile);
    }

    fread(&out->active_turn, sizeof(uint8_t), 1, stream);

    return out;
}

Team * getTeam(Board * board, int index) {
    if (index < 0 || index >= board->team_count)
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

Tile * getTile(Board * board, int x, int y) {
    return board->tiles[x + board->width * y];
}

GamePiece * getBoardGamePiece(Board * board, int x, int y) {
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

void moveBoardGamePiece(Board * board, int from_x, int from_y, int to_x, int to_y) {
    Tile * from = getTile(board, from_x, from_y),
         * to = getTile(board, to_x, to_y);

    freeGamePiece(to->game_piece);

    to->game_piece = from->game_piece;
    to->game_piece->moves++;
    from->game_piece = NULL;

    updateTilePaths(board, from_x, from_y);
    updateTilePaths(board, to_x, to_y);
}
