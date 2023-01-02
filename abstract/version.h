#ifndef CHESS_VERSION_H
#define CHESS_VERSION_H

#include <stdint.h>

#define BUILD_VERSION 12

typedef struct VersionInfo {
    char * name;
    uint16_t min_scenario_version;
} VersionInfo;

VersionInfo getVersionInfo(uint16_t version);
char * getVersionName(uint16_t version);
int getMinSupportedScenarioVersion(uint16_t version);

#endif //CHESS_VERSION_H
