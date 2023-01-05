#include "defaults.h"

#include "abstract/defs.h"

#include "model/move/move.h"
#include "model/move/moveset.h"
#include "model/spawn.h"

#include "ui/con_lib.h"

#include "utils/files.h"

Piece createDefaultPawn(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    addBasicMove(&move_set, createMoveRaw(0, 1, false, false));

    // attack possibilities
    addAttackMove(&move_set, createMoveRaw(1, 1, false, false));
    addAttackMove(&move_set, createMoveRaw(-1, 1, false, false));

    // special possibilities
    SpecialMove double_move = createSpecialMove(createSpecialDataRaw(0, 2, true, true, false));
    addSpecialDataConditionRaw(&double_move.data, 0, 1);
    updateSpecialDataPhantomRaw(&double_move.data, 0, -1, " e.p.");
    addSpecialMove(&move_set, double_move);

    // create the piece
    return createPiece(
        "Pawn",
        team == DEFAULT_TEAM_TYPE_WHITE ? L"♙" : L"♟",
        team == DEFAULT_TEAM_TYPE_WHITE ? 'P' : 'p',
        L"",
        '\0',
        true,
        false,
        team,
        move_set
    );
}

Piece createDefaultRook(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[4] = {
        createMoveRaw(0, 1, true, false),  // bottom
        createMoveRaw(0, -1, true, false),  // top
        createMoveRaw(1, 0, true, false),  // right
        createMoveRaw(-1, 0, true, false)  // left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 4);

    // create the piece
    return createPiece(
        "Rook",
        team == DEFAULT_TEAM_TYPE_WHITE ? L"♖" : L"♜",
        team == DEFAULT_TEAM_TYPE_WHITE ? 'R' : 'r',
        L"♜",
        'R',
        false,
        false,
        team,
        move_set
    );
}

Piece createDefaultKnight(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[8] = {
        createMoveRaw(1, 2, false, false),  // right 2*bottom
        createMoveRaw(1, -2, false, false),  // right 2*top

        createMoveRaw(-1, 2, false, false),  // left 2*bottom
        createMoveRaw(-1, -2, false, false),  // left 2*top

        createMoveRaw(2, 1, false, false),  // bottom 2*right
        createMoveRaw(-2, 1, false, false),  // bottom 2*left

        createMoveRaw(2, -1, false, false),  // top 2*right
        createMoveRaw(-2, -1, false, false)  // top 2*left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 8);

    // create the piece
    return createPiece(
        "Knight",
        team == DEFAULT_TEAM_TYPE_WHITE ? L"♘" : L"♞",
        team == DEFAULT_TEAM_TYPE_WHITE ? 'N' : 'n',
        L"♞",
        'N',
        false,
        false,
        team,
        move_set
    );
}

Piece createDefaultBishop(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[4] = {
        createMoveRaw(1, 1, true, false),  // bottom right
        createMoveRaw(-1, 1, true, false),  // bottom left
        createMoveRaw(1, -1, true, false),  // top right
        createMoveRaw(-1, -1, true, false)  // top left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 4);

    // create the piece
    return createPiece(
        "Bishop",
        team == DEFAULT_TEAM_TYPE_WHITE ? L"♗" : L"♝",
        team == DEFAULT_TEAM_TYPE_WHITE ? 'B' : 'b',
        L"♝",
        'B',
        false,
        false,
        team,
        move_set
    );
}

Piece createDefaultQueen(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[8] = {
        createMoveRaw(0, 1, true, false),  // bottom
        createMoveRaw(0, -1, true, false),  // top
        createMoveRaw(1, 0, true, false),  // right
        createMoveRaw(-1, 0, true, false),  // left

        createMoveRaw(1, 1, true, false),  // bottom right
        createMoveRaw(-1, 1, true, false),  // bottom left
        createMoveRaw(1, -1, true, false),  // top right
        createMoveRaw(-1, -1, true, false)  // top left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 8);

    // create the piece
    return createPiece(
        "Queen",
        team == DEFAULT_TEAM_TYPE_WHITE ? L"♕" : L"♛",
        team == DEFAULT_TEAM_TYPE_WHITE ? 'Q' : 'q',
        L"♛",
        'Q',
        false,
        false,
        team,
        move_set
    );
}

Piece createDefaultKing(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[8] = {
        createMoveRaw(0, 1, false, false),  // bottom
        createMoveRaw(0, -1, false, false),  // top
        createMoveRaw(1, 0, false, false),  // right
        createMoveRaw(-1, 0, false, false),  // left

        createMoveRaw(1, 1, false, false),  // bottom right
        createMoveRaw(-1, 1, false, false),  // bottom left
        createMoveRaw(1, -1, false, false),  // top right
        createMoveRaw(-1, -1, false, false)  // top left
    };

    // special possibilities
    SpecialMove castle_king_side = createSpecialMove(createSpecialDataRaw(2, 0, true, false, true));
    addSpecialDataConditionRaw(&castle_king_side.data, 1, 0);
    addSpecialMoveExtra(
            &castle_king_side,
            createVector8(3, 0),
            DEFAULT_PIECE_TYPE_ROOK,
            createSpecialDataRaw(-2, 0, true, false, true));
    setSpecialDataNotation(&castle_king_side.data, "O-O");
    addSpecialMove(&move_set, castle_king_side);

    SpecialMove castle_queen_side = createSpecialMove(createSpecialDataRaw(-2, 0, true, false, true));
    addSpecialDataConditionRaw(&castle_queen_side.data, -1, 0);
    addSpecialDataConditionRaw(&castle_queen_side.data, -3, 0);
    addSpecialMoveExtra(
            &castle_queen_side,
            createVector8(-4, 0),
            DEFAULT_PIECE_TYPE_ROOK,
            createSpecialDataRaw(3, 0, true, false, true));
    addSpecialDataConditionRaw(&castle_queen_side.extra[0].data, 1, 0);
    setSpecialDataNotation(&castle_queen_side.data, "O-O-O");
    addSpecialMove(&move_set, castle_queen_side);

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 8);

    // create the piece
    return createPiece(
        "King",
        team == DEFAULT_TEAM_TYPE_WHITE ? L"♔" : L"♚",
        team == DEFAULT_TEAM_TYPE_WHITE ? 'K' : 'k',
        L"♚",
        'K',
        false,
        true,
        team,
        move_set
    );
}

Team createDefaultTeamWhite() {
    Team team = createTeam("White", COLOR_LIGHT_ORANGE, TeamDirectionUp);

    addPiece(&team, createDefaultPawn(DEFAULT_TEAM_TYPE_WHITE));
    addPiece(&team, createDefaultRook(DEFAULT_TEAM_TYPE_WHITE));
    addPiece(&team, createDefaultKnight(DEFAULT_TEAM_TYPE_WHITE));
    addPiece(&team, createDefaultBishop(DEFAULT_TEAM_TYPE_WHITE));
    addPiece(&team, createDefaultQueen(DEFAULT_TEAM_TYPE_WHITE));
    addPiece(&team, createDefaultKing(DEFAULT_TEAM_TYPE_WHITE));

    return team;
}

Team createDefaultTeamBlack() {
    Team team = createTeam("Black", COLOR_RED, TeamDirectionDown);

    addPiece(&team, createDefaultPawn(DEFAULT_TEAM_TYPE_BLACK));
    addPiece(&team, createDefaultRook(DEFAULT_TEAM_TYPE_BLACK));
    addPiece(&team, createDefaultKnight(DEFAULT_TEAM_TYPE_BLACK));
    addPiece(&team, createDefaultBishop(DEFAULT_TEAM_TYPE_BLACK));
    addPiece(&team, createDefaultQueen(DEFAULT_TEAM_TYPE_BLACK));
    addPiece(&team, createDefaultKing(DEFAULT_TEAM_TYPE_BLACK));

    return team;
}

Scenario * createDefaultScenario() {
    Scenario * scenario = createScenario("Classic", "BLOODWIING", 8, 8);

    addTeam(scenario, createDefaultTeamWhite());
    addTeam(scenario, createDefaultTeamBlack());

    addSpawn(scenario, createSpawn(0, 7, DEFAULT_TEAM_TYPE_BLACK, 1));  // black rook
    addSpawn(scenario, createSpawn(1, 7, DEFAULT_TEAM_TYPE_BLACK, 2));  // black knight
    addSpawn(scenario, createSpawn(2, 7, DEFAULT_TEAM_TYPE_BLACK, 3));  // black bishop
    addSpawn(scenario, createSpawn(3, 7, DEFAULT_TEAM_TYPE_BLACK, 4));  // black queen
    addSpawn(scenario, createSpawn(4, 7, DEFAULT_TEAM_TYPE_BLACK, 5));  // black king
    addSpawn(scenario, createSpawn(5, 7, DEFAULT_TEAM_TYPE_BLACK, 3));  // black bishop
    addSpawn(scenario, createSpawn(6, 7, DEFAULT_TEAM_TYPE_BLACK, 2));  // black knight
    addSpawn(scenario, createSpawn(7, 7, DEFAULT_TEAM_TYPE_BLACK, 1));  // black rook
    for (int i = 0; i < 8; i++) {
        addSpawn(scenario, createSpawn(i, 6, DEFAULT_TEAM_TYPE_BLACK, 0));  // black pawn
        addSpawn(scenario, createSpawn(i, 1, DEFAULT_TEAM_TYPE_WHITE, 0));  // white pawn
    }
    addSpawn(scenario, createSpawn(0, 0, DEFAULT_TEAM_TYPE_WHITE, 1));  // white rook
    addSpawn(scenario, createSpawn(1, 0, DEFAULT_TEAM_TYPE_WHITE, 2));  // white knight
    addSpawn(scenario, createSpawn(2, 0, DEFAULT_TEAM_TYPE_WHITE, 3));  // white bishop
    addSpawn(scenario, createSpawn(3, 0, DEFAULT_TEAM_TYPE_WHITE, 4));  // white queen
    addSpawn(scenario, createSpawn(4, 0, DEFAULT_TEAM_TYPE_WHITE, 5));  // white king
    addSpawn(scenario, createSpawn(5, 0, DEFAULT_TEAM_TYPE_WHITE, 3));  // white bishop
    addSpawn(scenario, createSpawn(6, 0, DEFAULT_TEAM_TYPE_WHITE, 2));  // white knight
    addSpawn(scenario, createSpawn(7, 0, DEFAULT_TEAM_TYPE_WHITE, 1));  // white rook

    return scenario;
}

void saveDefaultScenario() {
    char * path = combinePath(SCENARIO_FOLDER, "classic.chess");
    FILE * file = fopen(path, "wb");

    if (file == NULL) {
        free(path);
        return;
    }

    Scenario * scenario = createDefaultScenario();
    saveScenario(scenario, file, true);
    freeScenario(scenario);

    fclose(file);
    free(path);
}
