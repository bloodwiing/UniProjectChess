#ifndef CHESS_DEFS_H
#define CHESS_DEFS_H

#include <stdint.h>
#include <wchar.h>

// single byte boolean, because why is the built-in one 4 bytes...
typedef uint8_t bool_t;
#define true 1
#define false 0

char * bool2str(bool_t value);

typedef int8_t coord_t;
typedef uint8_t ucoord_t;

typedef uint8_t count_t;
typedef uint16_t count16_t;

typedef count_t team_index_t;
typedef count16_t spawn_index_t;
typedef count_t piece_index_t;
typedef count_t move_index_t;

typedef count_t special_extra_index_t;

#endif
