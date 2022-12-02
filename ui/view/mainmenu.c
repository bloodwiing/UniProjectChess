#include "mainmenu.h"
#include "../component/menuselector.h"
#include "../con_lib.h"

void onStart();
void onExit();

void mainMenuLoop(UserSettings * settings) {
    MenuSelector * selector = createMenuSelector();
    addMenuItem(selector, L"Start", onStart);
    addMenuItem(selector, L"Exit", onExit);
    displayMenuSelector(selector, 2, 2);
    updateMenuSelector(selector);
}

void onStart() {
    con_set_pos(2, 10);
    wprintf(L"Hello!    \n");
}

void onExit() {
    con_set_pos(2, 10);
    wprintf(L"Goodbye!  \n");
}
