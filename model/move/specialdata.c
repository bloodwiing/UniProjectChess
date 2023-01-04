#include "specialdata.h"

#include <stdlib.h>
#include <string.h>

SpecialData createSpecialData(Vector8 vector, bool_t is_first_move, bool_t is_check_safe, bool_t normalised) {
    return (SpecialData){
        .normalised = normalised,
        .vector = vector,
        .is_first_move = is_first_move,
        .is_check_safe = is_check_safe,
        .is_phantom = false,
        .phantom = VECTOR8_ZERO
    };
}

SpecialData createSpecialDataRaw(coord_t x, coord_t y, bool_t is_first_move, bool_t is_check_safe, bool_t normalised) {
    return createSpecialData(createVector8(x, y), is_first_move, is_check_safe, normalised);
}

void saveSpecialData(SpecialData special_data, FILE * stream) {
    fwrite(&special_data.vector, sizeof(Vector8), 1, stream);
    fwrite(&special_data.is_first_move, sizeof(bool_t), 1, stream);
    fwrite(&special_data.is_check_safe, sizeof(bool_t), 1, stream);

    fwrite(&special_data.condition_count, sizeof(count_t), 1, stream);
    fwrite(special_data.conditions, sizeof(Vector8), special_data.condition_count, stream);

    fwrite(&special_data.is_phantom, sizeof(bool_t), 1, stream);
    fwrite(&special_data.phantom, sizeof(Vector8), 1, stream);
}

SpecialData loadSpecialData(FILE * stream) {
    SpecialData out = {};
    fread(&out.vector, sizeof(Vector8), 1, stream);
    fread(&out.is_first_move, sizeof(bool_t), 1, stream);
    fread(&out.is_check_safe, sizeof(bool_t), 1, stream);

    fread(&out.condition_count, sizeof(count_t), 1, stream);
    out.conditions = calloc(out.condition_count, sizeof(Vector8));
    fread(out.conditions, sizeof(Vector8), out.condition_count, stream);

    fread(&out.is_phantom, sizeof(bool_t), 1, stream);
    fread(&out.phantom, sizeof(Vector8), 1, stream);
    return out;
}

void printSpecialData(SpecialData special_data) {
    wprintf(L"SpecialMove: \n"
             "\tX: %d\n"
             "\tY: %d\n"
             "\tIs First move?: %hs\n"
             "\tIs Check safe?: %hs\n"
             "\tIs Phantom?: %hs\n"
             "\tPhantom X: %d\n"
             "\tPhantom Y: %d\n"
             "\tConditions: %d\n",
             special_data.vector.x, special_data.vector.y, bool2str(special_data.is_first_move),
             bool2str(special_data.is_check_safe), bool2str(special_data.is_phantom),
             special_data.phantom.x, special_data.phantom.y);
    for (count_t i = 0; i < special_data.condition_count; i++)
        wprintf(L"\t\tX: %d\n"
                 "\t\tY: %d\n",
                 special_data.conditions[i].x, special_data.conditions[i].y);
}

void addSpecialDataCondition(SpecialData * special_data, Vector8 condition) {
    special_data->conditions = realloc(special_data->conditions, sizeof(Vector8) * ++special_data->condition_count);
    memset(&special_data->conditions[special_data->condition_count - 1], 0, sizeof(Vector8));
    special_data->conditions[special_data->condition_count - 1] = condition;
}

void addSpecialDataConditionRaw(SpecialData * special_data, coord_t x, coord_t y) {
    addSpecialDataCondition(special_data, createVector8(x, y));
}

void updateSpecialDataPhantom(SpecialData * special_data, Vector8 phantom) {
    special_data->is_phantom = true;
    special_data->phantom = phantom;
}

void updateSpecialDataPhantomRaw(SpecialData * special_data, coord_t x, coord_t y) {
    updateSpecialDataPhantom(special_data, createVector8(x, y));
}

void normaliseSpecialData(SpecialData * special_data, TeamDirection direction) {
    if (special_data->normalised)
        return;

    special_data->vector = normaliseVector8(special_data->vector, direction);
    special_data->phantom = normaliseVector8(special_data->phantom, direction);

    for (count_t i = 0; i < special_data->condition_count; i++)
        special_data->conditions[i] = normaliseVector8(special_data->conditions[i], direction);

    special_data->normalised = true;
}

void freeSpecialData(SpecialData * special_data) {
    free(special_data->conditions);
}
