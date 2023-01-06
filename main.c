#include <stdio.h>
#include <locale.h>

#include "defaults.h"

#include "engine/board.h"
#include "engine/moveentry.h"

#include "settings/settings.h"

#include "ui/con_lib.h"
#include "ui/component/responsive/responsive.h"
#include "ui/notation.h"
#include "ui/render.h"
#include "ui/shape.h"
#include "ui/view/mainmenu.h"

void setupConsole();

RESPONSIVE_CALLBACK(item1) {
    con_set_color(COLOR_RED, COLOR_RESET);
    clearRect(rect);
    drawDoubleBox((UserSettings *)data, rect, COLOR_LIGHT_RED, COLOR_BLACK);
}

RESPONSIVE_CALLBACK(item2) {
    con_set_color(COLOR_GREEN, COLOR_RESET);
    clearRect(rect);
    drawSingleBox((UserSettings *)data, rect, COLOR_LIGHT_GREEN, COLOR_BLACK);
}

RESPONSIVE_CALLBACK(item3) {
    con_set_color(COLOR_YELLOW, COLOR_RESET);
    clearRect(rect);
    drawSingleBox((UserSettings *)data, rect, COLOR_LIGHT_YELLOW, COLOR_BLACK);
}

int main() {
    setlocale(LC_ALL, "");
    setupConsole();

    UserSettings * settings = safeLoadUserSettings();

    con_show_echo(false);
    con_show_cursor(false);

    saveDefaultScenario();

    ResponsiveManager * manager = createResponsiveManager();

    ResponsiveHorizontalLayout * hor1 = createHorizontalLayout(1.0f);
    addHorizontalChild(hor1, createLayout(0.7f, settings, item1));

        ResponsiveVerticalLayout * ver1 = createVerticalLayout(0.3f);
        addVerticalChild(ver1, createLayout(0.5f, settings, item2));
        addVerticalChild(ver1, createLayout(0.5f, settings, item3));

    addHorizontalChild(hor1, compileVerticalLayout(ver1));

    addResponsiveBreakpoint(manager, createResponsiveBreakpoint(SIZE_MAX, 100, compileHorizontalLayout(hor1)));

    ResponsiveHorizontalLayout * hor2 = createHorizontalLayout(1.0f);
    addHorizontalChild(hor2, createLayout(0.4f, settings, item1));
    addHorizontalChild(hor2, createLayout(0.3f, settings, item2));
    addHorizontalChild(hor2, createLayout(0.3f, settings, item3));

    addResponsiveBreakpoint(manager, createResponsiveBreakpoint(SIZE_MAX, SIZE_MAX, compileHorizontalLayout(hor2)));

//    while (1) {
        renderResponsive(manager);
//    }

//    mainMenuLoop(settings);
    freeSettings(settings);

    con_show_echo(true);
    con_show_cursor(true);
}

#ifdef _WIN32
#include <io.h>

void setupConsole() {
    _setmode(_fileno(stdout), 0x00040000);
}
#else
void setupConsole() {}
#endif  // _WIN32
