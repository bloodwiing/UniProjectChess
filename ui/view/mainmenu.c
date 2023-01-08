#include "mainmenu.h"

#include <malloc.h>

#include "./gamemenu.h"
#include "./scenariomenu.h"

#include "ui/component/menuselector.h"
#include "ui/render.h"

#include "utils/files.h"

#define LOG_MODULE L"MainMenu"

MENU_SELECTOR_INIT_CALLBACK(initMainMenu);
MENU_SELECTOR_UPDATE_CALLBACK(updateMainMenu);

MENU_ITEM_CALLBACK(onMainMenuResume);
MENU_ITEM_CALLBACK(onMainMenuStart);
MENU_ITEM_CALLBACK(onMainMenuExit);

void mainMenuLoop(UserSettings * settings) {
    logInfo(settings, LOG_MODULE, L"Running main menu...");
    initMainMenu(settings);

    MenuSelector * selector = createMenuSelector(settings, initMainMenu, updateMainMenu);

    if (isPathFile(GAME_STATE_SAVE_FILE))
        addMenuItem(selector, L"Resume", "Let's get back into the fight", selector, onMainMenuResume);
    addMenuItem(selector, L"New Scenario", "New day, new battle", selector, onMainMenuStart);
    addMenuItem(selector, L"Quit", "Leaving already?", selector, onMainMenuExit);

    runMenuSelectorUpdateCallback(selector);
    while (updateMenuSelector(selector, true));
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

    displayMenuSelector(other_data, offsetRect(getScreenRect(), 2, 4, -2, -4));

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GREEN, createRect(5, 8, settings->size.width - 5, 2), L"%hs", text_data);
    con_flush();
}

MENU_ITEM_CALLBACK(onMainMenuResume) {
    logInfo(settings, LOG_MODULE, L"Resuming previous game...");
    Exception exception = {};
    GameState * state = loadGameStateDefault(settings, &exception);
    if (state == NULL && exception.status) {
        logError(settings, LOG_MODULE, L"Failed to resume: %hs", exception.message);
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
    logInfo(settings, LOG_MODULE, L"Exiting...");
    return true;
}
