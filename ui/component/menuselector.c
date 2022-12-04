#include "menuselector.h"
#include <stdlib.h>
#include "../con_lib.h"
#include "../render.h"
#include "../../enum/key.h"

MenuSelector * createMenuSelector(UserSettings * settings, void (*update_callback)(UserSettings * settings, char * data)) {
    MenuSelector * out = calloc(1, sizeof(MenuSelector));
    out->settings = settings;
    out->selected = -1;
    out->update_callback = update_callback;
    return out;
}

void addMenuItem(MenuSelector * menu_selector, wchar_t * name, char * data, void (*callback)(UserSettings *, char *)) {
    menu_selector->items = realloc(menu_selector->items, sizeof(MenuItem) * ++(menu_selector->item_count));
    menu_selector->items[menu_selector->item_count - 1] = createMenuItem(menu_selector->settings, name, data, callback);
    if (menu_selector->item_count == 1) {
        menu_selector->selected = 0;
        runMenuSelectorUpdateCallback(menu_selector);
    }
}

void displayMenuSelector(MenuSelector * menu_selector, int x, int y) {
    for (int i = 0; i < menu_selector->item_count; i++) {
        con_set_pos(x, y + i);
        MenuItem * item = menu_selector->items[i];

        if (menu_selector->selected == i)
            renderTextColoured(menu_selector->settings, COLOR_RESET, COLOR_LIGHT_YELLOW, L">> %ls", item->text);
        else
            renderTextColoured(menu_selector->settings, COLOR_RESET, COLOR_LIGHT_GRAY, L"   %ls", item->text);
    }
}

bool_t updateMenuSelector(MenuSelector * menu_selector) {
    uint32_t key;

    while ((key = con_read_key())) {
        if (key == KEY_ENTER) {
            runMenuItem(menu_selector->items[menu_selector->selected]);
            return false;
        }

        switch (key) {
            case KEY_W:
            case KEY_ARROW_UP:
                if (menu_selector->selected-- == 0) menu_selector->selected = menu_selector->item_count - 1;
                runMenuSelectorUpdateCallback(menu_selector);
                break;
            case KEY_S:
            case KEY_ARROW_DOWN:
                if (menu_selector->selected++ == menu_selector->item_count - 1) menu_selector->selected = 0;
                runMenuSelectorUpdateCallback(menu_selector);
                break;
            default:
                break;
        }
    }

    return true;
}

void runMenuSelectorUpdateCallback(MenuSelector * menu_selector) {
    menu_selector->update_callback(menu_selector->settings, menu_selector->items[menu_selector->selected]->data);
}
