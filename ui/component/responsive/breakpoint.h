#ifndef CHESS_RESPONSIVE_BREAKPOINT_H
#define CHESS_RESPONSIVE_BREAKPOINT_H

#include <stdint.h>

#include "./layout.h"

#include "abstract/defs.h"

#include "ui/con_lib.h"

typedef struct ResponsiveBreakpoint {
    size_t height, width;
    ResponsiveLayout layout;
} ResponsiveBreakpoint;

ResponsiveBreakpoint createResponsiveBreakpoint(size_t height, size_t width, ResponsiveLayout layout);

bool_t checkResponsiveBreakpoint(ResponsiveBreakpoint breakpoint, ConSize screen);

#endif //CHESS_RESPONSIVE_BREAKPOINT_H
