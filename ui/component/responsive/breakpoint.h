#ifndef CHESS_RESPONSIVE_BREAKPOINT_H
#define CHESS_RESPONSIVE_BREAKPOINT_H

#include <stdint.h>

#include "./layout.h"

#include "abstract/defs.h"

#include "ui/con_lib.h"

typedef enum ResponsiveBreakpointType {
    ResponsiveBreakpointType_AND,
    ResponsiveBreakpointType_OR
} ResponsiveBreakpointType;

typedef struct ResponsiveBreakpoint {
    size_t height, width;
    ResponsiveLayout layout;
    ResponsiveBreakpointType type;
} ResponsiveBreakpoint;

/// Creates a new breakpoint that contains a layout which will only render if the given size condition is met
ResponsiveBreakpoint createResponsiveBreakpoint(size_t height, size_t width, ResponsiveLayout layout, ResponsiveBreakpointType type);

/// Checks if the breakpoint has the console size condition met
bool_t checkResponsiveBreakpoint(ResponsiveBreakpoint breakpoint, ConSize screen);

#endif //CHESS_RESPONSIVE_BREAKPOINT_H
