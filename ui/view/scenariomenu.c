#include <stdlib.h>
#include <string.h>
#include "scenariomenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "../../utils/files.h"
#include "mainmenu.h"
#include "../../model/scenario.h"
#include "../render.h"
#include "../../engine/board.h"
#include "gamemenu.h"
#include "../../abstract/version.h"

MENU_SELECTOR_INIT_CALLBACK(initScenarioMenu);
MENU_SELECTOR_UPDATE_CALLBACK(updateScenarioMenu);

MENU_ITEM_CALLBACK(onScenarioMenuSelect);
MENU_ITEM_CALLBACK(onScenarioMenuLeave);

void scenarioMenuLoop(UserSettings * settings) {
    initScenarioMenu(settings);

    MenuSelector * selector = createMenuSelector(settings, initScenarioMenu, updateScenarioMenu);

    size_t count = 0;
    char ** files = listDirectoryFiles(SCENARIO_FOLDER, &count);

    for (int i = 0; i < count; i++) {
        wchar_t name[MENU_ITEM_MAX_STRING_LEN];
        mbstowcs(name, files[i], MENU_ITEM_MAX_STRING_LEN);
        addMenuItem(selector, name, files[i], onScenarioMenuSelect);
    }
    addMenuItem(selector, L"Back", "", onScenarioMenuLeave);

    while (updateMenuSelector(selector, true)) {
        displayMenuSelector(selector, 2, 3);
    }

    for (int i = 0; i < count;)
        free(files[i++]);
    free(files);
}

MENU_SELECTOR_INIT_CALLBACK(initScenarioMenu) {
    con_clear();

    con_set_pos(2, 1);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Scenario select");
}

MENU_SELECTOR_UPDATE_CALLBACK(updateScenarioMenu) {
    if (strlen(data) == 0) {
        clearRect(50, 2, 30, 22);
        return;
    }

    FILE * file = fopen(combinePath(SCENARIO_FOLDER, data), "rb");
    Scenario * scenario = loadScenario(file);
    fclose(file);
    renderScenario(scenario, settings, 50, 2, 0, 0, 30, 10);

    con_set_pos(50, 14);
    if (getMinSupportedScenarioVersion(BUILD_VERSION) > scenario->version) {
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->name);
        con_set_pos(50, 15);
        renderTextColoured(settings, COLOR_RESET, COLOR_RED, L"[ %-*hs ]", SCENARIO_MAX_STRING_LEN, getVersionName(scenario->version));
    } else {
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GREEN, L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->name);
        con_set_pos(50, 15);
        renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"[ %-*hs ]", SCENARIO_MAX_STRING_LEN, getVersionName(scenario->version));
    }
    con_set_pos(50, 17);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Author: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"@%-*hs", SCENARIO_MAX_STRING_LEN, scenario->author);
    con_set_pos(50, 19);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Size: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%d x %d", scenario->size_x, scenario->size_y);
    con_set_pos(50, 20);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Teams: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%d", scenario->team_count);

    con_set_pos(50, 22);
    if (getMinSupportedScenarioVersion(BUILD_VERSION) > scenario->version) {
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_RED, L"Scenario outdated!");
        con_set_pos(50, 23);
        renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_RED, L"( < %hs )",
                           getVersionName(getMinSupportedScenarioVersion(BUILD_VERSION)));
    } else {
        renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"%30hs", "");
        con_set_pos(50, 23);
        renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"%30hs", "");
    }

    freeScenario(scenario);
}

MENU_ITEM_CALLBACK(onScenarioMenuSelect) {
    FILE * file = fopen(combinePath(SCENARIO_FOLDER, data), "rb");
    Scenario * scenario;
    Board * board;

    if (file != NULL) {
        scenario = loadScenario(file);
        fclose(file);
        if (scenario->version < getMinSupportedScenarioVersion(BUILD_VERSION)) {
            freeScenario(scenario);
            return false;
        }
        Exception exception = {};
        board = createBoard(scenario, settings, &exception);
        if (board == NULL && exception.status) {
            reportException(exception);
            freeScenario(scenario);
            return false;
        }
    } else {
        return false;
    }

    gameLoop(settings, board);
    freeBoard(board);
    freeScenario(scenario);

    return true;
}

MENU_ITEM_CALLBACK(onScenarioMenuLeave) {
    return true;
}