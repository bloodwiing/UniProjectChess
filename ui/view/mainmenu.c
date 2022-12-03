#include "mainmenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "../render.h"
#include "scenariomenu.h"

void updateMainMenu(UserSettings * settings, char * data);

void onMainMenuResume(UserSettings * settings, char * data);
void onMainMenuStart(UserSettings * settings, char * data);
void onMainMenuExit(UserSettings * settings, char * data);

void mainMenuLoop(UserSettings * settings) {
    con_clear();
    MenuSelector * selector = createMenuSelector(settings, updateMainMenu);

    con_set_pos(2, 1);
    renderText(L"Rook's Gambit");

    addMenuItem(selector, L"Resume", "Let's get back into the fight", onMainMenuResume);
    addMenuItem(selector, L"New Scenario", "New day, new battle          ", onMainMenuStart);
    addMenuItem(selector, L"Quit", "Leaving already?             ", onMainMenuExit);

    while (updateMenuSelector(selector)) {
        displayMenuSelector(selector, 2, 3);
    }
}

void updateMainMenu(UserSettings * settings, char * data) {
    con_set_pos(5, 7);
    renderTextColoured(settings, COLOR_RESET, COLOR_GREEN, L"%hs", data);
}

void onMainMenuResume(UserSettings * settings, char * data) {
    con_set_pos(2, 10);
    wprintf(L"You're back!\n");
}

void onMainMenuStart(UserSettings * settings, char * data) {
    scenarioMenuLoop(settings);
}

void onMainMenuExit(UserSettings * settings, char * data) {
    con_clear();
    con_show_cursor(1);
}
