#include "menuselector.h"

#include <stdlib.h>

#include "enum/key.h"

#include "ui/con_lib.h"
#include "ui/render.h"

MenuSelector * createMenuSelector(UserSettings * settings, MENU_SELECTOR_INIT_CALLBACK(*init_callback), MENU_SELECTOR_UPDATE_CALLBACK(*update_callback)) {
    MenuSelector * out = calloc(1, sizeof(MenuSelector));
    out->settings = settings;
    out->selected = -1;
    out->init_callback = init_callback;
    out->update_callback = update_callback;
    return out;
}

void addMenuItem(MenuSelector * menu_selector, wchar_t * name, char * data, MENU_ITEM_CALLBACK(*callback)) {
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

bool_t updateMenuSelector(MenuSelector * menu_selector, bool_t auto_free) {
    uint32_t key;

    while ((key = con_read_key())) {
        if (key == KEY_ENTER) {
            if (!runMenuItem(menu_selector->items[menu_selector->selected])) {
                menu_selector->init_callback(menu_selector->settings);
                runMenuSelectorUpdateCallback(menu_selector);
                continue;
            }
            if (auto_free)
                freeMenuSelector(menu_selector);
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

void freeMenuSelector(MenuSelector * menu_selector) {
    for (int i = 0; i < menu_selector->item_count;) {
        freeMenuItem(menu_selector->items[i++]);
    }
    free(menu_selector->items);
    free(menu_selector);
}
