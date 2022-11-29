#include <stdio.h>
#include "con_lib.h"
#include "defaults.h"

int main() {
    Board * board = createDefaultBoard();

    printBoard(board);

    FILE * stream = fopen("set/original.chess", "wb");
    saveBoard(board, stream);
    fclose(stream);

    stream = fopen("set/original.chess", "rb");
    Board * loaded = loadBoard(stream);
    fclose(stream);

    printBoard(loaded);
}
