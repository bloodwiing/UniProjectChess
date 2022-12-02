#ifndef CHESS_MENUSELECTOR_H
#define CHESS_MENUSELECTOR_H

#include "menuitem.h"
#include "../../model/defs.h"

typedef struct MenuSelector {
    MenuItem ** items;
    size_t item_count;
} MenuSelector;

MenuSelector * createMenuSelector();
void addMenuItem(MenuSelector * menu_selector, wchar_t * name, void (*callback)());
void displayMenuSelector(MenuSelector * menu_selector, int x, int y);
bool_t updateMenuSelector(MenuSelector * menu_selector);

#endif //CHESS_MENUSELECTOR_H
