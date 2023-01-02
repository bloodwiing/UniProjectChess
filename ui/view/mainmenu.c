#include <malloc.h>
#include "mainmenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "../render.h"
#include "scenariomenu.h"
#include "../../utils/files.h"
#include "../../engine/board.h"
#include "gamemenu.h"

void updateMainMenu(UserSettings * settings, char * data);

void onMainMenuResume(UserSettings * settings, char * data);
void onMainMenuStart(UserSettings * settings, char * data);
void onMainMenuExit(UserSettings * settings, char * data);

void mainMenuLoop(UserSettings * settings) {
    con_clear();
    MenuSelector * selector = createMenuSelector(settings, updateMainMenu);

    con_set_pos(2, 1);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GREY, L"v0.4 beta");
    con_set_pos(2, 2);
    renderTextColoured(settings, COLOR_RESET, COLOR_WHITE, L"Rook's Gambit");

    if (isPathFile("./data/save.bin"))
        addMenuItem(selector, L"Resume", "Let's get back into the fight", onMainMenuResume);
    addMenuItem(selector, L"New Scenario", "New day, new battle", onMainMenuStart);
    addMenuItem(selector, L"Quit", "Leaving already?", onMainMenuExit);

    while (updateMenuSelector(selector)) {
        displayMenuSelector(selector, 2, 4);
    }

    free(selector);
}

void updateMainMenu(UserSettings * settings, char * data) {
    con_set_pos(5, 8);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GREEN, L"%-*hs", MENU_ITEM_MAX_STRING_LEN, data);
}

void onMainMenuResume(UserSettings * settings, char * data) {
    FILE * file = fopen("./data/save.bin", "rb");
    Board * board;

    if (file != NULL) {
        Exception exception = {};
        board = loadBoard(settings, file, &exception);
        if (board == NULL && exception.status) {
            reportException(exception);
            fclose(file);
            return;
        }
        fclose(file);
    } else {
        mainMenuLoop(settings);
        return;
    }

    gameLoop(settings, board);
    mainMenuLoop(settings);
}

void onMainMenuStart(UserSettings * settings, char * data) {
    scenarioMenuLoop(settings);
}

void onMainMenuExit(UserSettings * settings, char * data) {
    con_clear();
    con_show_cursor(1);
}
