#ifndef CHESS_MENUSELECTOR_H
#define CHESS_MENUSELECTOR_H

#include "./menuitem.h"

#include "abstract/defs.h"

#include "settings/settings.h"

#include "ui/generic/rect.h"

#define MENU_SELECTOR_INIT_CALLBACK(name) void (name)(UserSettings * settings)
#define MENU_SELECTOR_UPDATE_CALLBACK(name) void (name)(UserSettings * settings, char * text_data, void * other_data)

typedef struct MenuSelector {
    UserSettings * settings;

    MenuItem ** items;
    size_t item_count;

    size_t selected;

    MENU_SELECTOR_INIT_CALLBACK(*init_callback);
    MENU_SELECTOR_UPDATE_CALLBACK(*update_callback);
} MenuSelector;


/// Creates a new dynamic menu
MenuSelector * createMenuSelector(UserSettings * settings, MENU_SELECTOR_INIT_CALLBACK(*init_callback), MENU_SELECTOR_UPDATE_CALLBACK(*update_callback));

/// Adds a new dynamic menu option
void addMenuItem(MenuSelector * menu_selector, wchar_t * name, char * text_data, void * other_data, MENU_ITEM_CALLBACK(*callback));

/// Renders the dynamic menu to the given rect of the screen
void displayMenuSelector(MenuSelector * menu_selector, Rect rect);


/// Returns the selected option's text data
char * getSelectedTextData(MenuSelector * menu_selector);

/// Returns the selected option's custom data
void * getSelectedOtherData(MenuSelector * menu_selector);

/// Runs the dynamic menu's init callback (called every time when the screen needs a refresh)
void runMenuSelectorInitCallback(MenuSelector * menu_selector);

/// Runs the dynamic menu's update callback (called every time when the menu receives an update)
void runMenuSelectorUpdateCallback(MenuSelector * menu_selector);


/// Use within a loop to test and update the menu while an option didn't flag it to return
bool_t updateMenuSelector(MenuSelector * menu_selector, bool_t auto_free);


/// Clears used memory
void freeMenuSelector(MenuSelector * menu_selector);

#endif //CHESS_MENUSELECTOR_H
