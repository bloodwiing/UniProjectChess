#include <stdio.h>
#include <locale.h>

#include "defaults.h"

#include "engine/board.h"

#include "settings/settings.h"

#include "ui/con_lib.h"
#include "ui/view/mainmenu.h"
#include "ui/render.h"

void setupConsole();

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

    saveDefaultScenario();

//    Exception exception;
//    FILE * file = fopen("./data/save.bin", "rb");
//    Board * board = loadBoard(settings, file, &exception);
//    fclose(file);
//    if (board == NULL && exception.status) {
//        reportException(exception);
//    }
//    renderBoardWithSelection(board, 2, 2, 0, 0, 30, 10, 4, 7);
//    freeBoard(board, true);

    mainMenuLoop(settings);
    freeSettings(settings);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
