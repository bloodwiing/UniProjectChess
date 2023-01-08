#include "scenariomenu.h"

#include <stdlib.h>
#include <string.h>

#include "./gamemenu.h"

#include "ui/component/menuselector.h"
#include "ui/component/responsive/responsive.h"
#include "ui/render.h"

#include "utils/files.h"

#define LOG_MODULE L"ScenarioMenu"

MENU_SELECTOR_INIT_CALLBACK(initScenarioMenu);
MENU_SELECTOR_UPDATE_CALLBACK(updateScenarioMenu);

MENU_ITEM_CALLBACK(onScenarioMenuSelect);
MENU_ITEM_CALLBACK(onScenarioMenuLeave);


// RESPONSIVE PROCESSING
typedef struct ScenarioResponsiveProcessingResult {
    Exception exception;
    Scenario * scenario;
    MenuSelector * selector;
    ResponsiveManager * responsive;
} ScenarioResponsiveProcessingResult;

ScenarioResponsiveProcessingResult * scenarioResponsiveProcessingResultUpdate(ScenarioResponsiveProcessingResult * result) {
    freeScenario(result->scenario);
    result->scenario = NULL;

    clearException(&result->exception);

    char * selected = getSelectedTextData(result->selector);

    if (strlen(selected) == 0)
        return NULL;

    char * path = combinePath(SCENARIO_FOLDER, selected);
    FILE * file = fopen(path, "rb");

    result->scenario = loadScenario(file, true, &result->exception);

    fclose(file);
    free(path);

    return result;
}


// RESPONSIVE LAYOUTS
RESPONSIVE_CALLBACK(bp_scenarioCallback_List) {
    ScenarioResponsiveProcessingResult * result = data;
    renderTextColouredWrappedRect(result->selector->settings, COLOR_RESET, COLOR_DARK_GRAY, RECT_LINE(2, 1, rect.width - 4), L"Scenario select");
    displayMenuSelector(result->selector, offsetRect(rect, 2, 3, -4, -4));
}

RESPONSIVE_CALLBACK(bp_scenarioCallback_Preview) {
    logInfo(settings, LOG_MODULE, L"Rendering scenario PREVIEW...");

    ScenarioResponsiveProcessingResult * result = data;
    Scenario * scenario = result->scenario;

    if (result->exception.status) {
        logError(settings, LOG_MODULE, L"Failed to load scenario: %hs", result->exception.message);
        clearRect(rect);
        reportExceptionAtPos(result->selector->settings, result->exception, offsetRect(rect, 2, 2, -2, -2));
        return;
    }

    if (scenario == NULL) {
        logDebug(settings, LOG_MODULE, L"scenario=NULL");
        clearRect(rect);
        return;
    }

    Rect board_rect = getScenarioCenteredRect(scenario, rect, 0, 0);
    renderScenario(scenario, result->selector->settings, rect, board_rect, false, true);
    drawSingleBoxContained(result->selector->settings, rect, COLOR_RESET, COLOR_YELLOW);

    con_flush();
}

RESPONSIVE_CALLBACK(bp_scenarioCallback_Info) {
    logInfo(settings, LOG_MODULE, L"Rendering scenario INFO...");

    ScenarioResponsiveProcessingResult * result = data;
    Scenario * scenario = result->scenario;

    Rect original = rect;

    rect.y += 2;
    rect.height -= 2;

    if (scenario == NULL) {
        logDebug(settings, LOG_MODULE, L"scenario=NULL");
        clearRect(rect);
        return;
    }

    if (getMinSupportedScenarioVersion(BUILD_VERSION) > scenario->version) {
        renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GRAY, RECT_LINE(rect.x, rect.y++, rect.width), L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->name);
        renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_RED, RECT_LINE(rect.x, rect.y++, rect.width), L"[ %-*hs ]", SCENARIO_MAX_STRING_LEN, getVersionName(scenario->version));
    } else {
        renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GREEN, RECT_LINE(rect.x, rect.y++, rect.width), L"%-*hs", SCENARIO_MAX_STRING_LEN, scenario->name);
        renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_DARK_GRAY, RECT_LINE(rect.x, rect.y++, rect.width), L"[ %-*hs ]", SCENARIO_MAX_STRING_LEN, getVersionName(scenario->version));
    }

    ++rect.y;
    con_set_pos(rect.x, rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Author: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, RECT_LINE(rect.x + 8, rect.y++, rect.width - 8), L"@%-*hs", SCENARIO_MAX_STRING_LEN, scenario->author);

    con_set_pos(rect.x, rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Size: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, RECT_LINE(rect.x + 6, rect.y++, rect.width - 6), L"%d x %d", scenario->size_x, scenario->size_y);

    con_set_pos(rect.x, rect.y);
    renderTextColoured(settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"Teams: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_YELLOW, RECT_LINE(rect.x + 7, rect.y++, rect.width - 7), L"%d", scenario->team_count);

    ++rect.y;

    Rect remaining = createRect(rect.x, rect.y, rect.width, rect.height - (rect.y - original.y));

    if (getMinSupportedScenarioVersion(BUILD_VERSION) > scenario->version)
        renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_RED, remaining, L"Scenario outdated! ( < %hs )",
                                      getVersionName(getMinSupportedScenarioVersion(BUILD_VERSION)));
    else
        clearRect(remaining);
}


// RESPONSIVE BREAKPOINT
// Only the scenario list
ResponsiveBreakpoint createScenarioListBreakpoint(ScenarioResponsiveProcessingResult * result) {
    ResponsiveLayout layout = createLayout(1.0f, result, bp_scenarioCallback_List, NULL);
    return createResponsiveBreakpoint(12, 50, layout, ResponsiveBreakpointType_OR);
}

// RESPONSIVE BREAKPOINT
// Show both 50/50 without map
ResponsiveBreakpoint createScenarioFullBreakpoint(ScenarioResponsiveProcessingResult * result) {
    ResponsiveHorizontalLayout * hor = createHorizontalLayout(1.0f);
    addHorizontalChild(hor, createLayout(0.4f, result, bp_scenarioCallback_List, NULL));
    addHorizontalChild(hor, createLayout(0.6f, result, bp_scenarioCallback_Info, NULL));

    return createResponsiveBreakpoint(25, SIZE_MAX, compileHorizontalLayout(hor), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Show both 50/50 with map
ResponsiveBreakpoint createScenarioMapFullBreakpoint(ScenarioResponsiveProcessingResult * result) {
    ResponsiveHorizontalLayout * hor = createHorizontalLayout(1.0f);
    addHorizontalChild(hor, createLayout(0.4f, result, bp_scenarioCallback_List, NULL));

        ResponsiveVerticalLayout * ver = createVerticalLayout(0.6f);
        addVerticalChild(ver, createLayout(0.45f, result, bp_scenarioCallback_Preview, NULL));
        addVerticalChild(ver, createLayout(0.55f, result, bp_scenarioCallback_Info, NULL));

    addHorizontalChild(hor, compileVerticalLayout(ver));

    return createResponsiveBreakpoint(SIZE_MAX, SIZE_MAX, compileHorizontalLayout(hor), ResponsiveBreakpointType_AND);
}


// MENU NAVIGATION
void scenarioMenuLoop(UserSettings * settings) {
    logInfo(settings, LOG_MODULE, L"Running scenario menu...");

    initScenarioMenu(settings);

    // Responsive layout
    ResponsiveManager * responsive = createResponsiveManager();

    MenuSelector * selector = createMenuSelector(settings, initScenarioMenu, updateScenarioMenu);

    ScenarioResponsiveProcessingResult * result = calloc(1, sizeof(ScenarioResponsiveProcessingResult));
    result->responsive = responsive;
    result->selector = selector;

    addResponsiveBreakpoint(responsive, createScenarioListBreakpoint(result));
    addResponsiveBreakpoint(responsive, createScenarioFullBreakpoint(result));
    addResponsiveBreakpoint(responsive, createScenarioMapFullBreakpoint(result));

    logInfo(settings, LOG_MODULE, L"Listing files...");

    size_t count = 0;
    char ** files = listDirectoryFiles(SCENARIO_FOLDER, &count);

    logInfo(settings, LOG_MODULE, L"Found files: %d", count);

    for (int i = 0; i < count; i++) {
        wchar_t name[MENU_ITEM_MAX_STRING_LEN];
        mbstowcs(name, files[i], MENU_ITEM_MAX_STRING_LEN);
        addMenuItem(selector, name, files[i], result, onScenarioMenuSelect);
    }
    addMenuItem(selector, L"Back", "", result, onScenarioMenuLeave);

    scenarioResponsiveProcessingResultUpdate(result);
    renderResponsive(settings, responsive);
    while (updateMenuSelector(selector, true));

    freeResponsiveBreakpoint(responsive);

    freeScenario(result->scenario);
    free(result);

    for (int i = 0; i < count;)
        free(files[i++]);
    free(files);
}

MENU_SELECTOR_INIT_CALLBACK(initScenarioMenu) {
    con_clear();
}

MENU_SELECTOR_UPDATE_CALLBACK(updateScenarioMenu) {
    ScenarioResponsiveProcessingResult * result = other_data;
    scenarioResponsiveProcessingResultUpdate(result);
    renderResponsive(settings, result->responsive);
    con_flush();
}

MENU_ITEM_CALLBACK(onScenarioMenuSelect) {
    logInfo(settings, LOG_MODULE, L"Starting game...");

    char * path = combinePath(SCENARIO_FOLDER, data);
    FILE * file = fopen(path, "rb");

    Scenario * scenario;
    Board * board;

    if (file != NULL) {
        Exception exception = {};
        scenario = loadScenario(file, true, &exception);
        fclose(file);
        free(path);
        if (scenario == NULL && exception.status) {
            logError(settings, LOG_MODULE, L"Failed to load scenario: %hs", exception.message);
            reportException(settings, exception);
            return false;
        }
        if (scenario->version < getMinSupportedScenarioVersion(BUILD_VERSION)) {
            logWarning(settings, LOG_MODULE, L"Scenario outdated! (scenario)%hs < (supported)%hs", getVersionName(scenario->version),
                       getVersionName(getMinSupportedScenarioVersion(BUILD_VERSION)));
            freeScenario(scenario);
            return false;
        }

        clearException(&exception);
        board = createBoard(scenario, settings, &exception);
        if (board == NULL && exception.status) {
            logError(settings, LOG_MODULE, L"Failed to create board: %hs", exception.message);
            reportException(settings, exception);
            freeScenario(scenario);
            return false;
        }
    }
    else {
        logWarning(settings, LOG_MODULE, L"File not found: %hs", path);
        free(path);
        return false;
    }

    beginNewGameLoop(settings, board, true);
    freeBoard(board, true);

    return true;
}

MENU_ITEM_CALLBACK(onScenarioMenuLeave) {
    logInfo(settings, LOG_MODULE, L"Leaving scenario menu...");
    return true;
}