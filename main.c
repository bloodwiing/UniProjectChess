#include <stdio.h>
#include "defaults.h"
#include "ui/board.h"

int main() {
    Scenario * scenario = createDefaultScenario();

    Board * board = createBoard(scenario);
    renderBoard(board, 0, 0, 0, 0, 50, 50);
}
