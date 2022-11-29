#include <stdio.h>
#include "con_lib.h"
#include "piece.h"

void generateOriginalSet();

int main() {

}

void generateOriginalSet() {
    FILE * file = fopen("set/original.chess", "wb");

    // pawn
    Move pawn_moves[1];
    fillMove(pawn_moves + 0, 0, 1);
    Move pawn_attack[2];
    fillMove(pawn_attack + 0, 1, 1);
    fillMove(pawn_attack + 1, -1, 1);
    MoveSet * pawn_set = createMoveSet(pawn_moves, 1, pawn_attack, 2);
    Piece * piece = createPiece();
}
