#include "rect.h"

#include "ui/con_lib.h"

Rect createRect(int x, int y, int width, int height) {
    return (Rect){.x = x, .y = y, .width = width, .height = height};
}

Rect offsetRect(Rect rect, int x, int y, int width, int height) {
    return createRect(rect.x + x, rect.y + y, rect.width + width, rect.height + height);
}

Rect centerRectInRect(int inner_width, int inner_height, Rect outer) {
    int hor_pad = outer.width - inner_width,
        ver_pad = outer.height - inner_height;

    int left = hor_pad / 2 + outer.x,
        top = ver_pad / 2 + outer.y;

    return createRect(left > 0 ? left : 0, top > 0 ? top : 0, inner_width, inner_height);
}

Rect getScreenCenteredRect(int width, int height) {
    ConSize size = con_get_size();
    return centerRectInRect(width, height, createRect(0, 0, size.width, size.height));
}

Rect fitRect(Rect rect, int bound_width, int bound_height) {
    Rect out = createRect(
        rect.x > 0 ? rect.x : 0,
        rect.y > 0 ? rect.y : 0,
        rect.width > bound_width ? bound_width : rect.width,
        rect.height > bound_height ? bound_height : rect.height
    );
    if (rect.x + out.width > rect.width)
        out.x = rect.width - out.width;
    if (rect.y + out.height > rect.height)
        out.y = rect.height - out.height;
    return out;
}
