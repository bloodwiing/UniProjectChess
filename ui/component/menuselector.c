#include "menuselector.h"
#include <stdlib.h>
#include "../con_lib.h"

MenuSelector * createMenuSelector() {
    MenuSelector * out = calloc(1, sizeof(MenuSelector));
    return out;
}

void addMenuItem(MenuSelector * menu_selector, wchar_t * name, void (*callback)()) {
    menu_selector->items = realloc(menu_selector->items, sizeof(MenuItem) * ++(menu_selector->item_count));
    menu_selector->items[menu_selector->item_count - 1] = createMenuItem(name, callback);
    if (menu_selector->item_count == 1) menu_selector->items[0]->selected = true;
}

void displayMenuSelector(MenuSelector * menu_selector, int x, int y) {
    for (int i = 0; i < menu_selector->item_count; i++) {
        con_set_pos(x, y + i);
        MenuItem * item = menu_selector->items[i];
        wprintf(L"[ %c ] ", item->selected ? '*' : ' ');
        wprintf(item->text);
    }
}

bool_t updateMenuSelector(MenuSelector * menu_selector) {
    bool_t activated = false;
    int key;

    while (!activated) {
        while ((key = con_read_key())) {
            wprintf(L"%d\n", key);
        }
    }

    return false;
}
