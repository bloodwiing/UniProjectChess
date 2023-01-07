#include "encoding.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Solution by https://stackoverflow.com/questions/36280521/how-to-format-wide-char-string-via-vswprintf-on-osx-want-to-return-stdwstring/36282347#36282347
int vfwslen(const wchar_t * format, va_list va) {
#ifdef _WIN32
    FILE * null_file = fopen("NUL", "wb");
#else
    FILE * null_file = fopen("/dev/null", "wb");
#endif
    if (null_file == NULL)
        return -1;
    int result = vfwprintf(null_file, format, va);
    fclose(null_file);
    return result;
}

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
    uint32_t * out = calloc(size, sizeof(uint32_t));
    uint32_t * cursor = out;

    size_t str_len = strU16lenAsU32(string);
    if (str_len > size - 1)
        str_len = size - 1;

    const uint16_t * end = string + str_len;
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
    uint16_t * out = calloc(size, sizeof(uint16_t));
    uint16_t * cursor = out;

    size_t str_len = strU32lenAsU16(string);
    if (str_len > size - 1)
        str_len = size - 1;

    const uint32_t * end = string + str_len;
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

#ifdef __SIZEOF_WCHAR_T__
#if __SIZEOF_WCHAR_T__ == 4
wchar_t * createWStr(uint16_t * string, size_t size) {
    return (wchar_t *)convertU16toU32(string, size);
}

uint16_t * createU16(wchar_t * string, size_t size) {
    return convertU32toU16((uint32_t *)string, size);
}
#elif __SIZEOF_WCHAR_T__ == 2
wchar_t * createWStr(wchar16_t * string, size_t size) {
    wchar_t * temp = calloc(size, sizeof(wchar_t));
    memcpy(temp, string, size - 1);
    return temp;
}

wchar16_t * createU16(wchar_t * string, size_t size) {
    wchar16_t * temp = calloc(size, sizeof(wchar16_t));
    memcpy(temp, string, size - 1);
    return temp;
}
#endif // __SIZEOF_WCHAR_T__
#endif // defined(__SIZEOF_WCHAR_T__)
