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

MenuSelector * createMenuSelector(UserSettings * settings, MENU_SELECTOR_INIT_CALLBACK(*init_callback), MENU_SELECTOR_UPDATE_CALLBACK(*update_callback));

void addMenuItem(MenuSelector * menu_selector, wchar_t * name, char * text_data, void * other_data, MENU_ITEM_CALLBACK(*callback));

void displayMenuSelector(MenuSelector * menu_selector, Rect rect);

char * getSelectedTextData(MenuSelector * menu_selector);
void * getSelectedOtherData(MenuSelector * menu_selector);
void runMenuSelectorInitCallback(MenuSelector * menu_selector);
void runMenuSelectorUpdateCallback(MenuSelector * menu_selector);

bool_t updateMenuSelector(MenuSelector * menu_selector, bool_t auto_free);

void freeMenuSelector(MenuSelector * menu_selector);

#endif //CHESS_MENUSELECTOR_H
