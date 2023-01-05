#include <stdio.h>
#include <locale.h>

#include "defaults.h"

#include "engine/board.h"
#include "engine/moveentry.h"

#include "settings/settings.h"

#include "ui/con_lib.h"
#include "ui/notation.h"
#include "ui/view/mainmenu.h"

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

    saveDefaultScenario();

//    Scenario * scenario = createDefaultScenario();
//    Exception exception;
//    Board * board = createBoard(scenario, settings, &exception);
//    if (board == NULL && exception.status) {
//        reportException(exception);
//        return 1;
//    }
//
//    Tile * tile = getTile(board, 0, 1);
//
//    MoveEntry entry = createMoveEntry(getTilePiece(board, tile), tile->game_piece, 255, 254, 255, 255);
//    markMoveEntryPromote(&entry, &getGamePieceTeam(board, tile->game_piece)->pieces[4]);
//    markMoveEntryPhantom(&entry, &getGamePieceTeam(board, tile->game_piece)->pieces[0].move_set.specials[0]);
//    markMoveEntryCheck(&entry, true);
//    markMoveEntryAmbiguous(&entry, false, false);
//    wchar_t * test = generateMoveNotation(settings, entry);
//
//    wprintf(test);
//
//    freeBoard(board, true);
//    free(test);

    mainMenuLoop(settings);
    freeSettings(settings);

    con_show_echo(true);
    con_show_cursor(true);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
