#ifndef CHESS_RESPONSIVE_H
#define CHESS_RESPONSIVE_H

#include <stdint.h>

#include "./breakpoint.h"

typedef struct ResponsiveManager {
    size_t breakpoint_count;
    ResponsiveBreakpoint * breakpoints;
} ResponsiveManager;

ResponsiveManager * createResponsiveManager();
void addResponsiveBreakpoint(ResponsiveManager * manager, ResponsiveBreakpoint breakpoint);

void renderResponsive(UserSettings * settings, ResponsiveManager * manager);

void freeResponsiveBreakpoint(ResponsiveManager * manager);

#endif //CHESS_RESPONSIVE_H
