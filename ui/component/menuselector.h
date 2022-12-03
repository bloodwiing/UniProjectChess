#ifndef CHESS_MENUSELECTOR_H
#define CHESS_MENUSELECTOR_H

#include "menuitem.h"
#include "../../settings/settings.h"
#include "../../model/defs.h"

typedef struct MenuSelector {
    UserSettings * settings;

    MenuItem ** items;
    size_t item_count;

    size_t selected;

    void (*update_callback)(UserSettings * settings, char * data);
} MenuSelector;

MenuSelector * createMenuSelector(UserSettings * settings, void (*update_callback)(UserSettings * settings, char * data));
void addMenuItem(MenuSelector * menu_selector, wchar_t * name, char * data, void (*callback)(UserSettings *, char *));
void displayMenuSelector(MenuSelector * menu_selector, int x, int y);
bool_t updateMenuSelector(MenuSelector * menu_selector);
void runMenuSelectorUpdateCallback(MenuSelector * menu_selector);

#endif //CHESS_MENUSELECTOR_H
