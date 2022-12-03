#include <stdlib.h>
#include <string.h>
#include "scenariomenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "../../utils/files.h"
#include "mainmenu.h"
#include "../../model/scenario.h"
#include "../render.h"
#include "../board.h"

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
}

void updateScenarioMenu(UserSettings * settings, char * data) {
//    con_set_pos(2, 20);
//    wprintf(L"                                              ");
//
//    for (int i = 0; i < 20; ++i) {
//        con_set_pos(50, 2+i);
//        wprintf(L"                    ");
//    }

    if (strlen(data) == 0)
        return;

    con_set_pos(2, 20);
    renderTextColoured(settings, COLOR_RESET, COLOR_GREEN, L"Currently selected: %hs", data);

    FILE * file = fopen(combinePath(SCENARIO_FOLDER, data), "rb");
    Scenario * scenario = loadScenario(file);
    fclose(file);
    renderScenario(scenario, settings, 50, 2, 0, 0, 30, 10);
    free(scenario);
}

void onScenarioMenuSelect(UserSettings * settings, char * data) {
    con_set_pos(2, 20);
    wprintf(L"Selected: %hs\n", data);
}

void onScenarioMenuLeave(UserSettings * settings, char * data) {
    mainMenuLoop(settings);
}