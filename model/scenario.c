#include "scenario.h"

#include <string.h>
#include <wchar.h>
#include <assert.h>

#define STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS offsetof(Scenario, spawn_count) + sizeof(spawn_index_t)

#define SCENARIO_HEADER_SIZE 4
#define SCENARIO_HEADER_STRING "CHSS"

#define EXCEPTION_SCENARIO_NO_HEADER 0x20001, false, "The Scenario file is not supported"

Scenario * createScenario(char * name, char * author, ucoord_t size_x, ucoord_t size_y) {
    Scenario * out = calloc(1, sizeof(Scenario));
    out->version = BUILD_VERSION;
    strncpy(out->name, name, SCENARIO_MAX_STRING_LEN);
    strncpy(out->author, author, SCENARIO_MAX_STRING_LEN);
    out->size_x = size_x;
    out->size_y = size_y;
    return out;
}

void saveScenario(Scenario * scenario, FILE * stream, bool_t with_header) {
    if (with_header) {
        fwrite(SCENARIO_HEADER_STRING, sizeof(char), SCENARIO_HEADER_SIZE, stream);
        fwrite(scenario, STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS, 1, stream);
    } else {
        fwrite((uint8_t *)scenario + offsetof(Scenario, name), STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS - offsetof(Scenario, name), 1, stream);
    }

    for (team_index_t i = 0; i < scenario->team_count;)
        saveTeam(scenario->teams[i++], stream);
    for (spawn_index_t i = 0; i < scenario->spawn_count;)
        saveSpawn(scenario->spawns[i++], stream);
}

Scenario * loadScenario(FILE * stream, bool_t with_header, Exception * exception) {
    Scenario * out = calloc(1, sizeof(Scenario));

    if (with_header) {  // if the header should be looked for
        long prev = ftell(stream);
        char start[SCENARIO_HEADER_SIZE];
        if ((prev == 0L) && (fread(start, sizeof(char), SCENARIO_HEADER_SIZE, stream) == SCENARIO_HEADER_SIZE) && (strncmp(start, SCENARIO_HEADER_STRING, SCENARIO_HEADER_SIZE) == 0)) {  // if the file starts with 'CHSS'
            fread(out, STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS, 1, stream);
        } else {  // header failed: return error
            fseek(stream, prev, SEEK_SET);
            free(out);
            updateException(exception, EXCEPTION_SCENARIO_NO_HEADER);
            return NULL;
        }
    }
    else {
        out->version = VERSION_UNKNOWN;
        fread((uint8_t *)out + offsetof(Scenario, name), STRUCT_SCENARIO_SIZE_WITHOUT_POINTERS - offsetof(Scenario, name), 1, stream);
    }

    out->teams = calloc(out->team_count, sizeof(Team));
    for (team_index_t i = 0; i < out->team_count;)
        out->teams[i++] = loadTeam(stream);

    out->spawns = calloc(out->spawn_count, sizeof(Spawn));
    for (spawn_index_t i = 0; i < out->spawn_count;)
        out->spawns[i++] = loadSpawn(stream);

    return out;
}

void printScenario(Scenario * scenario) {
    wprintf(L"Scenario: \n"
             "\tVersion: %hs\n"
             "\tName: %hs\n"
             "\tAuthor: %hs\n"
             "\tSize X: %d\n"
             "\tSize Y: %d\n"
             "\tTeams:\n",
             getVersionName(scenario->version), scenario->name, scenario->author, scenario->size_x, scenario->size_y);
    for (team_index_t i = 0; i < scenario->team_count; i++)
        printTeam(scenario->teams[i]);

    wprintf(L"\tSpawns:\n");
    for (spawn_index_t i = 0; i < scenario->spawn_count; i++)
        printSpawnResolved(scenario->spawns + i, scenario);
}

void printSpawnResolved(Spawn * spawn, Scenario * scenario) {
    Team * team = scenario->teams + spawn->team;
    Piece * piece = team->pieces + spawn->type;
    wprintf(L"Spawn (Resolved): \n"
             "\tX: %d\n"
             "\tY: %d\n"
             "\tTeam: %hs\n"
             "\tPiece: %hs\n",
             spawn->x, spawn->y, team->name, piece->name);
}

void addTeam(Scenario * scenario, Team team) {
    scenario->teams = realloc(scenario->teams, sizeof(Team) * ++scenario->team_count);
    memset(&scenario->teams[scenario->team_count - 1], 0, sizeof(Team));
    scenario->teams[scenario->team_count - 1] = team;
}

void addSpawn(Scenario * scenario, Spawn spawn) {
    scenario->spawns = realloc(scenario->spawns, sizeof(Spawn) * ++scenario->spawn_count);
    memset(&scenario->spawns[scenario->spawn_count - 1], 0, sizeof(Spawn));
    scenario->spawns[scenario->spawn_count - 1] = spawn;
}

Rect getScenarioRect(Scenario * scenario, int x, int y) {
    return createRect(x, y, scenario->size_x * 2 + 3, scenario->size_y + 2);
}

Rect getScenarioRectWithinRect(Scenario * scenario, int x, int y, Rect rect) {
    Rect original = getScenarioRect(scenario, x, y);
    return fitRect(original, rect.width, rect.height);
}

Rect getScenarioCenteredRect(Scenario * scenario, Rect rect, int cursor_x, int cursor_y) {
    Rect inner = getScenarioRectWithinRect(scenario, 0, 0, rect);
    Rect centered = centerRectInRect(inner.width, inner.height, rect);

    Rect bounded = getScenarioRectWithinRect(scenario, cursor_x * 2 - (inner.width - 3) / 2 + 1,
                                             cursor_y - (inner.height - 2) / 2, rect);
    bounded.x /= 2;

    if (bounded.width < rect.width)
        bounded.x = -(centered.x - 3) / 2 - 1;
    if (bounded.height < rect.height)
        bounded.y = -centered.y + 1;

    return bounded;
}

void freeScenario(Scenario * scenario) {
    for (team_index_t i = 0; i < scenario->team_count;)
        freeTeam(&scenario->teams[i++]);
    free(scenario->teams);
    free(scenario->spawns);
    free(scenario);
}
