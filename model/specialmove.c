#include "specialmove.h"

#include <stdlib.h>

SpecialMove createSpecialMove(Vector8 vector, bool_t is_first_move, bool_t is_vulnerable, Vector8 vulnerable) {
    return (SpecialMove){
        .vector = vector,
        .is_first_move = is_first_move,
        .is_vulnerable = is_vulnerable,
        .vulnerable = vulnerable
    };
}

SpecialMove createSpecialMoveSafe(Vector8 vector, bool_t is_first_move) {
    return createSpecialMove(vector, is_first_move, false, VECTOR8_ZERO);
}

SpecialMove createSpecialMoveVulnerable(Vector8 vector, bool_t is_first_move, Vector8 vulnerable) {
    return createSpecialMove(vector, is_first_move, true, vulnerable);
}

SpecialMove createSpecialMoveRaw(coord_t x, coord_t y, bool_t is_first_move, bool_t is_vulnerable, coord_t vul_x, coord_t vul_y) {
    return createSpecialMove(createVector8(x, y), is_first_move, is_vulnerable, createVector8(vul_x, vul_y));
}

SpecialMove createSpecialMoveSafeRaw(coord_t x, coord_t y, bool_t is_first_move) {
    return createSpecialMove(createVector8(x, y), is_first_move, false, VECTOR8_ZERO);
}

SpecialMove createSpecialMoveVulnerableRaw(coord_t x, coord_t y, bool_t is_first_move, coord_t vul_x, coord_t vul_y) {
    return createSpecialMove(createVector8(x, y), is_first_move, true, createVector8(vul_x, vul_y));
}

void saveSpecialMove(SpecialMove move, FILE * stream) {
    fwrite(&move.vector, sizeof(Vector8), 1, stream);
    fwrite(&move.is_first_move, sizeof(bool_t), 1, stream);
    fwrite(&move.is_vulnerable, sizeof(bool_t), 1, stream);
    fwrite(&move.vulnerable, sizeof(Vector8), 1, stream);
}

SpecialMove loadSpecialMove(FILE * stream) {
    SpecialMove out = {};
    fread(&out.vector, sizeof(Vector8), 1, stream);
    fread(&out.is_first_move, sizeof(bool_t), 1, stream);
    fread(&out.is_vulnerable, sizeof(bool_t), 1, stream);
    fread(&out.vulnerable, sizeof(Vector8), 1, stream);
    return out;
}

void printSpecialMove(SpecialMove special_move) {
    wprintf(L"SpecialMove: \n"
             "\tX: %d\n"
             "\tY: %d\n"
             "\tFirst move: %hs\n"
             "\tVulnerable: %hs\n"
             "\tVulnerable X: %d\n"
             "\tVulnerable Y: %d\n",
             special_move.vector.x, special_move.vector.y, special_move.is_first_move ? "YES" : "NO",
             special_move.is_vulnerable ? "YES" : "NO", special_move.vulnerable.x, special_move.vulnerable.y);
}
