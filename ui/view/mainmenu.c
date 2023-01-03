#include <malloc.h>
#include "mainmenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "../render.h"
#include "scenariomenu.h"
#include "../../utils/files.h"
#include "../../engine/board.h"
#include "gamemenu.h"
#include "../../abstract/version.h"

MENU_SELECTOR_INIT_CALLBACK(initMainMenu);
MENU_SELECTOR_UPDATE_CALLBACK(updateMainMenu);

MENU_ITEM_CALLBACK(onMainMenuResume);
MENU_ITEM_CALLBACK(onMainMenuStart);
MENU_ITEM_CALLBACK(onMainMenuExit);

void mainMenuLoop(UserSettings * settings) {
    initMainMenu(settings);

    MenuSelector * selector = createMenuSelector(settings, initMainMenu, updateMainMenu);

    if (isPathFile("./data/save.bin"))
        addMenuItem(selector, L"Resume", "Let's get back into the fight", onMainMenuResume);
    addMenuItem(selector, L"New Scenario", "New day, new battle", onMainMenuStart);
    addMenuItem(selector, L"Quit", "Leaving already?", onMainMenuExit);

    while (updateMenuSelector(selector, true)) {
        displayMenuSelector(selector, 2, 4);
    }
}

MENU_SELECTOR_INIT_CALLBACK(initMainMenu) {
    con_clear();

    con_set_pos(2, 1);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GREY, L"%hs", getVersionName(BUILD_VERSION));
    con_set_pos(2, 2);
    renderTextColoured(settings, COLOR_RESET, COLOR_WHITE, L"Rook's Gambit");

    con_flush();
}

MENU_SELECTOR_UPDATE_CALLBACK(updateMainMenu) {
    con_set_pos(5, 8);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GREEN, L"%-*hs", MENU_ITEM_MAX_STRING_LEN, data);
    con_flush();
}

MENU_ITEM_CALLBACK(onMainMenuResume) {
    FILE * file = fopen("./data/save.bin", "rb");
    Board * board;

    if (file != NULL) {
        Exception exception = {};
        board = loadBoard(settings, file, &exception);
        if (board == NULL && exception.status) {
            reportException(exception);
            fclose(file);
            return false;
        }
        fclose(file);
    } else {
        return false;
    }

    gameLoop(settings, board);
    return false;
}

MENU_ITEM_CALLBACK(onMainMenuStart) {
    scenarioMenuLoop(settings);
    return false;
}

MENU_ITEM_CALLBACK(onMainMenuExit) {
    con_clear();
    con_show_cursor(1);
    return true;
}
