#include "version.h"

VersionInfo getVersionInfo(version_t version) {
    switch (version) {
        case 0:
            return (VersionInfo){.name = "v0.0 Alpha", .min_scenario_version = 0};  // first data storage
        case 1:
            return (VersionInfo){.name = "v0.1 Alpha", .min_scenario_version = 1};  // boards
        case 2:
            return (VersionInfo){.name = "v0.2 Alpha", .min_scenario_version = 2};  // wide chars
        case 3:
            return (VersionInfo){.name = "v0.3 Alpha", .min_scenario_version = 3};  // menu selectors
        case 4:
            return (VersionInfo){.name = "v0.4 Alpha", .min_scenario_version = 4};  // new rendering
        case 5:
            return (VersionInfo){.name = "v0.4.1 Alpha", .min_scenario_version = 5};  // scenario author
        case 6:
            return (VersionInfo){.name = "v0.5 Alpha", .min_scenario_version = 5};  // first gameplay
        case 7:
            return (VersionInfo){.name = "v0.5.1 Alpha", .min_scenario_version = 5};  // vectors
        case 8:
            return (VersionInfo){.name = "v0.6 Alpha", .min_scenario_version = 8};  // paths
        case 9:
            return (VersionInfo){.name = "v0.6.1 Alpha", .min_scenario_version = 8};  // path types
        case 10:
            return (VersionInfo){.name = "v0.7 Alpha", .min_scenario_version = 10};  // exceptions
        case 11:
            return (VersionInfo){.name = "v0.8 Beta", .min_scenario_version = 11};  // new validation
        case 12:
            return (VersionInfo){.name = "v0.8.1 Beta", .min_scenario_version = 12};  // scenario version check
        case 13:
            return (VersionInfo){.name = "v0.8.2 Beta", .min_scenario_version = 12};  // correct memory de-allocation
        default:
            return (VersionInfo){.name = "Unknown", .min_scenario_version = UINT16_MAX};
    }
}

char * getVersionName(version_t version) {
    return getVersionInfo(version).name;
}

version_t getMinSupportedScenarioVersion(version_t version) {
    return getVersionInfo(version).min_scenario_version;
}
