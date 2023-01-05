#ifndef CHESS_SPECIALDATA_H
#define CHESS_SPECIALDATA_H

#include <stdio.h>

#include "abstract/defs.h"

#include "model/teamdirection.h"
#include "model/vector8.h"

#define SPECIAL_MOVE_NOTATION_SIZE 10

typedef struct SpecialData {
    bool_t normalised;

    Vector8 vector;
    bool_t is_first_move;
    bool_t is_check_safe;

    count_t condition_count;
    Vector8 * conditions;

    bool_t is_phantom;
    Vector8 phantom;
    char phantom_notation[SPECIAL_MOVE_NOTATION_SIZE];

    bool_t has_custom_notation;
    char move_notation[SPECIAL_MOVE_NOTATION_SIZE];
} SpecialData;

SpecialData createSpecialData(Vector8 vector, bool_t is_first_move, bool_t is_check_safe, bool_t normalised);
SpecialData createSpecialDataRaw(coord_t x, coord_t y, bool_t is_first_move, bool_t is_check_safe, bool_t normalised);

void saveSpecialData(SpecialData special_data, FILE * stream);
SpecialData loadSpecialData(FILE * stream);
void printSpecialData(SpecialData special_data);

void addSpecialDataCondition(SpecialData * special_data, Vector8 condition);
void addSpecialDataConditionRaw(SpecialData * special_data, coord_t x, coord_t y);

void updateSpecialDataPhantom(SpecialData * special_data, Vector8 phantom, char * phantom_notation);
void updateSpecialDataPhantomRaw(SpecialData * special_data, coord_t x, coord_t y, char * phantom_notation);

void setSpecialDataNotation(SpecialData * special_data, char * notation);

void normaliseSpecialData(SpecialData * special_data, TeamDirection direction);

void freeSpecialData(SpecialData * special_data);

#endif //CHESS_SPECIALDATA_H
