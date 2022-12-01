#ifndef CHESS_ENCODING_H
#define CHESS_ENCODING_H

#include <stdint.h>
#include <stddef.h>

typedef uint16_t wchar16_t;

size_t strU16len(uint16_t * string);
size_t strU16lenAsU32(uint16_t * string);
uint32_t * convertU16toU32(uint16_t * string, size_t size);

size_t strU32len(uint32_t * string);
size_t strU32lenAsU16(uint32_t * string);
uint16_t * convertU32toU16(uint32_t * string, size_t size);

wchar_t * createWStr(wchar16_t * string, size_t size);
wchar16_t * createU16(wchar_t * string, size_t size);

#endif //CHESS_ENCODING_H
