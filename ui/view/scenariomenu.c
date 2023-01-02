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

void updateScenarioMenu(UserSettings * settings, char * data);

void onScenarioMenuSelect(UserSettings * settings, char * data);
void onScenarioMenuLeave(UserSettings * settings, char * data);

void scenarioMenuLoop(UserSettings * settings) {
    con_clear();
    MenuSelector * selector = createMenuSelector(settings, updateScenarioMenu);

    size_t count = 0;
    char ** files = listDirectoryFiles(SCENARIO_FOLDER, &count);

    for (int i = 0; i < count; i++) {
        wchar_t name[MENU_ITEM_MAX_STRING_LEN];
        mbstowcs(name, files[i], MENU_ITEM_MAX_STRING_LEN);
        addMenuItem(selector, name, files[i], onScenarioMenuSelect);
    }
    addMenuItem(selector, L"Back", "", onScenarioMenuLeave);

    con_set_pos(2, 1);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Scenario select");

    while (updateMenuSelector(selector)) {
        displayMenuSelector(selector, 2, 3);
    }

    for (int i = 0; i < count; i++) {
        free(files + i);
    }
    free(files);
    free(selector);
}

void updateScenarioMenu(UserSettings * settings, char * data) {
    if (strlen(data) == 0) {
        clearRect(50, 2, 30, 17);
        return;
    }

    FILE * file = fopen(combinePath(SCENARIO_FOLDER, data), "rb");
    Scenario * scenario = loadScenario(file);
    fclose(file);
    renderScenario(scenario, settings, 50, 2, 0, 0, 30, 10);

    con_set_pos(50, 14);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GREEN, L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->name);
    con_set_pos(50, 15);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Author: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->author);
    con_set_pos(50, 17);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Size: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%d x %d", scenario->size_x, scenario->size_y);
    con_set_pos(50, 18);
    renderTextColoured(settings, COLOR_RESET, COLOR_DARK_GRAY, L"Teams: ");
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L"%d", scenario->team_count);

    free(scenario);
}

void onScenarioMenuSelect(UserSettings * settings, char * data) {
    FILE * file = fopen(combinePath(SCENARIO_FOLDER, data), "rb");
    Scenario * scenario;
    Board * board;

    if (file != NULL) {
        scenario = loadScenario(file);
        Exception exception = {};
        board = createBoard(scenario, settings, &exception);
        fclose(file);
        if (board == NULL && exception.status) {
            reportException(exception);
            return;
        }
    } else {
        scenarioMenuLoop(settings);
        return;
    }

    gameLoop(settings, board);
    free(scenario);
    free(board);

    scenarioMenuLoop(settings);
}

void onScenarioMenuLeave(UserSettings * settings, char * data) {
    mainMenuLoop(settings);
}