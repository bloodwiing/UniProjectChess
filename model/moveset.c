#include "moveset.h"

#include <stdlib.h>
#include <wchar.h>
#include <string.h>

MoveSet createMoveSet() {
    return (MoveSet){};
}

void saveMoveSet(MoveSet move_set, FILE * stream) {
    fwrite(&move_set.move_count, sizeof(move_index_t), 1, stream);
    for (move_index_t i = 0; i < move_set.move_count;)
        saveMove(move_set.moves[i++], stream);

    fwrite(&move_set.attack_count, sizeof(move_index_t), 1, stream);
    for (move_index_t i = 0; i < move_set.attack_count;)
        saveMove(move_set.attacks[i++], stream);

    fwrite(&move_set.special_count, sizeof(move_index_t), 1, stream);
    for (move_index_t i = 0; i < move_set.special_count;)
        saveSpecialMove(move_set.specials[i++], stream);
}

MoveSet loadMoveSet(FILE * stream) {
    MoveSet out = {};

    fread(&out.move_count, sizeof(move_index_t), 1, stream);
    out.moves = calloc(out.move_count, sizeof(Move));
    for (move_index_t i = 0; i < out.move_count;)
        out.moves[i++] = loadMove(stream);

    fread(&out.attack_count, sizeof(move_index_t), 1, stream);
    out.attacks = calloc(out.attack_count, sizeof(Move));
    for (move_index_t i = 0; i < out.attack_count;)
        out.attacks[i++] = loadMove(stream);

    fread(&out.special_count, sizeof(move_index_t), 1, stream);
    out.specials = calloc(out.special_count, sizeof(SpecialMove));
    for (move_index_t i = 0; i < out.special_count;)
        out.specials[i++] = loadSpecialMove(stream);

    return out;
}

void printMoveSet(MoveSet move_set) {
    wprintf(L"Move Set:\n"
             "\tMoves: (%d)\n", move_set.move_count);
    for (move_index_t i = 0; i < move_set.move_count; i++) {
        wprintf(L"\t\t");
        printMove(move_set.moves[i]);
    }

    wprintf(L"\tAttacks: (%d)\n", move_set.attack_count);
    for (move_index_t i = 0; i < move_set.attack_count; i++) {
        wprintf(L"\t\t");
        printMove(move_set.attacks[i]);
    }

    wprintf(L"\tSpecials: (%d)\n", move_set.special_count);
    for (move_index_t i = 0; i < move_set.special_count; i++) {
        wprintf(L"\t\t");
        printSpecialMove(move_set.specials[i]);
    }
}

void addBasicMove(MoveSet * move_set, Move move) {
    move_set->moves = realloc(move_set->moves, sizeof(Move) * ++move_set->move_count);
    memset(&move_set->moves[move_set->move_count - 1], 0, sizeof(Move));
    move_set->moves[move_set->move_count - 1] = move;
}

void addAttackMove(MoveSet * move_set, Move move) {
    move_set->attacks = realloc(move_set->attacks, sizeof(Move) * ++move_set->attack_count);
    memset(&move_set->attacks[move_set->attack_count - 1], 0, sizeof(Move));
    move_set->attacks[move_set->attack_count - 1] = move;
}

void addSpecialMove(MoveSet * move_set, SpecialMove move) {
    move_set->specials = realloc(move_set->specials, sizeof(SpecialMove) * ++move_set->special_count);
    memset(&move_set->specials[move_set->special_count - 1], 0, sizeof(SpecialMove));
    move_set->specials[move_set->special_count - 1] = move;
}

void addBasicAttackMove(MoveSet * move_set, Move move) {
    addBasicMove(move_set, move);
    addAttackMove(move_set, move);
}

void appendBasicMoves(MoveSet * move_set, Move * moves, size_t n) {
    for (size_t i = 0; i < n;)
        addBasicMove(move_set, moves[i++]);
}

void appendAttackMoves(MoveSet * move_set, Move * moves, size_t n) {
    for (size_t i = 0; i < n;)
        addAttackMove(move_set, moves[i++]);
}

void appendSpecialMoves(MoveSet * move_set, SpecialMove * moves, size_t n) {
    for (size_t i = 0; i < n;)
        addSpecialMove(move_set, moves[i++]);
}

void appendBasicAttackMoves(MoveSet * move_set, Move * moves, size_t n) {
    for (size_t i = 0; i < n;)
        addBasicAttackMove(move_set, moves[i++]);
}

void freeMoveSet(MoveSet * move_set) {
    free(move_set->moves);
    free(move_set->attacks);
    free(move_set->specials);
}
