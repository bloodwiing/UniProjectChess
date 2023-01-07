#include "responsive.h"

#include <stdlib.h>

#include "ui/con_lib.h"

ResponsiveManager * createResponsiveManager() {
    ResponsiveManager * out = calloc(1, sizeof(ResponsiveManager));
    return out;
}

void addResponsiveBreakpoint(ResponsiveManager * manager, ResponsiveBreakpoint breakpoint) {
    manager->breakpoints = realloc(manager->breakpoints, sizeof(ResponsiveBreakpoint) * ++manager->breakpoint_count);
    manager->breakpoints[manager->breakpoint_count - 1] = breakpoint;
}

void renderResponsive(ResponsiveManager * manager) {
    ConSize size = con_get_size();
    for (size_t i = 0; i < manager->breakpoint_count; i++) {
        ResponsiveBreakpoint breakpoint = manager->breakpoints[i];
        if (checkResponsiveBreakpoint(breakpoint, size)) {
            runLayout(breakpoint.layout, getScreenRect());
            break;
        }
    }
}

void freeResponsiveBreakpoint(ResponsiveManager * manager) {
    free(manager->breakpoints);
    free(manager);
}
