#include "notation.h"

#include <string.h>

char * getFileNotation(uint8_t x) {
    char * result = calloc(3, sizeof(char));

    if (x > 26) {
        result[0] = (char)('a' + x / 26);
        result[1] = (char)('a' + x % 26);
    } else {
        result[0] = (char)('a' + x);
    }

    return result;
}

char * getRankNotation(uint8_t y) {
    char * result = calloc(4, sizeof(char));

    uint16_t new_y = y;
    ++new_y;

    int i = 0;
    if (new_y >= 10) i = 1;
    else if (new_y >= 100) i = 2;

    while (i >= 0) {
        result[i--] = (char)('0' + new_y % 10);
        new_y /= 10;
    }

    return result;
}

wchar_t * generateMoveNotation(UserSettings * settings, MoveEntry entry) {
    size_t alloc_size = 12 + PIECE_UNICODE_LENGTH + 2 + SPECIAL_MOVE_NOTATION_SIZE;  // Pab123xab123=P+ [phantom] - longest possible: 11 chars + 2 pieces + '\0' + phantom notation

    wchar_t * result = calloc(alloc_size, sizeof(wchar_t));
    memset(result, 0, alloc_size * sizeof(wchar_t));

    size_t bytes_written;

    if (entry.special_move != NULL && entry.special_move->data.has_custom_notation) {
        snwprintf(result, SPECIAL_MOVE_NOTATION_SIZE, L"%hs", entry.special_move->data.move_notation);
        if (entry.flags & MOVE_ENTRY_CHECKMATE)
            wcscat(result, L"#");
        else if (entry.flags & MOVE_ENTRY_CHECK)
            wcscat(result, L"+");
        return result;
    }

    if (entry.flags & MOVE_ENTRY_DRAW_OFFER) {
        wcscat(result, L"(=)");
        return result;
    }

    if (settings->notation_figurines)
        wcscat(result, entry.type->notation_unicode);
    else
        snwprintf(result, 2, L"%c", entry.type->notation);

    bytes_written = wcslen(result);

    bool_t needs_long_notation = false;

    if (entry.flags & MOVE_ENTRY_AMBIGUOUS_FILE || settings->notation == NotationType_LONG) {
        char * file = getFileNotation(entry.from_x);
        bytes_written += snwprintf(result + bytes_written, 3, L"%hs", file);
        free(file);
        needs_long_notation = true;
    }

    if (entry.flags & MOVE_ENTRY_AMBIGUOUS_RANK || settings->notation == NotationType_LONG) {
        char * rank = getRankNotation(entry.from_y);
        bytes_written += snwprintf(result + bytes_written, 4, L"%hs", rank);
        free(rank);
        needs_long_notation = true;
    }

    if (needs_long_notation)
        needs_long_notation = entry.from_x > 26 || entry.to_x > 26 || entry.from_y >= 9 || entry.to_y >= 9;

    ++bytes_written;
    if (entry.flags & MOVE_ENTRY_CAPTURE)
        wcscat(result, L"x");
    else if (settings->notation == NotationType_LONG || needs_long_notation)
        wcscat(result, L"-");
    else
        --bytes_written;

    char * file = getFileNotation(entry.to_x);
    bytes_written += snwprintf(result + bytes_written, 3, L"%hs", file);
    free(file);

    char * rank = getRankNotation(entry.to_y);
    bytes_written += snwprintf(result + bytes_written, 4, L"%hs", rank);
    free(rank);

    if (entry.flags & MOVE_ENTRY_PROMOTION) {
        if (settings->notation_figurines)
            bytes_written += snwprintf(result + bytes_written, 1 + PIECE_UNICODE_LENGTH, L"=%ls", entry.new_type->notation_unicode);
        else
            bytes_written += snwprintf(result + bytes_written, 3, L"=%hc", entry.new_type->notation);
    }

    ++bytes_written;
    if (entry.flags & MOVE_ENTRY_CHECKMATE)
        wcscat(result, L"#");
    else if (entry.flags & MOVE_ENTRY_CHECK)
        wcscat(result, L"+");
    else
        --bytes_written;

    if (entry.flags & MOVE_ENTRY_PHANTOM && entry.phantom_move != NULL)
        snwprintf(result + bytes_written, SPECIAL_MOVE_NOTATION_SIZE, L"%hs", entry.phantom_move->data.phantom_notation);

    return result;
}
