#include "move.h"
#include <wchar.h>
#include <stdlib.h>

Move createMove(Vector8 vector, bool_t repeat) {
    return (Move){.vector = vector, .repeat = repeat};
}

Move createMoveRaw(coord_t x, coord_t y, bool_t repeat) {
    return createMove(createVector8(x, y), repeat);
}

void saveMove(Move move, FILE * stream) {
    fwrite(&move.vector, sizeof(Vector8), 1, stream);
    fwrite(&move.repeat, sizeof(bool_t), 1, stream);
}

Move loadMove(FILE * stream) {
    Move out = {};
    fread(&out.vector, sizeof(Vector8), 1, stream);
    fread(&out.repeat, sizeof(bool_t), 1, stream);
    return out;
}

void printMove(Move move) {
    wprintf(L"x %d, y %d, repeat %s\n",
             move.vector.x, move.vector.y, move.repeat == true ? "YES" : "NO");
}
