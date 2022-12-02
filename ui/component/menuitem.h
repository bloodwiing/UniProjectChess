#ifndef CHESS_MENUITEM_H
#define CHESS_MENUITEM_H

#include <wchar.h>
#include "../../model/defs.h"

typedef struct MenuItem {
    wchar_t text[30];
    void (*callback)();
    bool_t selected;
} MenuItem;

MenuItem * createMenuItem(wchar_t * text, void (*callback)());
void run(MenuItem * menu_item);

#endif //CHESS_MENUITEM_H
