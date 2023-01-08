#ifndef CHESS_ENCODING_H
#define CHESS_ENCODING_H

#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <wchar.h>

/// Variable formatted wide-char string length
/// \param va Variable argument list
/// \return The calculated size of the string after formatting
int vfwslen(const wchar_t * format, va_list va);

typedef uint16_t wchar16_t;

/// Gets the length of the UTF-16 string
size_t strU16len(uint16_t * string);

/// Gets the length of the UTF-16 string if it were converted to UTF-32
size_t strU16lenAsU32(uint16_t * string);

/// Converts the UTF-16 string into a UTF-32 string
/// \param n The max amount of UTF-16 characters to read
/// \param size The allocated max size of the resulting UTF-32 string
/// \return The created UTF-32 string (make sure to free it!)
uint32_t * convertU16toU32(uint16_t * string, size_t n, size_t size);

/// Gets the length of the UTF-32 string
size_t strU32len(uint32_t * string);

/// Gets the length of the UTF-32 string if it were converted to UTF-16
size_t strU32lenAsU16(uint32_t * string);

/// Converts the UTF-32 string into a UTF-16 string
/// \param n The max amount of UTF-32 characters to read
/// \param size The allocated max size of the resulting UTF-16 string
/// \return The created UTF-16 string (make sure to free it!)
uint16_t * convertU32toU16(uint32_t * string, size_t n, size_t size);


/// A cross-platform function to convert a generic 16-bit string into a 16/32-bit wide string
/// \param n The max amount of 16-bit characters to read
/// \param size The allocated max size of the resulting wide string
/// \return The created wide string (make sure to free it!)
wchar_t * createWStr(wchar16_t * string, size_t n, size_t size);

/// A cross-platform function to convert a 16/32-bit wide string into a generic 16-bit string
/// \param n The max amount of wide characters to read
/// \param size The allocated max size of the resulting 16-bit string
/// \return The created 16-bit string (make sure to free it!)
wchar16_t * createU16(wchar_t * string, size_t n, size_t size);

#endif //CHESS_ENCODING_H
