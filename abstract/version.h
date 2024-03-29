#ifndef CHESS_VERSION_H
#define CHESS_VERSION_H

#include <stdint.h>

#define BUILD_VERSION 24

typedef uint16_t version_t;

#define VERSION_UNKNOWN UINT16_MAX

typedef struct VersionInfo {
    char * name;
    version_t min_scenario_version;
} VersionInfo;

VersionInfo getVersionInfo(version_t version);
char * getVersionName(version_t version);
version_t getMinSupportedScenarioVersion(version_t version);

#endif //CHESS_VERSION_H
