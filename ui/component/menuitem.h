#ifndef CHESS_MENUITEM_H
#define CHESS_MENUITEM_H

#include <wchar.h>

#include "abstract/defs.h"

#include "settings/settings.h"

#define MENU_ITEM_MAX_STRING_LEN 40
#define MENU_ITEM_CALLBACK(name) bool_t (name)(UserSettings * settings, char * data)

typedef struct MenuItem {
    UserSettings * settings;

    wchar_t text[MENU_ITEM_MAX_STRING_LEN];
    char text_data[MENU_ITEM_MAX_STRING_LEN];
    void * other_data;
    MENU_ITEM_CALLBACK(*callback);
} MenuItem;


/// Creates a new menu option
MenuItem * createMenuItem(UserSettings * settings, wchar_t * text, char * text_data, void * other_data, MENU_ITEM_CALLBACK(*callback));

/// Runs the menu option's callback
bool_t runMenuItem(MenuItem * menu_item);


/// Clears used memory
void freeMenuItem(MenuItem * menu_item);

#endif //CHESS_MENUITEM_H
