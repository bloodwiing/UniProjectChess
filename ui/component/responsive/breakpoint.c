#include "breakpoint.h"

ResponsiveBreakpoint createResponsiveBreakpoint(size_t height, size_t width, ResponsiveLayout layout, ResponsiveBreakpointType type) {
    return (ResponsiveBreakpoint){
        .height = height,
        .width = width,
        .layout = layout,
        .type = type
    };
}

bool_t checkResponsiveBreakpoint(ResponsiveBreakpoint breakpoint, ConSize screen) {
    if (breakpoint.type == ResponsiveBreakpointType_AND)
        return screen.height <= breakpoint.height && screen.width <= breakpoint.width;
    return screen.height <= breakpoint.height || screen.width <= breakpoint.width;
}
