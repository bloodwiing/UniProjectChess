#include "defaults.h"

#include "abstract/defs.h"

#include "model/move.h"
#include "model/moveset.h"
#include "model/spawn.h"

#include "ui/con_lib.h"

Piece createDefaultPawn(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    addBasicMove(&move_set, createMoveRaw(0, 1, false));

    // attack possibilities
    addAttackMove(&move_set, createMoveRaw(1, 1, false));
    addAttackMove(&move_set, createMoveRaw(-1, 1, false));

    // special possibilities
    addSpecialMove(&move_set, createSpecialMoveVulnerableRaw(0, 2, true, 0, -1));

    // create the piece
    return createPiece("Pawn", team == 1 ? L"♟" : L"♙", team == 1 ? 'p' : 'P', true, false, team, move_set);
}

Piece createDefaultRook(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[4] = {
        createMoveRaw(0, 1, true),  // bottom
        createMoveRaw(0, -1, true),  // top
        createMoveRaw(1, 0, true),  // right
        createMoveRaw(-1, 0, true)  // left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 4);

    // create the piece
    return createPiece("Rook", team == 1 ? L"♜" : L"♖", team == 1 ? 'r' : 'R', false, false, team, move_set);
}

Piece createDefaultKnight(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[8] = {
        createMoveRaw(1, 2, false),  // right 2*bottom
        createMoveRaw(1, -2, false),  // right 2*top

        createMoveRaw(-1, 2, false),  // left 2*bottom
        createMoveRaw(-1, -2, false),  // left 2*top

        createMoveRaw(2, 1, false),  // bottom 2*right
        createMoveRaw(-2, 1, false),  // bottom 2*left

        createMoveRaw(2, -1, false),  // top 2*right
        createMoveRaw(-2, -1, false)  // top 2*left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 8);

    // create the piece
    return createPiece("Knight", team == 1 ? L"♞" : L"♘", team == 1 ? 'n' : 'N', false, false, team, move_set);
}

Piece createDefaultBishop(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[4] = {
        createMoveRaw(1, 1, true),  // bottom right
        createMoveRaw(-1, 1, true),  // bottom left
        createMoveRaw(1, -1, true),  // top right
        createMoveRaw(-1, -1, true)  // top left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 4);

    // create the piece
    return createPiece("Bishop", team == 1 ? L"♝" : L"♗", team == 1 ? 'b' : 'B', false, false, team, move_set);
}

Piece createDefaultQueen(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[8] = {
        createMoveRaw(0, 1, true),  // bottom
        createMoveRaw(0, -1, true),  // top
        createMoveRaw(1, 0, true),  // right
        createMoveRaw(-1, 0, true),  // left

        createMoveRaw(1, 1, true),  // bottom right
        createMoveRaw(-1, 1, true),  // bottom left
        createMoveRaw(1, -1, true),  // top right
        createMoveRaw(-1, -1, true)  // top left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 8);

    // create the piece
    return createPiece("Queen", team == 1 ? L"♛" : L"♕", team == 1 ? 'q' : 'Q', false, false, team, move_set);
}

Piece createDefaultKing(uint8_t team) {
    MoveSet move_set = createMoveSet();

    // move possibilities
    Move moves[8] = {
        createMoveRaw(0, 1, false),  // bottom
        createMoveRaw(0, -1, false),  // top
        createMoveRaw(1, 0, false),  // right
        createMoveRaw(-1, 0, false),  // left

        createMoveRaw(1, 1, false),  // bottom right
        createMoveRaw(-1, 1, false),  // bottom left
        createMoveRaw(1, -1, false),  // top right
        createMoveRaw(-1, -1, false)  // top left
    };

    // compile move set
    appendBasicAttackMoves(&move_set, moves, 8);

    // create the piece
    return createPiece("King", team == 1 ? L"♚" : L"♔", team == 1 ? 'k' : 'K', false, true, team, move_set);
}

Team createDefaultTeamWhite() {
    Team team = createTeam("White", COLOR_LIGHT_ORANGE, TeamDirectionDown);

    addPiece(&team, createDefaultPawn(1));
    addPiece(&team, createDefaultRook(1));
    addPiece(&team, createDefaultKnight(1));
    addPiece(&team, createDefaultBishop(1));
    addPiece(&team, createDefaultQueen(1));
    addPiece(&team, createDefaultKing(1));

    return team;
}

Team createDefaultTeamBlack() {
    Team team = createTeam("Black", COLOR_RED, TeamDirectionUp);

    addPiece(&team, createDefaultPawn(0));
    addPiece(&team, createDefaultRook(0));
    addPiece(&team, createDefaultKnight(0));
    addPiece(&team, createDefaultBishop(0));
    addPiece(&team, createDefaultQueen(0));
    addPiece(&team, createDefaultKing(0));

    return team;
}

Scenario * createDefaultScenario() {
    Scenario * scenario = createScenario("Classic", "BLOODWIING", 8, 8);

    addTeam(scenario, createDefaultTeamBlack());
    addTeam(scenario, createDefaultTeamWhite());

    addSpawn(scenario, createSpawn(0, 0, 0, 1));  // black rook
    addSpawn(scenario, createSpawn(1, 0, 0, 2));  // black knight
    addSpawn(scenario, createSpawn(2, 0, 0, 3));  // black bishop
    addSpawn(scenario, createSpawn(3, 0, 0, 4));  // black queen
    addSpawn(scenario, createSpawn(4, 0, 0, 5));  // black king
    addSpawn(scenario, createSpawn(5, 0, 0, 3));  // black bishop
    addSpawn(scenario, createSpawn(6, 0, 0, 2));  // black knight
    addSpawn(scenario, createSpawn(7, 0, 0, 1));  // black rook
    for (int i = 0; i < 8; i++) {
        addSpawn(scenario, createSpawn(i, 1, 0, 0));  // black pawn
        addSpawn(scenario, createSpawn(i, 6, 1, 0));  // white pawn
    }
    addSpawn(scenario, createSpawn(0, 7, 1, 1));  // white rook
    addSpawn(scenario, createSpawn(1, 7, 1, 2));  // white knight
    addSpawn(scenario, createSpawn(2, 7, 1, 3));  // white bishop
    addSpawn(scenario, createSpawn(3, 7, 1, 4));  // white queen
    addSpawn(scenario, createSpawn(4, 7, 1, 5));  // white king
    addSpawn(scenario, createSpawn(5, 7, 1, 3));  // white bishop
    addSpawn(scenario, createSpawn(6, 7, 1, 2));  // white knight
    addSpawn(scenario, createSpawn(7, 7, 1, 1));  // white rook

    return scenario;
}