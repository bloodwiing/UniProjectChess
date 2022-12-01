#include "encoding.h"
#include <stdlib.h>

size_t strU16len(uint16_t * string) {
    size_t size = 0;
    while (*string++ != 0)
        ++size;
    return size;
}

size_t strU16lenAsU32(uint16_t * string) {
    size_t size = 0;
    while (*string != 0) {
        if (*string >= 0xD800 && *string < 0xDC00)  // high surrogate
            ++string;  // skip low surrogate
        ++size;
        ++string;
    }
    return size;
}

uint32_t * convertU16toU32(uint16_t * string, size_t size) {
    uint32_t * out = calloc(strU16lenAsU32(string) + 1, sizeof(uint32_t));
    uint32_t * cursor = out;

    const uint16_t * end = string + size;
    while (string < end) {
        if (*string >= 0xDC00 && *string < 0xE000) {  // low surrogate
            *cursor++ |= (uint32_t)((*string++ - 0xDC00) & 0x3FF) + 0x10000;
        } else if (*string >= 0xD800) {  // high surrogate
            *cursor |= (*string++ - 0xD800) << 10;
        } else {  // others
            *cursor++ = *string++;
        }
    }
    *cursor = 0;

    return out;
}

size_t strU32len(uint32_t * string) {
    size_t size = 0;
    while (*string++ != 0)
        ++size;
    return size;
}

size_t strU32lenAsU16(uint32_t * string) {
    size_t size = 0;
    while (*string != 0) {
        if (*string++ > 0x10000)
            ++size;
        ++size;
    }
    return size;
}

uint16_t * convertU32toU16(uint32_t * string, size_t size) {
    uint16_t * out = calloc(strU32lenAsU16(string) + 1, sizeof(uint16_t));
    uint16_t * cursor = out;

    const uint32_t * end = string + size;
    while (string < end) {
        if (*string >= 0x10000) {  // surrogate pair
            uint32_t pair = (*string++ - 0x10000) & 0xFFFF;
            *cursor++ |= (pair >> 10) + 0xD800;  // high surrogate
            *cursor++ |= (pair & 0x3FF) + 0xDC00;  // low surrogate
        } else {  // others
            *cursor++ = *string++;
        }
    }
    *cursor = 0;

    return out;
}

#if defined(__SIZEOF_WCHAR_T__) && __SIZEOF_WCHAR_T__ == 4
wchar_t * createWStr(uint16_t * string, size_t size) {
    return (wchar_t *)convertU16toU32(string, size);
}

uint16_t * createU16(wchar_t * string, size_t size) {
    return convertU32toU16((uint32_t *)string, size);
}
#else
wchar_t * createWStr(uint16_t * string, size_t size) {
    return (wchar_t *)string;
}

uint16_t * createU16(wchar_t * string, size_t size) {
    return (uint16_t *)string;
}
#endif
