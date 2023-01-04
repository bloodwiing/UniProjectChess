#include "rect.h"

#include "ui/con_lib.h"

Rect createRect(int x, int y, int width, int height) {
    return (Rect){.x = x, .y = y, .width = width, .height = height};
}

Rect centerRectInRect(int inner_width, int inner_height, Rect outer) {
    int hor_pad = outer.width - inner_width,
        ver_pad = outer.height - inner_height;

    int left = hor_pad / 2 + outer.x,
        top = ver_pad / 2 + outer.y;

    return createRect(left, top, inner_width, inner_height);
}

Rect getCenteredRect(int width, int height) {
    ConSize size = con_get_size();
    return centerRectInRect(width, height, createRect(0, 0, size.width, size.height));
}
