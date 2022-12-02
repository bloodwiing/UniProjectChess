#include "mainmenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"

void onResume();
void onStart();
void onExit();

void mainMenuLoop(UserSettings * settings) {
    MenuSelector * selector = createMenuSelector(settings);

    addMenuItem(selector, L"Resume", onResume);
    addMenuItem(selector, L"Start", onStart);
    addMenuItem(selector, L"Exit", onExit);

    while (updateMenuSelector(selector)) {
        displayMenuSelector(selector, 2, 2);
    }
}

void onResume() {
    con_set_pos(2, 10);
    wprintf(L"You're back!\n");
}

void onStart() {
    con_set_pos(2, 10);
    wprintf(L"Hello!      \n");
}

void onExit() {
    con_set_pos(2, 10);
    wprintf(L"Goodbye!    \n");
}
