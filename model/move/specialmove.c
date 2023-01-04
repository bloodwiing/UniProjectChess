#include "specialmove.h"

#include <stdlib.h>
#include <string.h>

SpecialMove createSpecialMove(SpecialData data) {
    return (SpecialMove){
        .data = data,
        .extra_count = 0
    };
}

void saveSpecialMove(SpecialMove move, FILE * stream) {
    saveSpecialData(move.data, stream);
    fwrite(&move.extra_count, sizeof(special_extra_index_t), 1, stream);
    for (special_extra_index_t i = 0; i < move.extra_count; i++) {
        fwrite(&move.extra[i].piece_location, sizeof(Vector8), 1, stream);
        fwrite(&move.extra[i].piece_type, sizeof(piece_index_t), 1, stream);
        saveSpecialData(move.extra[i].data, stream);
    }
}

SpecialMove loadSpecialMove(FILE * stream) {
    SpecialMove out = {};

    out.data = loadSpecialData(stream);

    fread(&out.extra_count, sizeof(special_extra_index_t), 1, stream);

    out.extra = calloc(out.extra_count, sizeof(SpecialMoveExtra));
    for (special_extra_index_t i = 0; i < out.extra_count; i++) {
        fread(&out.extra[i].piece_location, sizeof(Vector8), 1, stream);
        fread(&out.extra[i].piece_type, sizeof(piece_index_t), 1, stream);
        out.extra[i].data = loadSpecialData(stream);
    }

    return out;
}

void printSpecialMove(SpecialMove special_move) {
    wprintf(L"SpecialMove: \n"
              "\tData:\n");
    printSpecialData(special_move.data);
    wprintf(L"\tExtra count: %d\n"
              "\tExtras:\n",
              special_move.extra_count);
    for (special_extra_index_t i = 0; i < special_move.extra_count; i++) {
        SpecialMoveExtra extra = special_move.extra[i];
        wprintf(L"\t\tX: %d\n"
                 "\t\tY: %d\n",
                extra.piece_location.x, extra.piece_location.y);
        printSpecialData(extra.data);
    }
}

void addSpecialMoveExtra(SpecialMove * special_move, Vector8 piece_location, piece_index_t piece_type, SpecialData data) {
    special_move->extra = realloc(special_move->extra, sizeof(SpecialMoveExtra) * ++special_move->extra_count);
    SpecialMoveExtra * extra = &special_move->extra[special_move->extra_count - 1];
    memset(extra, 0, sizeof(SpecialMoveExtra));
    extra->piece_location = piece_location;
    extra->piece_type = piece_type;
    extra->data = data;
}

void normaliseSpecialMove(SpecialMove * special_move, TeamDirection direction) {
    normaliseSpecialData(&special_move->data, direction);
    for (special_extra_index_t i = 0; i < special_move->extra_count; i++)
        normaliseSpecialData(&special_move->extra[i].data, direction);
}

void freeSpecialMove(SpecialMove * special_move) {
    for (special_extra_index_t i = 0; i < special_move->extra_count;)
        freeSpecialData(&special_move->extra[i++].data);
    free(special_move->extra);
    freeSpecialData(&special_move->data);
}
