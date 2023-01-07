#include "mainmenu.h"

#include <malloc.h>

#include "./gamemenu.h"
#include "./scenariomenu.h"

#include "ui/component/menuselector.h"
#include "ui/render.h"

#include "utils/files.h"

MENU_SELECTOR_INIT_CALLBACK(initMainMenu);
MENU_SELECTOR_UPDATE_CALLBACK(updateMainMenu);

MENU_ITEM_CALLBACK(onMainMenuResume);
MENU_ITEM_CALLBACK(onMainMenuStart);
MENU_ITEM_CALLBACK(onMainMenuExit);

void mainMenuLoop(UserSettings * settings) {
    initMainMenu(settings);

    MenuSelector * selector = createMenuSelector(settings, initMainMenu, updateMainMenu);

    if (isPathFile(GAME_STATE_SAVE_FILE))
        addMenuItem(selector, L"Resume", "Let's get back into the fight", NULL, onMainMenuResume);
    addMenuItem(selector, L"New Scenario", "New day, new battle", NULL, onMainMenuStart);
    addMenuItem(selector, L"Quit", "Leaving already?", NULL, onMainMenuExit);

    while (updateMenuSelector(selector, true)) {
        displayMenuSelector(selector, offsetRect(getScreenRect(), 2, 4, -2, -4));
    }
}

MENU_SELECTOR_INIT_CALLBACK(initMainMenu) {
    con_clear();

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_DARK_GREY, RECT_LINE(2, 1, settings->size.width - 2), L"%hs", getVersionName(BUILD_VERSION));
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_WHITE, RECT_LINE(2, 2, settings->size.width - 2), L"Rook's Gambit");

    con_flush();
}

MENU_SELECTOR_UPDATE_CALLBACK(updateMainMenu) {
    if (settings->size.height < 12)
        return;

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GREEN, createRect(5, 8, settings->size.width - 5, 2), L"%-*hs", MENU_ITEM_MAX_STRING_LEN, text_data);
    con_flush();
}

MENU_ITEM_CALLBACK(onMainMenuResume) {
    Exception exception = {};
    GameState * state = loadGameStateDefault(settings, &exception);
    if (state == NULL && exception.status) {
        reportException(settings, exception);
        return false;
    }

    resumeGameLoop(settings, state, true);
    freeBoard(state->board, true);
    free(state);
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
