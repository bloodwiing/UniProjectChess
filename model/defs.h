#ifndef CHESS_DEFS_H
#define CHESS_DEFS_H

#include <stdint.h>
#include <wchar.h>

typedef uint8_t bool_t;
#define true 1
#define false 0

#ifdef _WIN32
typedef wchar_t wchar16_t;
#elif __LINUX__
typedef unsigned short wchar16_t;
#endif

#endif
