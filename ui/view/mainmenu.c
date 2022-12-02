#include "mainmenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"
#include "scenariomenu.h"

void onMainMenuResume(UserSettings * settings, char * data);
void onMainMenuStart(UserSettings * settings, char * data);
void onMainMenuExit(UserSettings * settings, char * data);

void mainMenuLoop(UserSettings * settings) {
    con_clear();
    MenuSelector * selector = createMenuSelector(settings);

    addMenuItem(selector, L"Resume", "", onMainMenuResume);
    addMenuItem(selector, L"New Scenario", "", onMainMenuStart);
    addMenuItem(selector, L"Quit", "", onMainMenuExit);

    while (updateMenuSelector(selector)) {
        displayMenuSelector(selector, 2, 2);
    }
}

void onMainMenuResume(UserSettings * settings, char * data) {
    con_set_pos(2, 10);
    wprintf(L"You're back!\n");
}

void onMainMenuStart(UserSettings * settings, char * data) {
    scenarioMenuLoop(settings);
}

void onMainMenuExit(UserSettings * settings, char * data) {
    con_set_pos(2, 10);
    wprintf(L"Goodbye!    \n");
}
