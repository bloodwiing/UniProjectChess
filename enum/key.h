#ifndef CHESS_KEY_H
#define CHESS_KEY_H

#ifdef _WIN32

#define KEY_ENTER 13

#define KEY_ARROW_UP 38
#define KEY_ARROW_DOWN 40
#define KEY_ARROW_LEFT 37
#define KEY_ARROW_RIGHT 49

#define KEY_W 87
#define KEY_A 65
#define KEY_S 83
#define KEY_D 68

#elif __linux__

#define KEY_ENTER 10

#define KEY_ARROW_UP 1792865
#define KEY_ARROW_DOWN 1792866
#define KEY_ARROW_LEFT 1792868
#define KEY_ARROW_RIGHT 1792867

#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100

#endif

#endif //CHESS_KEY_H
