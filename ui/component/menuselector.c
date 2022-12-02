#include "menuselector.h"
#include <stdlib.h>
#include "../con_lib.h"
#include "../render.h"
#include "../../enum/key.h"

MenuSelector * createMenuSelector(UserSettings * settings) {
    MenuSelector * out = calloc(1, sizeof(MenuSelector));
    out->settings = settings;
    out->selected = -1;
    return out;
}

void addMenuItem(MenuSelector * menu_selector, wchar_t * name, void (*callback)()) {
    menu_selector->items = realloc(menu_selector->items, sizeof(MenuItem) * ++(menu_selector->item_count));
    menu_selector->items[menu_selector->item_count - 1] = createMenuItem(name, callback);
    if (menu_selector->item_count == 1)
        menu_selector->selected = 0;
}

void displayMenuSelector(MenuSelector * menu_selector, int x, int y) {
    for (int i = 0; i < menu_selector->item_count; i++) {
        con_set_pos(x, y + i);
        MenuItem * item = menu_selector->items[i];

        if (menu_selector->selected == i)
            renderTextColoured(menu_selector->settings, COLOR_RESET, COLOR_YELLOW, L">> %s", item->text);
        else
            renderTextColoured(menu_selector->settings, COLOR_RESET, COLOR_GRAY, L"   %s", item->text);
    }
}

bool_t updateMenuSelector(MenuSelector * menu_selector) {
    uint32_t key;

    while ((key = con_read_key())) {
        if (key == KEY_ENTER) {
            runMenuItem(menu_selector->items[menu_selector->selected]);
            return false;
        }

        if (menu_selector->settings->inputs == InputType_WASD) {
            switch (key) {
                case (KEY_W):
                case (KEY_ARROW_UP):
                    if (menu_selector->selected-- == 0) menu_selector->selected = menu_selector->item_count - 1;
                    break;
                case (KEY_S):
                case (KEY_ARROW_DOWN):
                    if (menu_selector->selected++ == menu_selector->item_count - 1) menu_selector->selected = 0;
                    break;
                default:
                    break;
            }
        }
    }

    return true;
}
