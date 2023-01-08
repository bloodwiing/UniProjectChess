#include "test.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "model/vector.h"

#include "ui/generic/rect.h"

#include "utils/encoding.h"

void testVectors();
void testRect();
void testEncoding();

void runTests() {
    testVectors();
    testRect();
    testEncoding();
    wprintf(L"All tests passed\n");
}

void testVectors() {
    wprintf(L"Test 1: Vectors - ");

    assert(createVector(1, 2).x == 1);
    assert(createVector(2, -5).y == -5);
    assert(VECTOR_DOWN.y == 1);
    assert(VECTOR_LEFT.x == -1);
    assert(normaliseVector(createVector(2, 3), TeamDirectionRight).x == -3);
    assert(normaliseVector(createVector(5, 0), TeamDirectionDown).x == -5);

    assert(compVectors(VECTOR_UP, VECTOR_UP));
    assert(compVectors(createVector(1, 0), VECTOR_RIGHT));

    assert(compVectors(createVector(7, -3), normaliseVector(normaliseVector(createVector(7, -3), TeamDirectionDown), TeamDirectionDown)));
    assert(compVectors(normaliseVector(createVector(7, -3), TeamDirectionDown), normaliseVector(normaliseVector(createVector(7, -3), TeamDirectionRight), TeamDirectionRight)));

    wprintf(L"Passed\n");
}

void testRect() {
    wprintf(L"Test 2: Rect - ");

    assert(createRect(0, 1, 2, 3).x == 0);
    assert(createRect(5, -3, 8, 7).y == -3);
    assert(createRect(-10, 9, -6, 4).width == -6);
    assert(createRect(INT_MAX, -INT_MAX, -INT_MAX, INT_MAX).height == INT_MAX);

    assert(RECT_3X3(1, 2).width == 3);
    assert(RECT_3X3(8, 4).height == 3);

    assert(RECT_LINE(-2, 5, -5).height == 1);

    assert(offsetRect(RECT_3X3(1, 2), 2, 1, -1, -2).height == 1);
    assert(offsetRect(RECT_LINE(7, 5, -6), 7, 6, 9, 0).width == 3);
    assert(insetRect(RECT_3X3(5, 5), 1).x == 6);
    assert(insetRect(RECT_3X3(-2, -3), -3).width == 9);

    wprintf(L"Passed\n");
}

void testEncoding() {
    wprintf(L"Test 3: Unicode Encoding - ");
    {
        uint16_t u16[] = {55358, 56714, 55357, 56374, 84, 101, 115, 116, 55357, 56615, 55357, 56448, 10024, 65, 66, 67,
                          97, 98, 99, 10084, 65039, 1641, 40, 65290, 9685, 8255, 9685, 65290, 41, 1782, 0};
        uint32_t u32[] = {129418, 128054, 84, 101, 115, 116, 128295, 128128, 10024, 65, 66, 67, 97, 98, 99, 10084,
                          65039, 1641, 40, 65290, 9685, 8255, 9685, 65290, 41, 1782, 0};

        uint32_t *u16to32 = convertU16toU32(u16, sizeof(u16) / sizeof(uint16_t), sizeof(u32) / sizeof(uint32_t));
        assert(memcmp(u16to32, u32, sizeof(u32) / sizeof(uint32_t)) == 0);

        uint16_t *u16to32to16 = convertU32toU16(u16to32, sizeof(u32) / sizeof(uint32_t),
                                                sizeof(u16) / sizeof(uint16_t));
        assert(memcmp(u16to32to16, u16, sizeof(u16) / sizeof(uint16_t)) == 0);

        free(u16to32);
        free(u16to32to16);
    }
    wprintf(L"Passed\n");


    wprintf(L"Test 4: WChar standardisation - ");
    {
        wchar16_t string[] = {55357, 56576, 32, 83, 117, 114, 112, 114, 105, 115, 101, 32, 109, 101, 32, 55358, 56793,
                              8205, 9792, 65039, 32, 55356, 57264, 32, 55356, 57265, 32, 55356, 57266, 32, 55357, 56622,
                              32, 10024, 0};
        wchar_t *converted = createWStr(string, sizeof(string) / sizeof(uint16_t), 35);
        assert(wcscmp(converted, L"\U0001F500 Surprise me \U0001F9D9\u200D♀️ \U0001F3B0 \U0001F3B1 \U0001F3B2 \U0001F52E ✨") == 0);
        free(converted);
    }
    {
        wchar16_t string[] = {55356, 57289, 115, 55357, 56540, 101, 55357, 56582, 101, 9888, 65039, 107, 55357, 56577, 0};
        wchar_t *converted = createWStr(string, sizeof(string) / sizeof(uint16_t), 15);
        assert(wcscmp(converted, L"\U0001f3c9s\U0001f4dce\U0001f506e\U000026a0\U0000fe0fk\U0001f501") == 0);
        free(converted);
    }
    {
        wchar_t string[] = L"\U0001f17f\U0000fe0fn\U00002b50\U0000fe0fe\U0001f36fv\U0001f505e\U0001f477r\U0001f4f8g\U0001f311o\U0001f335n\U0001f6af";
        wchar16_t *converted = createU16(string, sizeof(string) / sizeof(wchar_t), 28);
        wchar16_t expected[] = { 55356, 56703, 65039, 110, 11088, 65039, 101, 55356, 57199, 118, 55357, 56581, 101, 55357, 56439, 114, 55357, 56568, 103, 55356, 57105, 111, 55356, 57141, 110, 55357, 57007, 0 };
        assert(memcmp(converted, expected, 28) == 0);
        free(converted);
    }

    wprintf(L"Passed\n");
}
