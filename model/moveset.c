#include "moveset.h"
#include <stdlib.h>
#include <wchar.h>

void initMoveSet(MoveSet * move_set, Move * moves, uint8_t move_count, Move * attacks, uint8_t attack_count, SpecialMove * specials, uint8_t special_count) {
    move_set->moves = moves;
    move_set->move_count = move_count;
    move_set->attacks = attacks;
    move_set->attack_count = attack_count;
    move_set->specials = specials;
    move_set->special_count = special_count;
}

void saveMoveSet(MoveSet * move_set, FILE * stream) {
    fwrite(&move_set->move_count, sizeof(uint8_t), 1, stream);
    for (int i = 0; i < move_set->move_count; i++)
        fwrite(move_set->moves + i, sizeof(Move), 1, stream);

    fwrite(&move_set->attack_count, sizeof(uint8_t), 1, stream);
    for (int i = 0; i < move_set->attack_count; i++)
        fwrite(move_set->attacks + i, sizeof(Move), 1, stream);

    fwrite(&move_set->special_count, sizeof(uint8_t), 1, stream);
    for (int i = 0; i < move_set->special_count; i++)
        fwrite(move_set->specials + i, sizeof(SpecialMove), 1, stream);
}

MoveSet * loadMoveSet(FILE * stream) {
    MoveSet * out = malloc(sizeof(MoveSet));

    fread(&out->move_count, sizeof(uint8_t), 1, stream);
    out->moves = malloc(sizeof(Move) * out->move_count);
    fread(out->moves, sizeof(Move), out->move_count, stream);

    fread(&out->attack_count, sizeof(uint8_t), 1, stream);
    out->attacks = malloc(sizeof(Move) * out->attack_count);
    fread(out->attacks, sizeof(Move), out->attack_count, stream);

    fread(&out->special_count, sizeof(uint8_t), 1, stream);
    out->specials = malloc(sizeof(Move) * out->special_count);
    fread(out->specials, sizeof(SpecialMove), out->special_count, stream);

    return out;
}

void printMoveSet(MoveSet * move_set) {
    wprintf(L"Move Set:\n"
             "\tMoves: (%d)\n", move_set->move_count);
    for (int i = 0; i < move_set->move_count; i++) {
        wprintf(L"\t\t");
        printMove(move_set->moves[i]);
    }

    wprintf(L"\tAttacks: (%d)\n", move_set->attack_count);
    for (int i = 0; i < move_set->attack_count; i++) {
        wprintf(L"\t\t");
        printMove(move_set->attacks[i]);
    }

    wprintf(L"\tSpecials: (%d)\n", move_set->special_count);
    for (int i = 0; i < move_set->special_count; i++) {
        wprintf(L"\t\t");
        printSpecialMove(move_set->specials[i]);
    }
}
