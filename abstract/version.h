#ifndef CHESS_VERSION_H
#define CHESS_VERSION_H

#include <stdint.h>

#define BUILD_VERSION 13

typedef uint16_t version_t;

typedef struct VersionInfo {
    char * name;
    version_t min_scenario_version;
} VersionInfo;

VersionInfo getVersionInfo(version_t version);
char * getVersionName(version_t version);
version_t getMinSupportedScenarioVersion(version_t version);

#endif //CHESS_VERSION_H
