#ifndef CHESS_RESPONSIVE_H
#define CHESS_RESPONSIVE_H

#include <stdint.h>

#include "./breakpoint.h"

typedef struct ResponsiveManager {
    size_t breakpoint_count;
    ResponsiveBreakpoint * breakpoints;
} ResponsiveManager;

/// Creates a new dynamic responsive manager
ResponsiveManager * createResponsiveManager();

/// Adds a new responsive breakpoint
void addResponsiveBreakpoint(ResponsiveManager * manager, ResponsiveBreakpoint breakpoint);


/// Picks a breakpoint that fits within the console size and renders it
void renderResponsive(UserSettings * settings, ResponsiveManager * manager);


/// Clears up used memory
void freeResponsiveManager(ResponsiveManager * manager);

#endif //CHESS_RESPONSIVE_H
