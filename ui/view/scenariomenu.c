#include <stdlib.h>
#include "scenariomenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "../../utils/files.h"
#include "mainmenu.h"

void onScenarioMenuSelect(UserSettings * settings, char * data);
void onScenarioMenuLeave(UserSettings * settings, char * data);

void scenarioMenuLoop(UserSettings * settings) {
    con_clear();
    MenuSelector * selector = createMenuSelector(settings);

    size_t count = 0;
    char ** files = listDirectoryFiles(".", &count);

    for (int i = 0; i < count; i++) {
        wchar_t name[MenuItemMaxStringLen];
        mbstowcs(name, files[i], MenuItemMaxStringLen);
        addMenuItem(selector, name, files[i], onScenarioMenuSelect);
    }
    addMenuItem(selector, L"Back", "", onScenarioMenuLeave);

    while (updateMenuSelector(selector)) {
        displayMenuSelector(selector, 2, 2);
    }
}

void onScenarioMenuSelect(UserSettings * settings, char * data) {
    con_set_pos(2, 20);
    wprintf(L"Selected: %hs\n", data);
}

void onScenarioMenuLeave(UserSettings * settings, char * data) {
    mainMenuLoop(settings);
}