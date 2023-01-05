#ifndef CHESS_RECT_H
#define CHESS_RECT_H

typedef struct Rect {
    int x, y, width, height;
} Rect;

Rect createRect(int x, int y, int width, int height);

Rect centerRectInRect(int inner_width, int inner_height, Rect outer);
Rect getScreenCenteredRect(int width, int height);

Rect fitRect(Rect rect, int bound_width, int bound_height);

#endif //CHESS_RECT_H
