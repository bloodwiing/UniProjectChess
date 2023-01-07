#ifndef CHESS_RECT_H
#define CHESS_RECT_H

#define RECT_3X3(x, y) createRect(x, y, 3, 3)
#define RECT_LINE(x, y, w) createRect(x, y, w, 1)

typedef struct Rect {
    int x, y, width, height;
} Rect;

Rect createRect(int x, int y, int width, int height);

Rect offsetRect(Rect rect, int x, int y, int width, int height);

Rect centerRectInRect(int inner_width, int inner_height, Rect outer);
Rect getScreenCenteredRect(int width, int height);

Rect fitRect(Rect rect, int bound_width, int bound_height);

Rect getScreenRect();

#endif //CHESS_RECT_H
