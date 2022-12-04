#include "specialmove.h"
#include <stdlib.h>

void initSpecialMove(SpecialMove * special_move, int8_t x, int8_t y, bool_t first_move, bool_t vulnerable, int8_t vul_x, int8_t vul_y) {
    special_move->x = x;
    special_move->y = y;
    special_move->first_move = first_move;
    special_move->vulnerable = vulnerable;
    special_move->vul_x = vul_x;
    special_move->vul_y = vul_y;
}

void initSpecialMoveSafe(SpecialMove * special_move, int8_t x, int8_t y, bool_t first_move) {
    return initSpecialMove(special_move, x, y, first_move, false, 0, 0);
}

void initSpecialMoveVulnerable(SpecialMove * special_move, int8_t x, int8_t y, bool_t first_move, int8_t vul_x, int8_t vul_y) {
    return initSpecialMove(special_move, x, y, first_move, true, vul_x, vul_y);
}

void saveSpecialMove(SpecialMove * special_move, FILE * stream) {
    fwrite(special_move, sizeof(SpecialMove), 1, stream);
}

SpecialMove * loadSpecialMove(FILE * stream) {
    SpecialMove * out = malloc(sizeof(SpecialMove));
    fread(out, sizeof(SpecialMove), 1, stream);
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
             special_move.x, special_move.y, special_move.first_move ? "YES" : "NO",
             special_move.vulnerable ? "YES" : "NO", special_move.vul_x, special_move.vul_y);
}
